require 'json'
require 'optparse'
require_relative 'libs/server'

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

opts = {
  map: JSON.load(File.read(map_file)),
  num_of_punters: num_of_punters,
  port: port
}
app = Server.new(opts)
app.run_game_once
