#include "BowAndArrow.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "common/Transform2D.h"
#include "common/Drawings.h" // It also includes "common/Constants.h"

BowAndArrow::BowAndArrow() {
	score = new int(0);
	mouseHoldSeconds = 0.0f;
	devMode = false;
	polygonMode = GL_FILL;

	bow = new Bow2D();
	level = new Level();
	gamePaused = false;
	gameOver   = false;
}

BowAndArrow::~BowAndArrow() {
	/* The C++ language guarantees that delete level/bow will do nothing if
	level/bow is equal to NULL. */
	delete bow;
	delete level;
	delete score;
}

void BowAndArrow::Init() {
	res = window->GetResolution();
	cameraPos = glm::vec3(0, 0, 50);

	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)res.x, 0, (float)res.y, 0.01f, 400);
	camera->SetPosition(cameraPos);
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// Non animated drawings. (Only created once).
	{
		Mesh *square = Drawings::createSquare("square", true);
		AddMeshToList(square);

		Mesh *squareOutline = Drawings::createSquare("squareOutline", false);
		AddMeshToList(squareOutline);

		Mesh *shuriken_2star = Drawings::createShuriken("shuriken_2star", 2);
		AddMeshToList(shuriken_2star);

		Mesh *shuriken_3star = Drawings::createShuriken("shuriken_3star", 3);
		AddMeshToList(shuriken_3star);

		Mesh *shuriken4_star = Drawings::createShuriken("shuriken_4star", 4);
		AddMeshToList(shuriken4_star);

		Mesh *shuriken5_star = Drawings::createShuriken("shuriken_5star", 5);
		AddMeshToList(shuriken5_star);

		Mesh *shuriken6_star = Drawings::createShuriken("shuriken_6star", 6);
		AddMeshToList(shuriken6_star);

		Mesh *balloon = Drawings::createBalloon("balloon");
		AddMeshToList(balloon);

		Mesh *balloonStr = Drawings::createBalloonStr("balloonStr");
		AddMeshToList(balloonStr);

		Mesh *arrowTip = Drawings::createArrowTip("arrowTip");
		AddMeshToList(arrowTip);

		Mesh *arrowBody = Drawings::createArrowBody("arrowBody");
		AddMeshToList(arrowBody);

		Mesh *arrowFeathers = Drawings::createArrowFeathers("arrowFeathers");
		AddMeshToList(arrowFeathers);
	}
}

void BowAndArrow::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.47f, 0.77f, 0.83f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);
}

void BowAndArrow::Update(float deltaTimeSeconds) { // Is in charge of the game.
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	if (devMode) {
		auto camera = GetSceneCamera();
		camera->SetPosition(cameraPos);
	}

	if (!gameOver) {
		if (!gamePaused) {
			level->update(deltaTimeSeconds, mouseHoldSeconds, res, bow, score);

			// bow is the player.
			bow->update(deltaTimeSeconds, mouseHoldSeconds, res);

			checkStatus(bow); // out of lifes, and what to do about that.
		}

		renderObjects2D();
	}
}

void BowAndArrow::FrameEnd() {
}

