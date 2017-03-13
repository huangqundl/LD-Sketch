test_ldsketch: main.cpp dyn_tbl.cpp dyn_tbl.hpp LDSketch.cpp LDSketch.hpp
	g++ -Wall -o test_ldsketch main.cpp dyn_tbl.cpp LDSketch.cpp

clean:
	rm -rf test_ldsketch
