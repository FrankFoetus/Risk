#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Vertex.h"
#include "GLTexture.h"


namespace Bengine {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph{
	public:

		Glyph() {};
		Glyph(const glm::vec4& destinationRectangle, const glm::vec4& uvRectangle, GLuint Texture, float Depth, const ColorRGBA8& color)
			: texture(Texture), depth(Depth) {

			 topLeft.color = color;
			 topLeft.setPosition(destinationRectangle.x, destinationRectangle.y + destinationRectangle.w);
			 topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

			 bottomLeft.color = color;
			 bottomLeft.setPosition(destinationRectangle.x, destinationRectangle.y);
			 bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

			 bottomRight.color = color;
			 bottomRight.setPosition(destinationRectangle.x + destinationRectangle.z, destinationRectangle.y);
			 bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);

			 topRight.color = color;
			 topRight.setPosition(destinationRectangle.x + destinationRectangle.z, destinationRectangle.y + destinationRectangle.w);
			 topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);
		}
		GLuint texture;
		float depth;
			
		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;
	};


	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
			offset(Offset), 
			numVertices(NumVertices), 
			texture(Texture) {
		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};


	class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destinationRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();

		void sortGlyphs();

		static bool compareFrontToBack(Glyph* A, Glyph* B);
		static bool compareBackToFront(Glyph* A, Glyph* B);
		static bool compareTexture(Glyph* A, Glyph* B);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph> _glyphs; /// these are the actual glyphs
		std::vector<Glyph*> _glyphsPointers; /// this is for sorting
		std::vector<RenderBatch> _renderBatches;
	};

}