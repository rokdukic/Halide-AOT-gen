
#include "stdafx.h"
//#include "Halide.h"
using namespace Halide;


void createAOTE() {
	Var x, y;
	Func erosion, limit;

	Param<int> dimension;
	ImageParam input(type_of<uint8_t>(), 2);

	//limit = BoundaryConditions::repeat_edge(input);
	limit = BoundaryConditions::constant_exterior(input, 255);

	RDom r(-1 * dimension / 2, dimension, -1 * dimension / 2, dimension);

	/*erosion(x, y) = limit(x, y);
	//erosion(x,y) = BoundaryConditions::repeat_edge(erosion(x,y));
	erosion(x, y) = min(limit(x + r.x, y + r.y), erosion(x, y));*/


	erosion(x, y) = argmin(r, limit(x + r.x, y + r.y))[2];




	erosion.vectorize(x, 4).parallel(y);
	//erosion.update(0).vectorize(x, 4).parallel(y);
	//erosion.update(1).vectorize(x, 4).parallel(y);
	/*Target target = get_host_target();

	target.set_feature(Target::NoRuntime, true);*/

	

	//erosion.compile_to_file("erosion", { input, dimension }, target);
	erosion.compile_to_static_library("erosion", { input, dimension });
}