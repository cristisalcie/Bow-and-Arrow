#pragma once

#include <include/glm.h>

namespace Transform2D {
	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY) {
		return glm::mat3(
			1,			0,			 0,
			0,			1,			 0,
			translateX, translateY,  1
		);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY) {
		return glm::mat3(
			scaleX,   0,        0,
			0,        scaleY,   0,
			0,        0,        1
		);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians) {
		float cosU{ glm::cos(radians) };
		float sinU{ glm::sin(radians) };


		return glm::mat3(
			cosU,    -sinU,        0,
			sinU,     cosU,        0,
			0,        0,           1
		);
	}
}
