#include "stdafx.h"
//#include "Halide.h"

using namespace Halide;

void createAOTD(char* path) {

	Var x, y;
	Func dilate, limit;
	Param<int> dimension;

	
	ImageParam input(type_of<uint8_t>(), 2);
	limit = BoundaryConditions::constant_exterior(input, 0);
	
	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);

/*	dilate(x,y) = limit(x, y);
	dilate(x, y) = max(limit(x + r.x, y + r.y), dilate(x, y));*/

	dilate(x, y) = argmax(r, limit(x + r.x, y + r.y))[2];

	dilate.vectorize(x, 4).parallel(y);
	//dilate.update(0).vectorize(x, 4).parallel(y);

	Halide::Target t = get_host_target();

	t.set_feature(Halide::Target::NoRuntime, true);


	dilate.compile_to_static_library(path, { input,dimension }, t);
}