void BowAndArrow::OnInputUpdate(float deltaTime, int mods) {
	if (!devMode && window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && !gamePaused) {
		for (Arrow2D* arrow : bow->arrows) {
			if (!arrow->isShot && !bow->maxStretched && !arrow->cancelShot) {
				mouseHoldSeconds += glm::min(Constants::bowStretchTime - mouseHoldSeconds, deltaTime);
				/* In case in the future we want to shoot 2 arrows at the same
				time, we only pull the string for 1 of the arrows. If we
				wouldn't break and we would shoot 2 arrows at the same time,
				the bow would stretch twice as fast. */
				break;
			}
		}
	}

	if (devMode) {
		if (window->KeyHold(GLFW_KEY_W)) 
			cameraPos.y += deltaTime * Constants::cameraSpeed;
		if (window->KeyHold(GLFW_KEY_A)) 
			cameraPos.x -= deltaTime * Constants::cameraSpeed;
		if (window->KeyHold(GLFW_KEY_S)) 
			cameraPos.y -= deltaTime * Constants::cameraSpeed;
		if (window->KeyHold(GLFW_KEY_D)) 
			cameraPos.x += deltaTime * Constants::cameraSpeed;
	} else if (!gamePaused) {
		// Bow motion on Oy axis. Should go to a separate function.
		if (window->KeyHold(GLFW_KEY_W)) {

			if (bow->pos.y < (res.y - 1) - 2.4f * Constants::bowRadius) {
				bow->pos.y += deltaTime * 100;
				float newBowAngle
				{
					-glm::atan((bow->lookAt.y - bow->pos.y)
					/ (bow->lookAt.x - bow->pos.x)) * 180 / 3.14f
				};
				if (glm::abs(newBowAngle) < Constants::bowMaxAngle
					&& (bow->lookAt.x - bow->pos.x) > 0) {
					bow->angle = newBowAngle;
				}
			}
		}
		if (window->KeyHold(GLFW_KEY_S)) {

			if (bow->pos.y > 2.4f * Constants::bowRadius
				+ Constants::groundWidth) {
				bow->pos.y -= deltaTime * 100;
				float newBowAngle
				{
					-glm::atan((bow->lookAt.y - bow->pos.y)
					/ (bow->lookAt.x - bow->pos.x)) * 180 / 3.14f
				};
				if (glm::abs(newBowAngle) < Constants::bowMaxAngle
					&& (bow->lookAt.x - bow->pos.x) > 0) {
					bow->angle = newBowAngle;
				}
			}
		}
		// --------------------------------------------------------------------
	}
}

void BowAndArrow::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_F2) {
		devMode = !devMode;
	}
	if (devMode && key == GLFW_KEY_SPACE) {
		switch (polygonMode) {
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
	if (!devMode && key == GLFW_KEY_N && !level->started) {
		level->started = true;
		level->ended = false;
		std::cout << "Lifes remaining: " << bow->lifes << '\n';
	}
	if (!devMode && key == GLFW_KEY_ENTER && gameOver) {
		// Reset level.
		level->current = 0;
		level->started = true;
		level->ended = true;

		// Reset bow.
		bow->lifes = Constants::bowLifes;
		bow->pos = bow->initialPos;

		*score = 0;
		gameOver = false;
	}
	if (key == GLFW_KEY_P) {
		gamePaused = !gamePaused;
	}
}

void BowAndArrow::OnKeyRelease(int key, int mods) {
}

void BowAndArrow::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	if (!devMode && !gamePaused) {
		bow->lookAt = glm::vec2(mouseX, (res.y - 1) - mouseY);

		float newBowAngle
		{
			-glm::atan((bow->lookAt.y - bow->pos.y) / (bow->lookAt.x - bow->pos.x))
			* 180 / 3.14f
		};
		if (glm::abs(newBowAngle) < Constants::bowMaxAngle
			&& (bow->lookAt.x - bow->pos.x) > 0) {
			bow->angle = newBowAngle;
		}
	}
}

void BowAndArrow::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	if (!devMode && !gamePaused) {
		if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
			// Cancel taking the shot
			bool resetString = false;
			for (Arrow2D* arrow : bow->arrows) {
				if (!arrow->isShot && bow->stringPulled) {
					arrow->cancelShot = true;
					resetString = true;
				}
			}
			if (resetString) {
				mouseHoldSeconds = 0.0f;
			}
		}
	}
}

void BowAndArrow::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	if (!devMode && !gamePaused) {
		if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
			for (Arrow2D* arrow : bow->arrows) {
				if (!arrow->isShot) {
					if (arrow->cancelShot) {
						arrow->cancelShot = false;
					} else {
						bow->shotArrow = true;
						arrow->isShot = true;
						arrow->speed.composed = mouseHoldSeconds * Constants::arrowSpeedFactor;
					}
				}
			}
			// String is released
			mouseHoldSeconds = 0.0f;
		}
	}
}

void BowAndArrow::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void BowAndArrow::OnWindowResize(int width, int height) {
}

