#include "Sprite.h"
#include "Vertex.h"
#include "RessourceManager.h"

#include <cstddef>

namespace Bengine {

	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		// GLuint red, GLuint green, GLuint blue) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = RessourceManager::getTexture(texturePath);

		if (_vboID == 0) {
			glGenBuffers(1, &_vboID);
		}

		Vertex vertexData[6];

		// first triangle
		vertexData[0].setPosition(_x + _width, _y);
		vertexData[0].setUV(1.0f, 0.0f);
		vertexData[1].setPosition(_x, _y);
		vertexData[1].setUV(0.0f, 0.0f);
		vertexData[2].setPosition(_x, _y + _height);
		vertexData[2].setUV(0.0f, 1.0f);

		// second triangle
		vertexData[3].setPosition(_x + _width, _y);
		vertexData[3].setUV(1.0f, 0.0f);
		vertexData[4].setPosition(_x + _width, _y + _height);
		vertexData[4].setUV(1.0f, 1.0f);
		vertexData[5].setPosition(_x, _y + _height);
		vertexData[5].setUV(0.0f, 1.0f);

		// set the color
		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 0, 255);
		}
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void Sprite::draw() {

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		// bind the buffer object
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		// tell opengl that we want to use the first
		// attribute array.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// this is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// this is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// this is the uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// draw the 6 vertices to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// disable the vertex attrib array. this is not optional
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
