CXX=g++
CXXFLAGS += -O2 -Wall -std=c++11

all: clean eval

eval_src=eval.cpp

eval:
	$(CXX) $(CXXFLAGS) -o $@ $(eval_src)

clean:
	rm -rf *.o
	rm -rf eval 
