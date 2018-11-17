CXX = clang++
CXXFLAGS = -std=c++11 -g

deps = CityGraph.hpp Driver.hpp Passenger.hpp
src = uber.cpp Driver.cpp Passenger.cpp
obj = $(addprefix bin/,$(src:.cpp=.o))

$(info ${obj})

all: bin/uber.out

bin/uber.out: $(obj)
	$(CXX) $(CXXFLAGS) -o bin/uber.out $^

bin/%.o: %.cpp $(deps)
	clang++ $(CXXFLAGS) -std=c++14 -c $< -o $@

clean:
	rm bin/*.o bin/*.d bin/*.out
