#include "Drawings.h"

#include <Core/Engine.h>



Mesh* Drawings::createShuriken(std::string name, int starPoints) {
	glm::vec3 color{			Constants::color };
	glm::vec3 center{			Constants::center };
	float radius{				Constants::shurikenRadius };
	float holeRadius{			Constants::shurikenHoleRadius };
	float smallTriangleHeight{	Constants::shurikenSmallTriangleHeight };

	int i;
	int crtStar;
	int startIndice;
	float angle = 0;
	int circlePoints = 60;
	float circleAngleOffset = 360.0f / circlePoints;
	//int starPoints = 3; // 7, 11, 13, 14 are problematic. Tested untill 15.
	float starOffset = 360.0f / starPoints;
	std::vector<VertexFormat> vertices{ VertexFormat(center, color) };

	// Create circle circlePoints
	for (i = 1; i <= circlePoints; ++i) {
		float x = center.x + glm::cos(glm::radians(angle)) * holeRadius;
		float y = center.y + glm::sin(glm::radians(angle)) * holeRadius;
		float z = 0;
		angle += circleAngleOffset;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}
	/* From indice 0 (center of shuriken) to indice 720 inclusive, are circle
	vertexes. */

	// Create triangle points
	angle = 0;
	for (i = 0; i < starPoints; ++i) {
		float x = center.x + glm::cos(glm::radians(angle)) * smallTriangleHeight;
		float y = center.y + glm::sin(glm::radians(angle)) * smallTriangleHeight;
		float z = 0;
		angle += starOffset;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}
	/* From indice circlePoints + 1 to circlePoints + starPoints inclusive
	we have the star tip points. */


	for (angle = starOffset / 2; angle < 360; angle += starOffset) {
		float x = center.x + glm::cos(glm::radians(angle)) * radius;
		float y = center.y + glm::sin(glm::radians(angle)) * radius;
		float z = 0;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}
	/* From indice (circlePoints + starPoints + 1) to (circlePoints + starPoints
	+ starPoints) inclusive we have the star tip points. */



	Mesh *shuriken = new Mesh(name);

	std::vector<unsigned short> indices;

	// Add the hole indices
	/* crtStar is always situated at 0 degrees no matter how many circle points
	or starPoints there are. */
	crtStar = circlePoints + 1; // 19
	// startIndice will be the id of the start circle vertex.
	startIndice = crtStar - (starOffset / 2) / (360 / circlePoints); // 16
	for (int j = 0; j < starPoints; ++j) {
		for (i = 0; i < circlePoints / starPoints; ++i) {
			if (startIndice > circlePoints) {
				startIndice = 1;
			}
			if (startIndice + 1 > circlePoints) {
				indices.push_back(1);
			} else {
				indices.push_back(startIndice + 1);
			}
			indices.push_back(crtStar);
			indices.push_back(startIndice);


			++startIndice;
		}
		++crtStar;
	}

	// Add the final spikes indices

	// Will never overflow. ( > circlePoints)
	int onGapIndex = 1 + (starOffset / 2) / (360 / circlePoints); //16

	crtStar = circlePoints + starPoints + 1; // 22
	// startIndice will be first triangle star vertex next to the hole
	startIndice = crtStar - starPoints; // 19
	int initialStartIndice = crtStar - starPoints;
	for (int j = 0; j < starPoints; ++j) {
		indices.push_back(startIndice);
		indices.push_back(crtStar);
		indices.push_back(onGapIndex);

		indices.push_back(crtStar);
		if (startIndice + 1 > circlePoints + starPoints) {
			indices.push_back(initialStartIndice); // 19
		} else {
			indices.push_back(startIndice + 1);
		}
		indices.push_back(onGapIndex);


		++startIndice;
		++crtStar;
		onGapIndex += starOffset / (360 / circlePoints);
	}



	shuriken->SetDrawMode(GL_TRIANGLES);

	shuriken->InitFromData(vertices, indices);
	return shuriken;
}


