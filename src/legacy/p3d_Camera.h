#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace p3d {
	class Camera {
	public:
		Camera(glm::vec3 pos);
		~Camera();

		glm::mat4x4 getView() const;
		glm::vec3 getEye() const;

		void move(float fwd, float right);
		void rotate(float pitchRad, float yawRad);

	private:
		void update();

		glm::vec3 _pos;
		float _pitch;
		float _yaw;

		glm::mat4x4 _view;
	};
}