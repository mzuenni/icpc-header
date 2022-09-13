#include <bits/stdc++.h>
#include "../src/visualizer.h"

int main(int argc, char** argv) {
	Visualizer::init(argc, argv);			// open files
	using namespace Visualizer;

	Integer n, m;
	testIn >> n >> m;
	Polygon p;								// create an empty polygon
	p.stroke = Stroke(BLACK, 0.1);			// set stroke
	for (Integer i = 0; i < n; i++) {
		Real x, y;
		testIn >> x >> y;					// read points from testfile
		p.points.emplace_back(x, y);		// and add them to to polyline
	}
	image << p << std::endl;				// write polygon to SVG image

	Polyline res;							// create an empty polyline
	res.stroke = Stroke(RED, 0.1);			// set stroke
	for (Integer i = 0; i < m; i++) {
		Real x, y;
		testAns >> x >> y;					// read points from asnwer
		res.points.emplace_back(x, y);		// and add them to the polyline
	}
	image << res << std::endl;				// write polygon to SVG image

	image.origin -= {3, 3};					// enlarge BoundingBox by 3 in all directions
	image.dimension += {6, 6};				// (the BB is updated whenever any object is added to the SVG)
}
