#pragma once

#include <GL\glew.h>


namespace Bengine {

	// this is the position struct.
	struct Position {
		float x;
		float y;
	};

	// 4 byte color struct for r, g, b, alpha(transparency)
	struct ColorRGBA8 {
		ColorRGBA8() : r(0), g(0), b(0), a(a) 
		{
		}

		ColorRGBA8(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) :
			r(red), g(green), b(blue), a(alpha)
		{
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {
		// when you store a struct or class
		// inside of another struct or class, it is called composition. this is
		// layed out exactly the same in memory as if we had a float position[2],
		// but doing it this way is more intuitive
		Position position;

		ColorRGBA8 color;

		// uv texture coordinates
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}
	};

}