require 'socket'
require 'json'
require 'monitor'
require 'optparse'

require_relative 'lib/barrier'
require_relative 'lib/score'

# 12:{"me":"bob"}
# 44:{"claim":{"punter":0,"source":0,"target":1}}
# 44:{"claim":{"punter":1,"source":1,"target":2}}

class IO
  def read_message
    len = 0
    while c = self.getc
      break if c == ":"
      len = len * 10 + c.to_i
    end
    JSON.load(self.read(len))
  end

  def send_message(obj)
    message = JSON.generate(obj)
    self.puts "#{message.length}:#{message}"
  end
end

opt = OptionParser.new

num_of_punters = nil
port = nil
map_file = nil

opt.on('-n', '--num-of-punters NUM') { |v| num_of_punters = v.to_i }
opt.on('-p', '--port PORT') { |v| port = v.to_i }
opt.on('-m', '--map-file FILENAME') { |v| map_file = v }

opt.parse!(ARGV)

if !num_of_punters || !port || !map_file
  $stderr.puts "Usage: ruby app.rb -n NUM_OF_PUNTERS -p PORT -m MAP_FILE"
  exit 1
end

@map = JSON.load(File.read(map_file))
@score = Score.new(@map, num_of_punters)
@result = nil
@moves_last = nil

server = TCPServer.open(port)
puts "Start server with num_of_punters = #{num_of_punters}"

@semaphore = Mutex.new
@barrier = Barrier.new(num_of_punters)

@punters = []
@threads = []
@moves = []

# initial move
num_of_punters.times do |i|
  @moves.push({"pass" => { "punter" => i }})
end

num_of_punters.times do
  @threads.push(
    Thread.start(server.accept) do |socket|
      p socket.peeraddr

      # HANDSHAKE
      handshake = socket.read_message

      name = handshake["me"]

      punter_id = nil
      @semaphore.synchronize do
        @punters.push(name)
        punter_id = @punters.length - 1
      end

      socket.send_message({"you" => name})

      @barrier.sync

      # SETUP
      setup = {
        "punter" => punter_id,
        "punters" => num_of_punters,
        "map" => @map
      }
      socket.send_message(setup)

      # read ready
      socket.read_message

      # GAMEPLAY
      play = nil
      (@map["rivers"].length / @punters.length).times do
        socket.send_message({"move" => {"moves" => @moves}})

        @barrier.sync

        @semaphore.synchronize do
          unless @moves.empty?
            @score.update(@moves)
            @moves = []
          end
        end

        @barrier.sync

        # TODO: timeout
        play = socket.read_message

        @semaphore.synchronize do
          @moves.push(play)
        end

        @barrier.sync
      end

      @semaphore.synchronize do
        unless @moves.empty?
          @score.update(@moves)
          @moves_last = @moves
          @moves = []
        end
      end

      @semaphore.synchronize do
        unless @result
          @result = @score.calc
        end
      end

      stop = {
        "stop" => {
          "moves" => @moves_last,
          "scores" => @result
        }
      }
      socket.send_message stop

      socket.close
    end
  )
end

@threads.each { |t| t.join }

server.close
