CXX:= clang++-3.8 
SRCS:= test.cpp ../AIs/shiotaAI/AI.cpp
OBJS := $(SRCS:.cpp=.o)

test: $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f test $(OBJS)