void BowAndArrow::renderObjects2D() {
	if (!level->ended) {
		for (Object2D* obj : level->objects) {
			if (Obstacle2D* obstacle = dynamic_cast<Obstacle2D*>(obj)) {
				renderObstacle2D(obstacle);
			} else if (Balloon2D* balloon = dynamic_cast<Balloon2D*>(obj)) {
				renderBalloon2D(balloon);
			} else if (Shuriken2D* shuriken = dynamic_cast<Shuriken2D*>(obj)) {
				renderShuriken2D(shuriken);
			}
		}
	}
	for (Arrow2D* arrow : bow->arrows) renderArrow2D(arrow);
	renderBow2D(bow);
	renderPowerBar(bow);
}

void BowAndArrow::renderBow2D(Bow2D* bow) {
	if (!bow->visible) return;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow->pos.x, bow->pos.y);
	/* We always rotate after translation here, because we want the bow to move
	on Oy axis at all angles. */
	modelMatrix *= Transform2D::Rotate(glm::radians(bow->angle));

	// Bow body bend animation
	glm::vec2 bodyFactor{ glm::vec2(1.0f, Constants::bowFactor) };
	bodyFactor.y -= mouseHoldSeconds / Constants::bowDeformationFactor.y;
	Mesh* bowBody = Drawings::createBowBody("bowBody", bodyFactor);
	RenderMesh2D(bowBody, modelMatrix, bow->bodyColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	RenderMesh2D(bowBody, modelMatrix, bow->bodyColor);
	modelMatrix *= Transform2D::Translate(0, -1);
	RenderMesh2D(bowBody, modelMatrix, bow->bodyColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	delete(bowBody);

	// Bow string bend animation
	glm::vec2 stringFactor{ glm::vec2(0.0f, Constants::bowFactor) };
	stringFactor.x -= mouseHoldSeconds / Constants::bowDeformationFactor.x;
	stringFactor.y -= mouseHoldSeconds / Constants::bowDeformationFactor.y;
	Mesh* bowString = Drawings::createBowString("bowString", stringFactor);

	RenderMesh2D(bowString, modelMatrix, bow->stringColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	RenderMesh2D(bowString, modelMatrix, bow->stringColor);
	modelMatrix *= Transform2D::Translate(0, -1);
	RenderMesh2D(bowString, modelMatrix, bow->stringColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	delete(bowString);
}

void BowAndArrow::renderPowerBar(Bow2D* bow) {
	if (!bow->stringPulled) return;
	glm::vec3 outlineColor{ glm::vec3(0.0f, 0.0f, 0.0f) };

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow->powerBar.pos.x, bow->powerBar.pos.y);
	modelMatrix *= Transform2D::Scale(bow->powerBar.lengthOx, bow->powerBar.widthOy);
	RenderMesh2D(meshes["squareOutline"], modelMatrix, outlineColor);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow->powerBar.pos.x, bow->powerBar.pos.y);
	modelMatrix *= Transform2D::Scale(bow->powerBar.stretch, bow->powerBar.widthOy);
	RenderMesh2D(meshes["square"], modelMatrix, bow->powerBar.color);
}

void BowAndArrow::renderArrow2D(Arrow2D *arrow) {
	if (!arrow->visible) return;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(arrow->pos.x, arrow->pos.y);
	modelMatrix *= Transform2D::Rotate(glm::radians(arrow->angle));

	RenderMesh2D(meshes["arrowBody"], modelMatrix, arrow->color);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	RenderMesh2D(meshes["arrowBody"], modelMatrix, arrow->color);
	modelMatrix *= Transform2D::Translate(0, -1);
	RenderMesh2D(meshes["arrowBody"], modelMatrix, arrow->color);
	modelMatrix *= Transform2D::Translate(0, 0.5);
	// Align the tip to the body
	{
		int xOffset = Constants::arrowBodyLength
			- Constants::arrowBodyWeightCenter - Constants::arrowTipHoleRadius;
		modelMatrix *= Transform2D::Translate(xOffset, 0);
		RenderMesh2D(meshes["arrowTip"], modelMatrix, arrow->tipColor);
	}
	// Align the feathers to the body
	{
		int xOffset = Constants::arrowTipHoleRadius;
		xOffset -= Constants::arrowBodyLength * 9 / 10;
		modelMatrix *= Transform2D::Translate(xOffset, 0);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->leftFeathersColor);
		modelMatrix *= Transform2D::Translate(0, 0.5f);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->leftFeathersColor);
		modelMatrix *= Transform2D::Translate(0, -1);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->leftFeathersColor);
		modelMatrix *= Transform2D::Translate(0, 0.5f);

		xOffset = Constants::arrowBodyLength / 10;
		modelMatrix *= Transform2D::Translate(xOffset, 0);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->rightFeathersColor);
		modelMatrix *= Transform2D::Translate(0, 0.5f);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->rightFeathersColor);
		modelMatrix *= Transform2D::Translate(0, -1);
		RenderMesh2D(meshes["arrowFeathers"], modelMatrix,
			arrow->rightFeathersColor);
		modelMatrix *= Transform2D::Translate(0, 0.5f);
	}
}

