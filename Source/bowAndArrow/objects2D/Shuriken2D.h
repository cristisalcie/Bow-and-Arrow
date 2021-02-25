#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "Object2D.h"
#include "../common/Constants.h"

class Shuriken2D : public Object2D {
public:
    Shuriken2D(glm::vec2 pos,
        float speedOx,
        float angularSpeed,
        int stars = 3,
        float spawnTime = 2, // default seconds
        float radius = Constants::shurikenRadius,
        glm::vec3 color = glm::vec3(0.25f, 0.25f, 0.25f));
    ~Shuriken2D();

    int stars;
    float angularSpeed;
    float accelerationOy;
    float radius;

    bool spawned;
    float spawnTime;
    float spawnTimer;

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

    void respawn();
};
