require 'socket'
require_relative 'score'

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

class Server
  def initialize(opts)
    @map = opts[:map]
    @num_of_punters = opts[:num_of_punters]
    @server = TCPServer.open(opts[:port])

    @max_play_count = @map["rivers"].length
  end

  def run_game_once
    puts "Start a game with #{@num_of_punters} punters."

    score = Score.new(@map, @num_of_punters)

    # accept clients
    sockets = []
    @num_of_punters.times do |idx|
      puts "accept #{idx}"
      socket = @server.accept
      sockets.push socket
      p socket.peeraddr
    end

    # handshake
    names = []
    sockets.each do |socket|
      handshake = socket.read_message
      name = handshake["me"]
      socket.send_message({"you" => name})
      names.push(name)
    end

    # setup
    sockets.each_with_index do |socket, index|
      setup = {
        "punter" => index,
        "punters" => @num_of_punters,
        "map" => @map
      }
      socket.send_message setup
      socket.read_message # read ready
    end

    # game play
    play_count = 0
    moves = []
    @num_of_punters.times do |idx|
      moves.push({"pass" => {"punter" => idx}})
    end
    current_moves = []
    while true
      break if play_count >= @max_play_count
      sockets.each_with_index do |socket, index|
        # TODO: timeout, zombie
        socket.send_message({"move" => {"moves" => moves}})
        current_moves.push socket.read_message
        play_count += 1
      end
      score.update(current_moves)
      moves = current_moves
      current_moves = []
    end

    # stop
    stop = {
      "stop" => {
        "moves" => moves,
        "scores" => score.calc
      }
    }
    sockets.each do |socket|
      socket.send_message stop
      socket.close
    end
  end
end
