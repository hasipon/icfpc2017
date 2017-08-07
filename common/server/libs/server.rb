# coding: utf-8
require 'socket'
require 'timeout'
require_relative 'score'

class IO
  def _read_message
    len = 0
    while c = self.getc
      break if c == ":"
      # raise "non-numeric character: #{c}" unless c =~ /[0-9]/
      len = len * 10 + c.to_i
    end
    raise "len shoud be a positive integer" if len == 0
    message = self.read(len)
    STDOUT.puts "S <- C: [#{len}:#{message}]" if $debug and $debug == 1
    JSON.load(message)
  end

  def read_message(retry_count = 5)
    raise "read_message: retry count == 0" if retry_count == 0
    begin
      _read_message
    rescue Errno::ECONNRESET
      read_message(retry_count - 1)
    end
  end

  def send_message(obj)
    message = JSON.generate(obj)
    if $debug and $debug == 1
      STDOUT.puts "S -> C: [#{message.length}:#{message}]"
    end
    self.print "#{message.length}:#{message}"
  end
end

class Server
  def initialize(opts)
    @map = opts[:map]
    @settings = opts[:settings]
    @timeout_setup = opts[:timeout_setup]
    @timeout_gameplay = opts[:timeout_gameplay]
    @logfile = File.open(opts[:logfile], 'w')

    @mode = opts[:mode]

    if @mode == :online
      @num_of_punters = opts[:num_of_punters]
      @port = opts[:port]
    end

    if @mode == :offline
      @punter_paths = opts[:punters]
      @num_of_punters = @punter_paths.length
    end

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

  # make a handshake via `io` and returns the name
  def make_handshake(io)
    hs = io.read_message
    name = hs["me"]
    io.send_message({"you" => name})
    name
  end

  def run_game_once_offline
    puts "Start a game with #{@num_of_punters} punters (offline)."

    futures = {}
    states = Array.new(@num_of_punters)

    # setup
    setup = {
      "punters" => @num_of_punters,
      "map" => @map,
      "settings" => @settings
    }
    punter_names = []
    @punter_paths.each_with_index do |punter_path, index|
      IO.popen(punter_path, "r+") do |io|
        punter_names << make_handshake(io)

        setup["punter"] = index

        io.send_message setup
        ready = io.read_message

        if @settings["futures"] && ready["futures"]
          futures[ready["ready"]] = normalize_futures(ready["futures"])
        end
        states[index] = ready["state"]
      end
    end
    # log output
    setup["punter"] = 0
    setup["punter_names"] = punter_names
    @logfile.puts(JSON.generate({"you" => punter_names[0]}))
    setup.delete("state")
    @logfile.puts(JSON.generate(setup))

    score = Score.new(@map, @num_of_punters, futures, @settings)

    # game play
    play_count = 0
    moves = []

    # initial move is pass
    @num_of_punters.times do |idx|
      moves.push({"pass" => {"punter" => idx}})
    end

    while true
      break if play_count >= @max_play_count - 1
      @punter_paths.each_with_index do |punter_path, index|
        break if play_count >= @max_play_count - 1

        puts "play_count: #{play_count}"
        p moves

        IO.popen(punter_path, "r+") do |io|
          make_handshake(io)

          message = nil
          move = nil
          begin
            Timeout.timeout(@timeout_gameplay) do
              message = {
                "move" => {
                  "moves" => moves
                },
                "state" => states[index]
              }
              io.send_message message
              move = io.read_message
            end
          rescue => e
            raise "error #{e}: client #{index} = #{punter_path}, message = #{message}"
          end
          states[index] = move["state"]
          move.delete("state")
          moves[index] = move

          if index == 0
            moves_tmp = moves
            moves_tmp.each { |m| m.delete("state") }
            @logfile.puts JSON.generate({"move"=>{"moves"=>moves_tmp}})
          end

          score.update(moves[index])
          play_count += 1
        end
      end
    end

    # stop
    stop = {
      "stop" => {
        "moves" => moves,
        "scores" => score.calc
      }
    }
    p stop

    stop_tmp = stop
    stop_tmp["stop"]["moves"].each do |s|
      s.delete("state")
    end
    @logfile.puts(JSON.generate(stop_tmp))

    @punter_paths.each_with_index do |punter_path, index|
      IO.popen(punter_path, "r+") do |io|
        make_handshake(io)

        stop["state"] = states[index]
        io.send_message stop
      end
    end

    @logfile.close
  end

  def run_game_once
    puts "Start a game with #{@num_of_punters} punters."

    server = TCPServer.open(@port)

    # accept clients
    sockets = []
    @num_of_punters.times do |idx|
      puts "accept #{idx}"
      socket = server.accept
      sockets.push socket
      p socket.peeraddr
    end

    # handshake
    names = []
    sockets.each do |socket|
      name = make_handshake(socket)
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

      # TODO: timeout
      socket.send_message setup
      puts "setup: #{setup}"
      ready = socket.read_message

      if ready["futures"]
        futures[ready["ready"]] = normalize_futures(ready["futures"])
      end
    end

    score = Score.new(@map, @num_of_punters, futures, @settings)

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
        break if play_count >= @max_play_count
        # TODO: zombie
        puts "send_message(play_count = #{play_count}, index = #{index})"
        p moves

        begin
          Timeout.timeout(@timeout_gameplay) do
            socket.send_message({"move" => {"moves" => moves}})
            moves[index] = socket.read_message
          end
        rescue ::Timeout::Error
          socket.send_message({"timeout" => @timeout_gameplay})
          # > If a punter fails to move within the specified time, then
          # > they will be made to pass for that turn.
          moves[index] = {"pass" => {"punter" => index}}
        end

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
