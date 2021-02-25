#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Arrow2D;
class Balloon2D;
class Obstacle2D;
class Bow2D;
class Shuriken2D;

class Object2D {
protected:
    typedef struct {
        float ox;
        float oy;
        float composed;
    } Speed;

public:
    Object2D();
    virtual ~Object2D();

    glm::vec3 color;
    glm::vec2 initialPos;
    glm::vec2 pos;
    Speed speed;
    bool expired;
    bool visible;
    float angle;
    int points;

    // So level knows that object is always visible and never expired
    bool indestructible;

    // So level can tell objects after some time to stop spawn.
    bool stopSpawn;

    // Object hit "switch".
    bool hit;

    // = 0 means pure virtual function. Hence this class can't be instantiated.
    virtual void update(float deltaTimeSeconds, float mouseHoldSeconds,
                        glm::vec2 resolution) = 0;

    /* The idea here is: If arrow hit balloon then the balloon is suffering.
    If we implement the balloon hit arrow then the arrow is suffering. */
    virtual bool hitObject(Arrow2D& arrow, int* score) = 0;
    virtual bool hitObject(Balloon2D& balloon, int* score) = 0;
    virtual bool hitObject(Obstacle2D& obstacle, int* score) = 0;
    virtual bool hitObject(Bow2D& bow, int* score) = 0;
    virtual bool hitObject(Shuriken2D& shuriken, int* score) = 0;
    virtual bool hitByObject(Object2D& object, int* score) = 0;

    virtual bool checkVisible(glm::vec2 resolution) = 0;
    virtual bool checkExpired(glm::vec2 resolution) = 0;
    virtual void animate(float deltaTimeSeconds, float mouseHoldSeconds) = 0;
};
