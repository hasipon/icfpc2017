# https://stackoverflow.com/questions/13765839/implementing-a-synchronization-barrier-in-ruby

require 'monitor'

class Barrier
  def initialize(count)
    @mutex = Mutex.new
    @cond = ConditionVariable.new
    @max_count = count
    @count = @max_count
  end

  def sync
    @mutex.synchronize do
      @count -= 1
      if @count > 0
        @cond.wait @mutex
      else
        @cond.broadcast
        @count = @max_count
      end
    end
  end
end
