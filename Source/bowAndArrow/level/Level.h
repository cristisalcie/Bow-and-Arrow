#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "LevelFactory.h"
#include "../objects2D/Object2D.h"
#include "../objects2D/Obstacle2D.h"
#include "../objects2D/Bow2D.h"
#include "../common/Constants.h"

class Level {
public:
    Level();
    ~Level();

    std::vector<Object2D*> objects;

    int current;
    float durationSeconds;
    bool stopTimer;
    bool endless;
    bool started;
    bool ended;

    void update(float deltaTimeSeconds, float mouseHoldSeconds,
                glm::vec2 resolution, Bow2D* bow, int* score);

protected:
    void updateTimer(float deltaTimeSeconds, float mouseHoldSeconds);
    void updateObjects(float deltaTimeSeconds, float mouseHoldSeconds,
                        glm::vec2 resolution, Bow2D* bow, int* score);
    void generateNextLevel(glm::vec2 resolution, Bow2D* bow);
    void tryToEnd();
};
