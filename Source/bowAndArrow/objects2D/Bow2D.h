#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "../common/Constants.h"
#include "Object2D.h"
#include "Arrow2D.h"

class Bow2D : public Object2D {
protected:
    typedef struct {
        glm::vec3 color;
        glm::vec2 pos;
        float lengthOx;
        float widthOy;
        float stretch;
    } PowerBar;

public:
    Bow2D(glm::vec2 pos = Constants::initialBowPos, 
            glm::vec3 bodyColor     = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 stringColor   = glm::vec3(1, 1, 1));
    ~Bow2D();

    std::vector<Arrow2D*> arrows;
    std::vector<Arrow2D*> expiredArrows;

    glm::vec3 bodyColor;
    glm::vec3 stringColor;
    glm::vec2 pos;
    glm::vec2 lookAt;
    bool stringPulled;
    bool maxStretched;
    bool shotArrow;

    float reloadCooldown;
    float cooldown;
    float angle;
    int lifes;

    PowerBar powerBar;

    void reload();
    void updateArrowsShooterData();

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
