# coding: utf-8
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
  # opts = {
  #   map: mapのオブジェクト,
  #   num_of_punters: パンターの数,
  #   port: ポート,
  #   settings: セッティング
  # }
  def initialize(opts)
    @map = opts[:map]
    @num_of_punters = opts[:num_of_punters]
    @server = TCPServer.open(opts[:port])
    @settings = opts[:settings]

    @max_play_count = @map["rivers"].length
  end

  # source が mine じゃないやつは無視する
  # target が mine のやつも無視する
  # 同じ mine から複数の target が指定されている場合は最後の1つだけを採用する
  def normalize_futures(futures)
    ret = {}
    futures.each do |river|
      next if !@map["mines"].member? river["source"]
      next if @map["mines"].member? river["target"]
      ret[river["source"]] = river["target"]
    end
    ret.map { |s, t| { "source" => s, "target" => t } }
  end

  def run_game_once
    puts "Start a game with #{@num_of_punters} punters."

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
    futures = {}
    sockets.each_with_index do |socket, index|
      setup = {
        "punter" => index,
        "punters" => @num_of_punters,
        "punter_names" => names,
        "map" => @map,
        "settings" => @settings
      }
      socket.send_message setup

      puts "setup: #{setup}"

      ready = socket.read_message
      if ready["futures"]
        futures[ready["ready"]] = normalize_futures(ready["futures"])
      end
    end

    score = Score.new(@map, @num_of_punters, futures)

    # game play
    play_count = 0
    moves = []

    # initial move is pass
    @num_of_punters.times do |idx|
      moves.push({"pass" => {"punter" => idx}})
    end

    while true
      break if play_count >= @max_play_count
      sockets.each_with_index do |socket, index|
        # TODO: timeout, zombie
        puts "send_message(play_count = #{play_count}, index = #{index})"
        p moves
        socket.send_message({"move" => {"moves" => moves}})
        moves[index] = socket.read_message
        score.update(moves[index])
        play_count += 1
      end
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

    p stop
  end
end
