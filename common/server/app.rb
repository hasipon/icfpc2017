require 'json'
require 'optparse'
require_relative 'libs/server'

opt = OptionParser.new

num_of_punters = nil
port = nil
map_file = nil
map_json = nil
settings = {}

opt.on('-n', '--num-of-punters NUM') { |v| num_of_punters = v.to_i }
opt.on('-p', '--port PORT') { |v| port = v.to_i }
opt.on('-m', '--map-file FILENAME') { |v| map_file = v }
opt.on('-j', '--map-json MAP_JSON') { |v| map_json = v }
opt.on('-s', '--settings SETTINGS_JSON') { |v| settings = JSON.load(v) }

opt.parse!(ARGV)

if !num_of_punters || !port || (!map_file && !map_json)
  $stderr.puts "Usage: ruby app.rb -n NUM_OF_PUNTERS -p PORT -m MAP_FILE"
  exit 1
end

if map_file && map_json
  $stderr.puts "specify map_file or map_json"
  exit 1
end

map = (map_file && JSON.load(File.read(map_file))) ||
      (map_json && JSON.load(map_json))

opts = {
  map: map,
  num_of_punters: num_of_punters,
  port: port,
}
opts[:settings] = settings if settings

app = Server.new(opts)
app.run_game_once
