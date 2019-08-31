#pragma once
#include "CameraI.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace p3d {
	class PerspectiveCamera : public CameraI {
	public:
        PerspectiveCamera(glm::vec3 pos, float fovy, float aspectRatio, float zNear, float zFar);
		~PerspectiveCamera();

		glm::mat4x4 getView() const override;
        glm::mat4x4 getProjection() const override;
		glm::vec3 getEye() const override;

		void move(float fwd, float right);
		void rotate(float pitchRad, float yawRad);

	private:
		void update();

		glm::vec3 _pos;
		float _pitch;
		float _yaw;

		glm::mat4x4 _view;
        glm::mat4x4 _projection;
	};
}