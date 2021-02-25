#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "../common/Constants.h"
#include "Object2D.h"

class Obstacle2D;

class Arrow2D : public Object2D {
private:
    typedef struct {
        glm::vec2 tipPos;
        glm::vec2 tipCenterPos;
        glm::vec2 tipUpCornerPos;
        glm::vec2 tipDownCornerPos;
    } Hitbox;

    /* Will hold the data about shooter object untill arrow is shot, or
    has collided or has expired. */
    typedef struct {
        glm::vec2 pos;
        float angle{};
    } ShotByData;

public:
    Arrow2D(float angle,
            glm::vec2 pos,
            glm::vec3 tipColor              = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 color                 = glm::vec3(0.2f, 1.0f, 0.0f),
            glm::vec3 rightFeathersColor    = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 leftFeathersColor     = glm::vec3(1.0f, 1.0f, 1.0f));
    ~Arrow2D();

    glm::vec3 tipColor;
    glm::vec3 rightFeathersColor;
    glm::vec3 leftFeathersColor;
	bool isShot;
	bool cancelShot;
	bool collided;
    float lifetime;
	Hitbox hitbox;
    ShotByData shotByData;


    void recalculateHitbox();
    void respawn(float angle,
            glm::vec2 pos,
            glm::vec3 tipColor              = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 color                 = glm::vec3(0.2f, 1.0f, 0.0f),
            glm::vec3 rightFeathersColor    = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 leftFeathersColor     = glm::vec3(1.0f, 1.0f, 1.0f));

    void updateShotByData(glm::vec2 pos, float angle);

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


