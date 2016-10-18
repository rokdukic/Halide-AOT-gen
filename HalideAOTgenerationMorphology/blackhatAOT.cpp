#include "stdafx.h"

using namespace Halide;

void createAOTB(char* path) {
	Var x, y, x_outer, x_inner, y_outer, y_inner, tile_index;
	Func dilation, erosion, blackhat, limit;
	ImageParam input(type_of<uint8_t>(), 2);
	Param<int> dimension;
	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);
	limit = BoundaryConditions::repeat_edge(input);

/*	dilation(x, y) = limit(x, y);
	dilation(x, y) = max(limit(x + r.x, y + r.y), limit(x, y));
	erosion(x, y) = dilation(x, y);
	erosion(x, y) = min(dilation(x + r.x, y + r.y), dilation(x, y));*/
	
	dilation(x, y) = argmax(r, limit(x + r.x, y + r.y), "dilation")[2];
	erosion(x, y) = argmin(r, dilation(x + r.x, y + r.y), "erosion")[2];


	dilation.compute_at(erosion, x);
	erosion.vectorize(x, 4).parallel(y);
	erosion.compute_root().store_root();

	blackhat(x, y) = erosion(x, y) - input(x, y);
	
	blackhat.tile(x, y, x_outer, y_outer, x_inner, y_inner, 20,20).fuse(x_outer, y_outer, tile_index).parallel(tile_index);

	Halide::Target t = get_host_target();

	t.set_feature(Halide::Target::NoRuntime, true);

	blackhat.compile_to_static_library(path, { input,dimension },t);
}