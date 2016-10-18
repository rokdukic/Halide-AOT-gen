#include "stdafx.h"

using namespace Halide;

void createAOTG(char* path) {
	Var x, y, x_outer, x_inner, y_outer, y_inner,tile_index;
	Func limit, d, e, g;
	ImageParam input(type_of<uint8_t>(), 2);
	Param<int> dimension;

	limit = BoundaryConditions::repeat_edge(input);
	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);

	d(x, y) = argmax(r, limit(x + r.x, y + r.y), "dilation")[2];
	e(x, y) = argmin(r, limit(x + r.x, y + r.y), "erosion")[2];

	d.vectorize(x, 4).parallel(y);
	e.vectorize(x, 4).parallel(y);

	d.store_root().compute_root(); e.store_root().compute_root();

	g(x, y) = d(x, y) - e(x, y);

	//g.tile(x, y, x_outer, y_outer, x_inner, y_inner, 64, 64).fuse(x_outer, y_outer, tile_index).parallel(tile_index);
	g.vectorize(x, 4).parallel(y);

	Halide::Target t = get_host_target();

	t.set_feature(Halide::Target::NoRuntime, true);

	g.compile_to_static_library(path, { input,dimension }, t);
}