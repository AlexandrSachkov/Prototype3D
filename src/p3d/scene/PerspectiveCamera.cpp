#include "PerspectiveCamera.h"
#include "glm/gtx/quaternion.hpp"

namespace p3d {
    PerspectiveCamera::PerspectiveCamera(glm::vec3 pos, float fovy, float aspectRatio, float zNear, float zFar):
		_pos(pos), _pitch(0), _yaw(0)
	{
        _projection = glm::perspective(fovy, aspectRatio, zNear, zFar);
		update();
	}

    PerspectiveCamera::~PerspectiveCamera() {}

	glm::mat4x4 PerspectiveCamera::getView() const {
		return _view;
	}

    glm::mat4x4 PerspectiveCamera::getProjection() const {
        return _projection;
    }

	glm::vec3 PerspectiveCamera::getEye() const {
		return _pos;
	}

	void PerspectiveCamera::move(float fwdDist, float rightDist) {
		glm::vec3 forward(_view[0][2], _view[1][2], _view[2][2]);
		glm::vec3 strafe(_view[0][0], _view[1][0], _view[2][0]);
		_pos += fwdDist * forward - rightDist * strafe;

		update();
	}

	void PerspectiveCamera::rotate(float pitchRad, float yawRad) {
		_pitch += pitchRad;
		_yaw += yawRad;
		update();
	}

	void PerspectiveCamera::update() {
		glm::quat qPitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));

		glm::quat orientation = qPitch * qYaw;
		orientation = glm::normalize(orientation);
		glm::mat4 rotate = glm::mat4_cast(orientation);

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), _pos);
		_view = translate * rotate;
	}
}