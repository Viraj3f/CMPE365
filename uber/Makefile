CXX=clang++
CXXFLAGS=-std=c++11 -I.

deps = CityGraph.hpp
obj=uber.o CityGraph.o
src=$(obj:.o=.cpp)

uber: uber.o CityGraph.o
	clang++ -std=c++14 -o uber uber.o CityGraph.o

uber.o: uber.cpp CityGraph.hpp
	clang++ -std=c++14 -c uber.cpp

CityGraph.o: CityGraph.cpp CityGraph.hpp
	clang++ -std=c++14 -c CityGraph.cpp

clean:
	rm *.o *.d *.out
