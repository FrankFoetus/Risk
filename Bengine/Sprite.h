#pragma once
#include <string>
#include <GL\glew.h>

#include "GLTexture.h"


namespace Bengine {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float heigth, std::string texturePath);
		//GLuint red, GLuint green, GLuint blue);

		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		// vertex buffer object ID. A buffer is another word for array
		GLuint _vboID;
		GLTexture _texture;

	};
}

