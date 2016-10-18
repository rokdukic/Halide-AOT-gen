#include "stdafx.h"
#include "Halide.h"
using namespace Halide;

void createAOTC(char* path) {
	Var x, y;
	Func limit, erosion, dilation;

	ImageParam input(type_of<uint8_t>(), 2);
	Param<int> dimension;

	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);
	limit = BoundaryConditions::repeat_edge(input);

/*	erosion(x, y) = limit(x, y);
	erosion(x, y) = min(limit(x + r.x, y + r.y), erosion(x, y));*/

	erosion(x, y) = argmin(r, limit(x + r.x, y + r.y), "erosion")[2];

	

	dilation(x, y) = argmax(r, erosion(x + r.x, y + r.y), "dilation")[2];
	
	
	erosion.compute_root().store_root();
	dilation.vectorize(x, 4).parallel(y);

	Halide::Target t = get_host_target();

	t.set_feature(Halide::Target::NoRuntime, true);


	dilation.compile_to_static_library(path, { input,dimension },t);

	printf("closion done");
}