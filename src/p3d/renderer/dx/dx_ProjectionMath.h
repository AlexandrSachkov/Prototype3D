#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/fast_trigonometry.hpp"

namespace p3d {
	namespace math {
		glm::mat4x4 perspectiveDX(float fovy, float aspect, float zNear, float zFar) {
			assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0);

			float yScale = 1.0f / glm::tan(fovy * 0.5f);
			float xScale = yScale / aspect;
			float zFDivDelta = zFar / (zNear - zFar);

			glm::mat4x4 m(0);
			m[0][0] = xScale;
			m[1][1] = yScale;
			m[2][2] = zFDivDelta;
			m[2][3] = -1.0f;
			m[3][2] = zNear * zFDivDelta;

			return m;
		}

		glm::mat4x4 orthographicDX(float width, float height, float zNear, float zFar) {
			glm::mat4x4 m(0);
			m[0][0] = 2.0f / width;
			m[1][1] = 2.0f / height;
			m[2][2] = 1.0f / (zNear - zFar);
			m[2][3] = zNear / (zNear - zFar);
			m[3][3] = 1.0f;

			return m;
		}
	}
}