void BowAndArrow::renderBalloon2D(Balloon2D* balloon) {
	if (!balloon->visible) return;
	float radius{ Constants::balloonRadius };
	float factor{ Constants::balloonEllipseFactor };
	float tHeight{ Constants::balloonTriangleHeight };
	float tBotSideLen{ Constants::balloonTriangleBotSideLength };

	float yOffset{ -radius * factor };

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(balloon->pos.x, balloon->pos.y);
	modelMatrix *= Transform2D::Scale(balloon->size, balloon->size);
	modelMatrix *= Transform2D::Rotate(balloon->angle);
	RenderMesh2D(meshes["balloon"], modelMatrix, balloon->color);

	modelMatrix *= Transform2D::Translate(0, yOffset);

	RenderMesh2D(meshes["balloonStr"], modelMatrix, balloon->stringColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
	RenderMesh2D(meshes["balloonStr"], modelMatrix, balloon->stringColor);
	modelMatrix *= Transform2D::Translate(0, -1);
	RenderMesh2D(meshes["balloonStr"], modelMatrix, balloon->stringColor);
	modelMatrix *= Transform2D::Translate(0, 0.5f);
}

void BowAndArrow::renderShuriken2D(Shuriken2D* shuriken) {
	if (!shuriken->visible) return;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(shuriken->pos.x, shuriken->pos.y);
	modelMatrix *= Transform2D::Rotate(shuriken->angle);

	switch (shuriken->stars) {
	case 2:
		RenderMesh2D(meshes["shuriken_2star"], modelMatrix, shuriken->color);
		break;
	case 3:
		RenderMesh2D(meshes["shuriken_3star"], modelMatrix, shuriken->color);
		break;
	case 4:
		RenderMesh2D(meshes["shuriken_4star"], modelMatrix, shuriken->color);
		break;
	case 5:
		RenderMesh2D(meshes["shuriken_5star"], modelMatrix, shuriken->color);
		break;
	case 6:
		RenderMesh2D(meshes["shuriken_6star"], modelMatrix, shuriken->color);
		break;
	default:
		RenderMesh2D(meshes["shuriken_3star"], modelMatrix, shuriken->color);
		break;
	}
}

void BowAndArrow::renderObstacle2D(Obstacle2D* obstacle) {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(obstacle->pos.x, obstacle->pos.y);
	modelMatrix *= Transform2D::Scale(obstacle->lengthOx, obstacle->widthOy);
	RenderMesh2D(meshes["square"], modelMatrix, obstacle->color);
}

void BowAndArrow::checkStatus(Bow2D* bow) {
	if (bow->lifes <= 0) {
		gameOver = true;
		std::cout << "Game over.\n"; 
		std::cout << "Final score: " << *score << '\n'; 
		std::cout << "Menu:\n";
		std::cout << "ENTER: play again\n"; 
		std::cout << "ESC: exit\n"; 
	}
}
