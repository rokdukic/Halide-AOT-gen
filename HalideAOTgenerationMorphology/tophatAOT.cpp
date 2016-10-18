#include "stdafx.h"
using namespace Halide;

void createAOTT(char* path) {
	Var x, y, x_outer, x_inner, y_outer, y_inner, tile_index;
	Func erosion, dilation, tophat, limit;
	ImageParam input(type_of<uint8_t>(), 2);
	Param<int> dimension;
	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);
	limit = BoundaryConditions::repeat_edge(input);

	/*erosion(x, y) = limit(x, y);
	erosion(x, y) = min(limit(x + r.x, y + r.y), erosion(x, y));*/

	erosion(x, y) = argmin(r, limit(x + r.x, y + r.y), "erosion")[2];



	/*dilation(x, y) = erosion(x, y);
	dilation(x, y) = max(erosion(x + r.x, y + r.y), erosion(x, y));*/

	dilation(x, y) = argmax(r, erosion(x + r.x, y + r.y), "dilation")[2];

	erosion.compute_root();
	dilation.vectorize(x, 4).parallel(y);
//	dilation.update(0).vectorize(x, 4).parallel(y);
	dilation.compute_root().store_root();


	tophat(x, y) = input(x, y) - dilation(x, y);

	tophat.tile(x, y, x_outer, y_outer, x_inner, y_inner, 20, 20).fuse(x_outer, y_outer, tile_index).parallel(tile_index);
	//tophat.vectorize(x, 20).parallel(y);

	Halide::Target t = get_host_target();

	t.set_feature(Halide::Target::NoRuntime, true);

	tophat.compile_to_static_library("tophat", { input,dimension },t);
}
