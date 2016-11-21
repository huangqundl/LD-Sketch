test_ldsketch: main.cpp dyn_tbl.cpp LDSketch.cpp
	g++ -Wall -o test_ldsketch main.cpp dyn_tbl.cpp LDSketch.cpp

clean:
	rm -rf test_ldsketch