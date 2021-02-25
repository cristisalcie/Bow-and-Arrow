#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#include "../common/Constants.h"
#include "Object2D.h"

class Obstacle2D : public Object2D {
public:
    Obstacle2D(glm::vec2 pos = glm::vec2(0, 0),
            float lengthOx = Constants::leftGroundEnd,
            float widthOy = Constants::groundWidth,
            glm::vec3 color = glm::vec3(0.3f, 0.3f, 0.35f));
    ~Obstacle2D();

    float lengthOx;
    float widthOy;

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
