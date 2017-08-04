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
  def initialize(socket)
    @socket = socket
    @punter_id = nil
  end

  def handle_handshake
    handshake = @socket.read_message
    handshake["me"] # name
  end

  def self.
end
