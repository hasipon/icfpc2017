require 'optparse'
require 'set'
require 'json'

opt = OptionParser.new

w = nil
h = nil

opt.on('-w VAL') { |v| w = v.to_i }
opt.on('-h VAL') { |v| h = v.to_i }

opt.parse!(ARGV)

map = {
  "sites" => [],
  "rivers" => [],
  "mines" => []
}

h.times do |i|
  w.times do |j|
    id = i * h + j

    map["sites"].push({"id" => id, "x" => j, "y" => i})

    if i < h - 1 && rand(10) != 0
      r = {
        "source" => id,
        "target" => id + h
      }
      map["rivers"].push r
    end

    if j < w - 1 && rand(10) != 0
      r = {
        "source" => id,
        "target" => id + 1
      }
      map["rivers"].push r
    end

    if i < h - 1 && j < w - 1 && rand(10) == 0
      r = {
        "source" => id,
        "target" => id + h + 1
      }
      map["rivers"].push r
    end

    if i > 0 && j < w - 1 && rand(10) == 0
      r = {
        "source" => id,
        "target" => id - h + 1
      }
      map["rivers"].push r
    end

  end
end

mines = Set.new

(2 + rand(Math.sqrt(w * h).to_i)).times do
  mines.add(rand(w * h))
end

map["mines"] = mines.to_a

puts JSON.generate(map)
