#include "LDSketch.hpp"

int main(int argc, char** argv) {

	LDSketch_t* lds = LDSketch_init(10000, 4, 1, 64, 1024*1024, 0);
	LDSketch_destroy(lds);
	return 0;
}