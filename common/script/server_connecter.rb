require "socket"
require 'optparse'
require 'pp'

class Client
  def initialize(host, port, ai_path, name)
    @name = "#{name}@#{Time.now.to_i}"
    @log_file = open(logfile_name, mode = "w")
    exit 1 unless @server_socket = TCPSocket.new(host, port)
    exit 1 unless @ai_socket = IO.popen(ai_path, "r+")
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
    # log "ai -> me\n#{ret}"
    ret
  end

  def send_msg_to_ai(msg)
    # log "ai <- me\n#{msg}"
    ai_socket.print(msg)
  end

  def send_msg_to_server(msg)
    log "me -> server\n#{msg}"
    server_socket.print(msg)
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

  def server_socket
    @server_socket
  end

  def ai_socket
    @ai_socket
  end

  def log(msg)
    STDOUT.puts(msg)
  end

  def response_log(msg)
    @log_file.puts(msg)
  end

  def logfile_name
    "#{@name}.log"
  end
end



port =  nil
ai_path = nil
host = nil
name = "bob"
opt = OptionParser.new
opt.on('-p', '--port PORT') {|v| port =  v }
opt.on('-h', '--host HOST') {|v| host =  v }
opt.on('-a', '--ai AI_PATH') {|v| ai_path =  v }
opt.on('-n', '--name NAME') {|v| name =  v }
opt.parse(ARGV)

client = Client.new(host, port.to_i, ai_path, name)

# handshake
client.handshake

# setup
setup_msg = client.next_server_message
client.send_msg_to_ai(setup_msg)

# ready
# client.send_msg_to_server '11:{"ready":0}'
ready_msg = client.next_ai_message
client.send_msg_to_server ready_msg

while msg = client.next_server_message
  client.send_msg_to_ai msg
  msg = client.next_ai_message
  break unless msg
  client.send_msg_to_server msg
end

puts "logfile: #{client.logfile_name}"

