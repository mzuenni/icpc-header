#include <bits/stdc++.h>
#include "../src/visualizer_png.h"
using namespace Visualizer;												// we are writing a visulaizer

int main(int argc, char** argv) {
	Visualizer::init(argc, argv);										// open files

	Integer n, w, h;
	testIn >> n >> w >> h;

	image.clear(w+2, h+2, WHITE);										// set width and height of mage (and make everythin WHITE)
	
	for (Integer i = 0; i < n; i++) {
		Integer x1, y1, x2, y2;
		testIn >> x1 >> y1 >> x2 >> y2;
		image.drawLine({x1+1,y1+1}, {x2+1,y2+1}, BLACK);				//draw a line
	}

	image.floodFill({0,0}, TRANSPARENT);								//clear everything connected to the border

	image.fillRectangle<BLEND>({0,0}, {w+1,h+1}, Color(0xFF00FF, 0x80));// draw with alpha
}
