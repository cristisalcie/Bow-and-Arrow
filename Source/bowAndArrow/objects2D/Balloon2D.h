#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#include "../common/Constants.h"
#include "../level/Level.h"
#include "Object2D.h"

class Balloon2D : public Object2D {
public:
    Balloon2D(glm::vec2 pos,
            glm::vec3 color = glm::vec3(1, 1, 0),
            float floatingSpeed = 0,
            float spawnTime = 2, // default seconds
            float radius = Constants::balloonRadius,
            float radiusFactor = Constants::balloonEllipseFactor,
            glm::vec3 stringColor = glm::vec3(1.0f, 1.0f, 1.0f));
    virtual ~Balloon2D();


    glm::vec3 stringColor;
    bool leftDirection;
    bool upDirection;
    float size;
    float radius;
    float radiusFactor;
    float hitboxRadius;

    bool spawned;
    float spawnTime;
    float spawnTimer;


    void respawn();

    virtual void update(float deltaTimeSeconds, float mouseHoldSeconds,
        glm::vec2 resolution);

    virtual bool hitObject(Arrow2D& arrow, int* score);
    virtual bool hitObject(Balloon2D& balloon, int* score);
    virtual bool hitObject(Obstacle2D& obstacle, int* score);
    virtual bool hitObject(Bow2D& bow, int* score);
    virtual bool hitObject(Shuriken2D& shuriken, int* score);
    virtual bool hitByObject(Object2D& object, int* score);

    virtual bool checkVisible(glm::vec2 resolution);
    virtual bool checkExpired(glm::vec2 resolution);
    virtual void animate(float deltaTimeSeconds, float mouseHoldSeconds);
};