Mesh* Drawings::createUglyShuriken(std::string name) {
	glm::vec3 color{	Constants::color };
	glm::vec3 center{	Constants::center };
	float radius{		Constants::shurikenRadius };

	int i;
	float angle = 0;
	int points = 8;
	float angleOffset = 360.0f / points;
	std::vector<VertexFormat> vertices{ VertexFormat(center, color) };

	for (i = 1; i <= points; ++i) {
		float x = center.x + glm::cos(glm::radians(angle)) * radius;
		float y = center.y + glm::sin(glm::radians(angle)) * radius;
		float z = 0;
		angle += angleOffset;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}

	Mesh *shuriken = new Mesh(name);

	std::vector<unsigned short> indices;

	for (i = 0; i <= points; i += 2) {
		indices.push_back(i + 2);
		indices.push_back(center.x); 
		indices.push_back(i + 1);
	}

	shuriken->InitFromData(vertices, indices);
	return shuriken;
}


Mesh* Drawings::createBalloon(std::string name) {
	glm::vec3 color{	Constants::color };
	glm::vec3 center{	Constants::center };
	float radius{		Constants::balloonRadius };
	float factor{		Constants::balloonEllipseFactor };
	float tHeight{		Constants::balloonTriangleHeight };
	float tBotSideLen{	Constants::balloonTriangleBotSideLength };
	float stringSpace{	Constants::balloonStringSpace };

	int i;
	float angle = 0;
	int points = 60;
	float angleOffset = 360.0f / points;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(center, color)
	};

	glm::vec2 topTrianglePos;
	glm::vec2 botLeftTrianglePos;
	glm::vec2 botRightTrianglePos;

	for (i = 1; i <= points; ++i) {
		float x = center.x + glm::cos(glm::radians(angle)) * radius;
		float y = center.y + factor * glm::sin(glm::radians(angle)) * radius;
		float z = 0;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
		if (angle == 270) {
			// Make top triangle point
			topTrianglePos.x = x;
			topTrianglePos.y = y - stringSpace;

			// Make bottom left point
			botLeftTrianglePos.x = x - tBotSideLen;
			botLeftTrianglePos.y = y - tHeight;

			// Make bottom right point
			botRightTrianglePos.x = x + tBotSideLen;
			botRightTrianglePos.y = y - tHeight;
		}
		angle += angleOffset;
	}

	{
		VertexFormat v = VertexFormat(
			glm::vec3(topTrianglePos.x, topTrianglePos.y, 0), color
		);
		vertices.push_back(v);
	}

	{
		VertexFormat v = VertexFormat(
			glm::vec3(botLeftTrianglePos.x, botLeftTrianglePos.y, 0), color
		);
		vertices.push_back(v);
	}

	{
		VertexFormat v = VertexFormat(
			glm::vec3(botRightTrianglePos.x, botRightTrianglePos.y, 0), color
		);
		vertices.push_back(v);
	}

	Mesh *balloon = new Mesh(name);

	std::vector<unsigned short> indices;

	for (i = 0; i < points - 1; ++i) {
		indices.push_back(i + 2);
		indices.push_back(center.x); 
		indices.push_back(i + 1);
	}

	// Finish balloon
	indices.push_back(1);
	indices.push_back(center.x);
	indices.push_back(i + 1);

	// Add last triangle
	i += 2;
	indices.push_back(i);
	indices.push_back(i + 1);
	indices.push_back(i + 2);

	balloon->SetDrawMode(GL_TRIANGLES);

	balloon->InitFromData(vertices, indices);
	return balloon;
}


Mesh* Drawings::createBalloonStr(std::string name) {
	glm::vec3 color{ Constants::color };

	glm::vec3 leftTopCorner{ glm::vec3(0, 0, 0) };
	float space{ Constants::balloonStringSpace };
	float oneLineLength{ 10 };

	int i;
	int points = 6;
	std::vector<VertexFormat> vertices
	{
		VertexFormat(leftTopCorner, color),
		VertexFormat(glm::vec3(0, -Constants::balloonTriangleHeight, 0), color) 
	};

	float x_min = leftTopCorner.x - oneLineLength / 5;
	float x_max = leftTopCorner.x + oneLineLength / 5;
	float y = leftTopCorner.y;
	for (i = 1; i < points - 1; ++i) {
		y -= oneLineLength / 2;
		float z = 0;
		if (i % 2 != 0) {
			VertexFormat v = VertexFormat(glm::vec3(x_min, y, z), color);
			vertices.push_back(v);
		} else {
			VertexFormat v = VertexFormat(glm::vec3(x_max, y, z), color);
			vertices.push_back(v);
		}
	}

	Mesh *balloonString = new Mesh(name);

	std::vector<unsigned short> indices;

	for (i = 0; i < points; ++i) {
		indices.push_back(i);
	}

	balloonString->SetDrawMode(GL_LINE_STRIP);

	balloonString->InitFromData(vertices, indices);
	return balloonString;
}


