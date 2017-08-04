require 'socket'
require 'json'
require 'monitor'
require 'optparse'

require './barrier'

# 12:{"me":"bob"}
# 44:{"claim":{"punter":0,"source":0,"target":1}}
# 44:{"claim":{"punter":1,"source":1,"target":2}}

sample_map = JSON.load('{   "sites": [     {"id": 0, "x": 0.0, "y": 0.0},     {"id": 1, "x": 1.0, "y": 0.0},     {"id": 2, "x": 2.0, "y": 0.0},     {"id": 3, "x": 2.0, "y": -1.0},     {"id": 4, "x": 2.0, "y": -2.0},     {"id": 5, "x": 1.0, "y": -2.0},     {"id": 6, "x": 0.0, "y": -2.0},     {"id": 7, "x": 0.0, "y": -1.0}   ],   "rivers": [     { "source": 0, "target": 1},     { "source": 1, "target": 2},     { "source": 0, "target": 7},     { "source": 7, "target": 6},     { "source": 6, "target": 5},     { "source": 5, "target": 4},     { "source": 4, "target": 3},     { "source": 3, "target": 2},     { "source": 1, "target": 7},     { "source": 1, "target": 3},     { "source": 7, "target": 5},     { "source": 5, "target": 3}   ],   "mines": [1, 5] }')

class IO
  def read_message
    len = 0
    while c = self.getc
      break if c == ":"
      len = len * 10 + c.to_i
    end
    JSON.load(self.read(len))
  end
end

def encode(obj)
  message = JSON.generate(obj)
  "#{message.length}:#{message}"
end

if ARGV.length != 2
  $stderr.puts "Usage: ruby app.rb [port] [num_of_punters]"
  exit 1
end

num_of_punters = ARGV[1].to_i
port = ARGV[0].to_i

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

      socket.puts encode({"you" => name})

      @barrier.sync

      # SETUP
      setup = {
        "punter" => punter_id,
        "punters" => num_of_punters,
        "map" => sample_map
      }
      socket.puts encode(setup)

      # GAMEPLAY
      play = nil
      (sample_map["rivers"].length / @punters.length).times do
        socket.puts encode({"move" => {"moves" => @moves}})

        @barrier.sync

        @semaphore.synchronize do
          @moves = [] unless @moves.empty?
        end

        @barrier.sync

        play = socket.read_message

        @semaphore.synchronize do
          @moves.push(play)
        end

        @barrier.sync
      end

      socket.puts "-- END --"
      socket.close
    end
  )
end

@threads.each { |t| t.join }

server.close
