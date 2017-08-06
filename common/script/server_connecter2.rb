require "socket"
require 'optparse'
require 'pp'
require 'json'

class Timer
  def initialize
  end

  def epoc
    Time.now.strftime('%s%L').to_i
  end

  def begin
    @begin = epoc
  end

  def finish
    epoc - @begin
  end
end

class Client
  def initialize(host, port, ai_path, name, logfile, verbose)
    @ai_path = ai_path
    @name = "#{name}@#{Time.now.to_i}"
    @log_file = open(logfile || logfile_name, mode = "w")
    @verbose = verbose
    @turn = 0
    exit 1 unless @server_socket = TCPSocket.new(host, port)
  end

  def handshake
    msg = '{"me":"'+ @name + '"}'
    send_msg_to_server("#{msg.length}:#{msg}")
    next_server_message
  end

  def next_server_message
    log "wait server message"
    ret = receive_messege(server_socket)
    log "me <- server\n#{ret}"
    response_log ret.split(':', 2)[1]
    ret
  end

  def next_ai_message
    log "wait ai message"
    ret = receive_messege(ai_socket)
    log "ai -> me\n#{ret}" if @verbose
    ret
  end

  def send_msg_to_ai(msg)
    json = msg_to_json(msg)
    json['state'] = state if state
    msg = json_to_msg(json)
    log "ai <- me\n#{msg}" if @verbose
    ai_socket.print(msg)
  end

  def send_msg_to_server(msg)
    json = msg_to_json(msg)
    @state = json['state']
    json.delete 'state'
    msg = json_to_msg(json)
    log "me -> server\n#{msg}"
    server_socket.print(json_to_msg(json))
  end

  def receive_messege(stream)
    len = 0 
    while c = stream.getc
      break if c == ":"
      len *= 10
      len += c.to_i
    end
    return nil if len == 0
    msg = stream.read(len)
    "#{len}:#{msg}"
  end

  def msg_to_json(msg)
    JSON.load(msg.split(':', 2)[1])
  end

  def json_to_msg(json)
    payload = JSON.generate(json)
    "#{payload.length}:#{payload}"
  end


  def server_socket
    @server_socket
  end

  def ai_socket
    @ai_socket
  end

  def state
    @state
  end

  def log(msg)
    STDOUT.puts("turn:#{@turn} #{msg}")
  end

  def response_log(msg)
    @log_file.puts(msg.gsub(/(\r\n|\r|\n|\f)/,""))
    @log_file.flush
  end

  def logfile_name
    "#{@name}.log"
  end

  def ai_path
    @ai_path
  end

  def handshake_with_client
    if @pid
      Process.detach(@pid)
      Process.kill('KILL', @pid)
    end
    exit 1 unless @ai_socket = IO.popen(ai_path, "r+")
    @pid = @ai_socket.pid
    msg = next_ai_message
    json = msg_to_json(msg)
    json = {"you" =>  json["me"]}
    log "handshaked with ai"
    send_msg_to_ai(json_to_msg(json))
  end

  def increment_turn
    @turn = @turn+1
  end
end



port =  nil
ai_path = nil
host = nil
name = "bob"
quiet = nil
opt = OptionParser.new
logfile = nil
verbose = nil
opt.on('-p', '--port PORT') {|v| port =  v }
opt.on('-h', '--host HOST') {|v| host =  v }
opt.on('-a', '--ai AI_PATH') {|v| ai_path =  v }
opt.on('-n', '--name NAME') {|v| name =  v }
opt.on('-q', '--quiet') {|v| quiet =  v }
opt.on('-l', '--log LOGFILE') {|v| logfile =  v }
opt.on('-v', '--verbose') {|v| verbose =  v }
opt.parse(ARGV)

client = Client.new(host, port.to_i, ai_path, name, logfile, verbose)
timer = Timer.new

# handshake
client.handshake

# setup
client.handshake_with_client
setup_msg = client.next_server_message
timer.begin
client.send_msg_to_ai(setup_msg)
setup_time = timer.finish
puts "setup end. time: #{setup_time}[ms]"

# ready
# client.send_msg_to_server '11:{"ready":0}'
ready_msg = client.next_ai_message
client.send_msg_to_server(ready_msg)

puts "ready end"

client.increment_turn
max_ai_time = 0
while msg = client.next_server_message
  client.handshake_with_client
  break_flag = client.msg_to_json(msg)['stop']

  timer.begin
  client.send_msg_to_ai msg
  break if break_flag
  msg = client.next_ai_message
  ai_time = timer.finish
  max_ai_time = ai_time if ai_time > max_ai_time
  unless msg
    puts "!!AI failed to return response!!"
    exit 1
  end
  puts "AI returned response time: #{ai_time}[ms] (max: #{max_ai_time})"
  client.send_msg_to_server msg
  client.increment_turn
end

puts "setup time #{setup_time}[ms]. max think time #{max_ai_time}[ms]."
puts "logfile: #{client.logfile_name}"

`curl -F 'file=@#{client.logfile_name}' http://13.114.38.186/uploadlog` unless quiet
