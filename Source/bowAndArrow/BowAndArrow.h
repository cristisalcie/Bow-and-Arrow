#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

#include "objects2D/Object2D.h";
#include "objects2D/Bow2D.h";
#include "objects2D/Balloon2D.h";
#include "objects2D/Shuriken2D.h";
#include "objects2D/Obstacle2D.h";
#include "level/Level.h";

class BowAndArrow : public SimpleScene {
public:
	BowAndArrow();
	~BowAndArrow();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void renderObjects2D();
	void renderBow2D(Bow2D* bow);
	void renderPowerBar(Bow2D* bow);
	void renderArrow2D(Arrow2D* arrow);
	void renderBalloon2D(Balloon2D* balloon);
	void renderShuriken2D(Shuriken2D* shuriken);
	void renderObstacle2D(Obstacle2D *obstacle);

	void checkStatus(Bow2D* bow);

protected:
	glm::mat3 modelMatrix;
	GLenum polygonMode;
	glm::ivec2 res; 
	glm::vec3 cameraPos;
	bool devMode;
	bool gamePaused;
	bool gameOver;

	float mouseHoldSeconds;

	Level* level;
	Bow2D *bow{ nullptr };

	int* score;
};