Mesh* Drawings::createArrowTip(std::string name) {
	glm::vec3 color{	Constants::color };
	glm::vec3 center{	Constants::center };
	float holeRadius{	Constants::arrowTipHoleRadius };
	float tHeight{		Constants::arrowTipHeight };
	float radiusToCornerAngle{ Constants::arrowTipRadiusToCornerAngle };

	int i;
	float angle = 0;
	int circlePoints = 30;
	float circleAngleOffset = 360.0f / circlePoints;
	std::vector<VertexFormat> vertices{ VertexFormat(center, color) };

	// Create circle circlePoints
	for (i = 1; i <= circlePoints; ++i) {
		float x = center.x + glm::cos(glm::radians(angle)) * holeRadius;
		float y = center.y + glm::sin(glm::radians(angle)) * holeRadius;
		float z = 0;
		angle += circleAngleOffset;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}
	/* From indice 0 (center of hole) to indice circlePoints inclusive, are circle
	vertexes. */

	// Create arrow tip triangle point
	{
		float x = center.x + glm::cos(glm::radians(angle)) * tHeight;
		float y = center.y + glm::sin(glm::radians(angle)) * tHeight;
		float z = 0;
		angle += radiusToCornerAngle * 2;
		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
	}
	// At indice circlePoints + 1 we have arrow tip.

	Mesh *arrowTip = new Mesh(name);

	std::vector<unsigned short> indices;

	// crtStar is always situated at 0 degrees
	int tip = circlePoints + 1;
	// startIndice will be the id of the start circle vertex.
	int startIndice;
	startIndice = tip - radiusToCornerAngle / (360 / circlePoints);
	for (i = 0; i < circlePoints / (180 / radiusToCornerAngle); ++i) {
		if (startIndice > circlePoints) {
			startIndice = 1;
		}
		if (startIndice + 1 > circlePoints) {
			indices.push_back(1);
		} else {
			indices.push_back(startIndice + 1);
		}
		indices.push_back(tip);
		indices.push_back(startIndice);
		++startIndice;
	}

	arrowTip->SetDrawMode(GL_TRIANGLES);

	arrowTip->InitFromData(vertices, indices);
	return arrowTip;
}


