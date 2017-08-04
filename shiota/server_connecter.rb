require "socket"

class Client
  def initialize
    @server_socket = TCPSocket.new("punter.inf.ed.ac.uk", 9100)
  end

  def handshake
    send_msg_to_server('12:{"me":"bob"}')
    next_server_message
  end

  def next_server_message
    ret = receive_messege(server_socket)
    log "<- #{ret}"
  end

  def send_msg_to_client(msg)
    puts(msg)
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
    stream.read(len)
  end

  def server_socket
    @server_socket
  end

  def log(msg)
    STDERR.puts(msg)
  end

end

client = Client.new
client.handshake

setup_msg = client.next_server_message
# client.send_msg_to_client setup_msg

client.send_msg_to_server '11:{"ready":0}'

client.next_server_message

