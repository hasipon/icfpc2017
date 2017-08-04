require 'pqueue'

class Site
end

class Score
  def initialize(map, num_of_punters)
    @map = map
    @num_of_punters = num_of_punters
    @rivers = {}
  end

  def update(moves)
    # sort by punter id
    moves.sort! do |a, b|
      (a["claim"] || a["pass"])["punter"] <=>
        (b["claim"] || b["pass"])["punter"]
    end
    moves.each do |k, v|
      continue if k == 'pass'
      punter_id = v["punter"]
      source = v["source"]
      target = v["target"]

      if @rivers[[source, target]].nil?
        @rivers[[source, target]] = punter_id
      end
    end
  end

  def calc
    ret = [0] * @num_of_punters

    @map["mines"].each do |mine|
    end
  end

  def shortest_path
    
  end
end
