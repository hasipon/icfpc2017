# coding: utf-8
require 'pqueue'
require 'set'

class Site
  def initialize(site_id, cost)
    @site_id = site_id
    @cost = cost
  end

  attr_reader :site_id, :cost
end

class UndirectedGraph
  def initialize
    @path = {}
  end

  def add_edge(source, target)
    @path[source] = [] if @path[source].nil?
    @path[target] = [] if @path[target].nil?

    @path[source].push target
    @path[target].push source
  end

  def load_paths(paths)
    # [{"source" => 1, "target" => 2}, ...] の形式を読み込む
    paths.each do |path|
      add_edge(path["source"], path["target"])
    end
  end

  def accessible(source)
    ret = Set.new
    q = [source]

    while !q.empty?
      s = q.shift
      ret.add(s)
      next if @path[s].nil?
      @path[s].each do |v|
        next if ret.member? v
        q.push v
      end
    end

    ret
  end

  attr_reader :path
end

class Score
  def initialize(map, num_of_punters, futures, splurges)
    @map = map
    @num_of_punters = num_of_punters

    @futures = {}
    futures.each do |punter_id, future|
      @futures[punter_id] = UndirectedGraph.new
      @futures[punter_id].load_paths future
    end

    @rivers = {}

    @graph = UndirectedGraph.new
    @graph.load_paths @map["rivers"]

    # @map["rivers"].each do |river|
    #   @graph.add_edge(river["source"], river["target"])
    # end

    @claims = []
    @num_of_punters.times do
      @claims.push(UndirectedGraph.new)
    end

    @splurges = splurges
    @pass_counts = Array.new(@num_of_punters, 0)
  end

  def handle_splurge(move)
    unless @splurges
      $stderr.puts "splurges is not enabled"
      return false
    end

    punter_id = move["splurge"]["punter"]
    route = move["splurge"]["route"]

    unless @pass_counts[punter_id] >= route.length - 2
      $stderr.puts "pass count is insufficient (pass count = #{@pass_counts[punter_id]}, route = #{route})"
      return false
    end

    route.each_cons(2) do |source, target|
      if @rivers[[source, target]]
        $stderr.puts "already occupied (#{source},#{target})"
        return false
      end
    end

    route.each_cons(2) do |source, target|
      @rivers[[source, target]] = punter_id
      @claims[punter_id].add_edge(source, target)
    end

    true
  end

  def update(move)
    if move.has_key? 'pass'
      # increment pass count for splurge
      @pass_counts[move["pass"]["punter"]] += 1
    elsif move.has_key? 'claim'
      punter_id = move["claim"]["punter"]
      source = move["claim"]["source"]
      target = move["claim"]["target"]

      if @rivers[[source, target]].nil?
        @rivers[[source, target]] = punter_id
        @claims[punter_id].add_edge(source, target)
      end

      @pass_counts[punter_id] = 0
    elsif move.has_key? 'splurge'
      unless handle_splurge(move)
        $stderr.puts "WARNING: invalid splurge :#{move}"
      end
    else
      raise "not supported move: #{move}"
    end
  end

  def calc_for_mine(mine)
    # calculate shotest paths
    costs = {} # costs for each site
    @map["sites"].each do |site|
      costs[site["id"]] = -1
    end
    pq = PQueue.new([Site.new(mine, 0)]) { |a, b| a.cost < b.cost }
    while !pq.empty?
      site = pq.pop
      next if costs[site.site_id] != -1
      costs[site.site_id] = site.cost
      @graph.path[site.site_id].each do |site_id|
        next if costs[site_id] != -1
        pq.push(Site.new(site_id, site.cost + 1))
      end
    end

    # calculate score
    ret = []
    @num_of_punters.times do |punter_id|
      score = 0
      acc = @claims[punter_id].accessible(mine)
      acc.each do |site_id|
        score += costs[site_id] ** 2
      end

      # future
      if @futures[punter_id] && @futures[punter_id].path[mine]
        site_id = @futures[punter_id].path[mine].first
        if acc.include? site_id
          score += costs[site_id] ** 3
        else
          score -= costs[site_id] ** 3
        end
      end

      ret.push(score)
    end
    ret
  end

  def calc
    scores = [0] * @num_of_punters
    @map["mines"].each do |mine|
      s = self.calc_for_mine(mine)
      s.each_with_index do |v, idx|
        scores[idx] += v
      end
    end
    ret = []
    scores.each_with_index do |v, idx|
      ret.push({"punter" => idx, "score" => v})
    end
    ret
  end
end
