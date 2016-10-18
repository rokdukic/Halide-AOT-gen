#include "stdafx.h"
#include "Halide.h"
using namespace Halide;

void createAOTO(char* path) {
	Var x, y;
	Func limit, erosion, dilation, limitE;

	ImageParam input(type_of<uint8_t>(), 2);
	Param<int> dimension;

	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);
	limit = BoundaryConditions::constant_exterior(input,255);

	//erosion(x, y) = argmin(limit(x + r.x, y + r.y), "erosion")[2];

//	limitE = BoundaryConditions::constant_exterior(erosion, 0);

	//dilation(x, y) = argmax(erosion(x + r.x, y + r.y), "dilation")[2];

	erosion(x, y) = minimum(limit(x + r.x, y + r.y), "erosion");
	dilation(x, y) = maximum(erosion(x + r.x, y + r.y), "dilation");

	erosion.compute_root();
	dilation.vectorize(x, 4).parallel(y);
	
	Target target = get_host_target();

	target.set_feature(Target::NoRuntime, true);

	dilation.compile_to_static_library(path, { input,dimension }, target);

}