Mesh* Drawings::createArrowBody(std::string name) {
	glm::vec3 color{ Constants::color };
	glm::vec3 center{ Constants::center };
	float length{ Constants::arrowBodyLength };
	float weightCenter{ Constants::arrowBodyWeightCenter };

	std::vector<VertexFormat> vertices;

	{
		// Weight point of the arrow
		float x, y;
		x = center.x;
		y = center.y;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	{
		// Left point of the arrow body at the tail of the arrow.
		float x, y;
		x = center.x - weightCenter;
		y = center.y;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	{
		// Right point of the arrow body at the head of the arrow.
		float x, y;
		x = center.x + (length - weightCenter);
		y = center.y;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	Mesh *arrowBody = new Mesh(name);

	std::vector<unsigned short> indices{ 1, 0, 2 };

	arrowBody->SetDrawMode(GL_LINE_STRIP);

	arrowBody->InitFromData(vertices, indices);
	return arrowBody;
}


Mesh* Drawings::createArrowFeathers(std::string name) {
	glm::vec3 color{ Constants::color };
	glm::vec3 center{ Constants::center };
	float tipHeight{ Constants::arrowTipHeight };
	float tipSideLength{ Constants::arrowTipBodySideLength };

	std::vector<VertexFormat> vertices;

	{
		// Center
		float x, y;
		x = center.x;
		y = center.y;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	{
		// Up point
		float x, y;
		x = center.x - tipHeight / 2;
		y = center.y + tipSideLength / 1.5f;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	{
		// Down point
		float x, y;
		x = center.x - tipHeight / 2;
		y = center.y - tipSideLength / 1.5f;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	{
		// Mid point
		float x, y;
		x = center.x - (tipHeight / 2) / 3;
		y = center.y;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
	}

	Mesh *arrowFeathers = new Mesh(name);

	std::vector<unsigned short> indices{ 0, 3, 1, 0, 3, 2 };

	//arrowFeathers->SetDrawMode(GL_LINE_STRIP);
	arrowFeathers->SetDrawMode(GL_TRIANGLES);

	arrowFeathers->InitFromData(vertices, indices);
	return arrowFeathers;
}


Mesh* Drawings::createBowBody(std::string name, glm::vec2 factor,
							  float radius) {
	glm::vec3 color{	Constants::color };
	glm::vec3 center{	Constants::center };

	int i;
	float angle = -90;
	int points = 30;
	float angleOffset = 360.0f / points;

	std::vector<VertexFormat> vertices;

	// We stop at half. We only want a semi ellipse.
	for (i = 0; i <= points / 2; ++i) {
		float x = center.x + factor.x * glm::cos(glm::radians(angle)) * radius;
		float y = center.y + factor.y * glm::sin(glm::radians(angle)) * radius;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
		angle += angleOffset;
	}

	Mesh *bowBody = new Mesh(name);

	std::vector<unsigned short> indices;

	for (i = 0; i <= points / 2; ++i) {
		indices.push_back(i);
	}

	bowBody->SetDrawMode(GL_LINE_STRIP);

	bowBody->InitFromData(vertices, indices);
	return bowBody;
}


Mesh* Drawings::createBowString(std::string name, glm::vec2 factor,
								float radius) {
	glm::vec3 color{  Constants::color };
	glm::vec3 center{ Constants::center };

	int i;
	float angle = -90;
	int points = 4;
	float angleOffset = 360.0f / points;

	std::vector<VertexFormat> vertices;

	// We stop at half. We only want a semi ellipse.
	for (i = 0; i <= points / 2; ++i) {
		float x = center.x + factor.x * glm::cos(glm::radians(angle)) * radius;
		float y = center.y + factor.y * glm::sin(glm::radians(angle)) * radius;
		VertexFormat v = VertexFormat(glm::vec3(x, y, 0), color);
		vertices.push_back(v);
		angle += angleOffset;
	}

	Mesh *bowBody = new Mesh(name);

	std::vector<unsigned short> indices;

	for (i = 0; i <= points / 2; ++i) {
		indices.push_back(i);
	}

	bowBody->SetDrawMode(GL_LINE_STRIP);

	bowBody->InitFromData(vertices, indices);
	return bowBody;
}

Mesh* Drawings::createSquare(std::string name, bool fill) {
	glm::vec3 color{  Constants::color };
	glm::vec3 center{ Constants::center };
	int side = 1;

	int i;
	std::vector<VertexFormat> vertices
	{
		// Left bottom corner
		VertexFormat(glm::vec3(center.x, center.y, 0), color),

		// Left up corner
		//VertexFormat(glm::vec3(center.x, center.y + length, 0), color),
		VertexFormat(glm::vec3(center.x, center.y + side, 0), color),

		// Right up corner
		//VertexFormat(glm::vec3(center.x + side, center.y + length, 0), color),
		VertexFormat(glm::vec3(center.x + side, center.y + side, 0), color),

		// Right bottom corner
		VertexFormat(glm::vec3(center.x + side, center.y, 0), color),
	};

	Mesh *ground = new Mesh(name);

	std::vector<unsigned short> indices{ 3, 2, 1, 0 };

	if (fill) {
		indices.push_back(3);
		indices.push_back(1);
		ground->SetDrawMode(GL_TRIANGLES);
	} else {
		ground->SetDrawMode(GL_LINE_LOOP);
	}


	ground->InitFromData(vertices, indices);
	return ground;
}
