#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "Constants.h"

namespace Drawings {

	/* Note: Drawings of the same object must have the same first vertice in order
	to scalate and rotate properly without additional translation in update
	function. */
	Mesh* createShuriken	 (std::string name, int starPoints);
	Mesh* createUglyShuriken (std::string name);

	Mesh* createBalloon		 (std::string name);
	// Create balloon bottom string.
	Mesh* createBalloonStr	 (std::string name);

	Mesh* createArrowTip	 (std::string name);
	Mesh* createArrowBody	 (std::string name);
	Mesh* createArrowFeathers(std::string name);

	// For animation meshes.
	Mesh* createBowBody(std::string name,
						glm::vec2 bowStringFactor = glm::vec2(1.0f, Constants::bowFactor),
						float radius = Constants::bowRadius);

	Mesh* createBowString(std::string name,
							glm::vec2 bowStringFactor = glm::vec2(0.0f, Constants::bowFactor),
							float radius = Constants::bowRadius);

	Mesh* createSquare(std::string name, bool fill);
}

