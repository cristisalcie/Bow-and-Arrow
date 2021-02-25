#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Constants {

	const int cameraSpeed{ 500 };

	/* One center and color for all of them because i do not intend to
	fiddle with them here. */
	const glm::vec3 center{ glm::vec3(0, 0, 0) };
	const glm::vec3 color{ glm::vec3(1, 1, 1) };

	const float shurikenSmallTriangleHeight{ 8 };
	const float shurikenHoleRadius{ shurikenSmallTriangleHeight / 2 };
	const float shurikenRadius{ 30.0f };

	const float balloonRadius{ 20.0f };
	const float balloonEllipseFactor{ 1.3f };

	const float balloonTriangleHeight{ balloonRadius / 4 };
	const float balloonTriangleBotSideLength{ balloonRadius / 4 };

	const float balloonStringSpace{ balloonTriangleHeight / 10 };
	const float balloonDeflateSpeed1 { 0.500f };
	const float balloonDeflateSpeed2 { 0.350f };
	const float balloonDeflateSpeed3 { 0.100f };
	const float balloonDeflateSpeed4 { 0.020f };

	const float balloonFloatingSpeed{ 30.0f };
	
	const float arrowTipHeight{ 18.0f };
	const float arrowTipRadiusToCornerAngle{ 60.0f };
	const float arrowTipHoleRadius{ 6 };
	const float arrowTipBodySideLength
	{
		2 * arrowTipHoleRadius * glm::sin(glm::radians(arrowTipRadiusToCornerAngle))
	};
	const float arrowTipCenterToCornerOnOx
	{
		arrowTipHoleRadius * (1 - glm::cos(glm::radians(arrowTipRadiusToCornerAngle)))
	};

	const float arrowBodyLength{ 4 * arrowTipHeight };
	const float arrowBodyWeightCenter{ arrowBodyLength * 3 / 4 };

	const float powerBarLengthOx{ 50.0f };
	const float powerBarWidthOy{ 10.0f };

	const float bowRadius{ 36.0f };
	const float bowFactor{ 1.6f };

	const glm::vec2 initialBowPos{ glm::vec2(50.0f, 300.0f) };
	const float bowInitialAngle{ 0.0f };
	const int bowLifes{ 3 };
	const float bowCooldownLimit{ 3 }; // seconds
	const float bowReloadCooldownLimit{ 1 }; // seconds
	const float arrowLifeTime{ 4 };	 // seconds

	const float bowStretchTime{ 1.5f };	 // seconds
	const glm::vec2 bowDeformationFactor{ glm::vec2(5, 10) };
	const float arrowSpeedFactor{ 5 };
	const float bowMaxAngle{ 86.0f };

	const float groundWidth{ 10.0f };
	const float leftGroundEnd{ 24 * balloonRadius };
	const float exitHoleEnd{ leftGroundEnd + 36 * balloonRadius };

	// Width is on Oy.
	const float wallWidth{ 50.0f };

	// Length is on Ox.
	const float wallLength{ 10.0f };

	const int levelsCount{ 2 };

	const float gravity{ 0.005f };

	const float shurikenPoints{ 3 };
	const float balloonRedPoints { 7 };
	const float balloonYellowPoints { -4 };

	const glm::vec3 redBalloonColor{ glm::vec3(0.9f, 0.5f, 0.2f) };
	const glm::vec3 yellowBalloonColor{ glm::vec3(0.9f, 0.9f, 0) };

	// Length is on Ox.
	const float obstacleLengthOx{ 12.0f };

	// Width is on Oy.
	const float obstacleWidthOy{ 75.0f };

	const float level1Duration{ 12.0f };
	const float level2Duration{ 12.0f };
}

