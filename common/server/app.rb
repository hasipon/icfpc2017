require 'json'
require 'optparse'
require_relative 'libs/server'

opt = OptionParser.new

# common
map_file = nil
map_json = nil
settings = {}
timeout_setup = 10
timeout_gameplay = 1
mode = :online
logfile = nil
$debug = 0

# online
num_of_punters = nil
port = nil

# offline
punters = []

# common
opt.on('--mode MODE') { |v| mode = v.to_sym }
opt.on('-m', '--map-file FILENAME') { |v| map_file = v }
opt.on('-j', '--map-json MAP_JSON') { |v| map_json = v }
opt.on('-s', '--settings SETTINGS_JSON') { |v| settings = JSON.load(v) }
opt.on('--timeout-setup') { |v| timeout_setup = v }
opt.on('--timeout-gameplay') { |v| timeout_gameplay = v }
opt.on('--logfile FILENAME') { |v| logfile = v }
opt.on('--debug') { |v| $debug = 1 }

# online
opt.on('-n', '--num-of-punters NUM') { |v| num_of_punters = v.to_i }
opt.on('-p', '--port PORT') { |v| port = v.to_i }

# offline
opt.on('--punters p1,p2,p3') { |v| punters = v.split(",") }

opt.parse!(ARGV)

if (map_file && map_json) || (!map_file && !map_json)
  $stderr.puts "specify map_file or map_json"
  exit 1
end

map = (map_file && JSON.load(File.read(map_file))) ||
      (map_json && JSON.load(map_json))

logfile = "/dev/null" unless logfile

opts = {
  map: map,
  settings: settings,
  timeout_setup: timeout_setup,
  timeout_gameplay: timeout_gameplay,
  logfile: logfile
}

if mode == :online
  if !num_of_punters || !port
    $stderr.puts "specify num_of_punters and port"
    exit 1
  end
  opts[:port] = port
  opts[:num_of_punters] = num_of_punters

  Server.new(opts).run_game_once
elsif mode == :offline
  if punters.length == 0
    $stderr.puts "specify punter paths when offline mode"
    exit 1
  end
  opts[:punters] = punters

  Server.new(opts).run_game_once_offline
else
  $stderr.puts "mode is online or offline"
  exit 1
end
