require "socket"
require 'optparse'

class Client
  def initialize(port, ai_path)
    exit 1 unless @server_socket = TCPSocket.new("punter.inf.ed.ac.uk", port)
    exit 1 unless @ai_socket = IO.popen(ai_path, "r+")
  end

  def handshake
    send_msg_to_server('12:{"me":"bob"}')
    next_server_message
  end

  def next_server_message
    ret = receive_messege(server_socket)
    log "<- #{ret}"
  end

  def next_ai_message
    ret = receive_messege(ai_socket)
    log "-> #{ret}"
  end

  def send_msg_to_ai(msg)
    log "<- #{msg}"
    ai_socket.puts(msg)
  end

  def send_msg_to_server(msg)
    log "-> #{msg}"
    server_socket.puts(msg)
  end

  def receive_messege(stream)
    len = 0 
    while c = stream.getc
      break if c == ":"
      len *= 10
      len += c.to_i
    end
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
    STDERR.puts(msg)
  end

end



port =  nil
ai_path = nil
opt = OptionParser.new
opt.on('-p', '--port PORT') {|v| port =  v }
opt.on('-a', '--ai AI_PATH') {|v| ai_path =  v }
opt.parse(ARGV)

client = Client.new(port.to_i, ai_path)

# handshake
client.handshake

# setup
setup_msg = client.next_server_message
client.send_msg_to_ai setup_msg

# ready
# client.send_msg_to_server '11:{"ready":0}'
ready_msg = client.next_ai_message
client.send_msg_to_server ready_msg

while msg = client.next_server_message
  ai.send_msg_to_ai msg
end

