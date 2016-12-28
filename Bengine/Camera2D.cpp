#include "Camera2D.h"

namespace Bengine {

	Camera2D::Camera2D() : 
		_position(0, 0),
		_cameraMatrix(0),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(1000),
		_screenHeight(1000)
	{
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}


	void Camera2D::update() {
		if (_needsMatrixUpdate) {

			// translate camera2D
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			// scaling
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			// camera doesnt need to be updated anymore
			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		// invvert y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		// scale the coordinates
		screenCoords /= _scale;
		// translate with the camera
		screenCoords += _position;
		
		return screenCoords;
	}


	bool Camera2D::inBoundaryWidth() {
		if (_position.x < _screenWidth/2 && _position.x > -_screenWidth/2) {
			return true;
		}
		return false;
	}

	bool Camera2D::inBoundaryHeight() {
		if (_position.y < 1000 && _position.y > -1000) {
			return true;
		}
		return false;
	}

	bool Camera2D::isBoxInView(const glm::vec2 &position, const glm::vec2 &dimension) {

		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;

		const float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		glm::vec2 centerPos = position + dimension / 2.0f;
		glm::vec2 centerCameraPos = _position;

		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {

			return true;
		}
		return false;
	}
}


