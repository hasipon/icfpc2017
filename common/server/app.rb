require 'json'
require 'optparse'
require_relative 'libs/server'

opt = OptionParser.new

num_of_punters = nil
port = nil
map_file = nil
map_json = nil
settings = {}
timeout_setup = 10
timeout_gameplay = 1
mode = :online
punters = []

opt.on('-n', '--num-of-punters NUM') { |v| num_of_punters = v.to_i }
opt.on('-p', '--port PORT') { |v| port = v.to_i }
opt.on('-m', '--map-file FILENAME') { |v| map_file = v }
opt.on('-j', '--map-json MAP_JSON') { |v| map_json = v }
opt.on('-s', '--settings SETTINGS_JSON') { |v| settings = JSON.load(v) }
opt.on('--timeout-setup') { |v| timeout_setup = v }
opt.on('--timeout-gameplay') { |v| timeout_gameplay = v }
opt.on('--mode MODE') { |v| mode = v.to_sym }
opt.on('--punters p1,p2,p3') { |v| punters = v.split(",") }

opt.parse!(ARGV)

if !num_of_punters || !port || (!map_file && !map_json)
  $stderr.puts "Usage: ruby app.rb -n NUM_OF_PUNTERS -p PORT -m MAP_FILE"
  exit 1
end

if map_file && map_json
  $stderr.puts "specify map_file or map_json"
  exit 1
end

if mode == :offine && punters.length == 0
  $stderr.puts "specify punter paths when offline mode"
  exit 1
end

map = (map_file && JSON.load(File.read(map_file))) ||
      (map_json && JSON.load(map_json))

opts = {
  map: map,
  num_of_punters: num_of_punters,
  port: port,
  settings: settings,
  timeout_setup: timeout_setup,
  timeout_gameplay: timeout_gameplay
}

app = Server.new(opts)

if mode == :online
  app.run_game_once
elsif mode == :offline
  app.run_game_once_offline(punters)
end
  
