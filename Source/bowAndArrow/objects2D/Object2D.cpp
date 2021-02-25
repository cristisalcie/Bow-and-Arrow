#include "Object2D.h"

Object2D::Object2D() {
    printf("Created OBJECT: ");
    color           = glm::vec3(1.0f, 1.0f, 1.0f);
    speed.ox        = 0;
    speed.oy        = 0;
    speed.composed  = 0;
    pos             = glm::vec2(0.0f, 0.0f);
    initialPos      = pos;
    expired         = false;
    visible         = false;
    angle           = 0.0f;
    indestructible  = false;
    stopSpawn       = false;
    hit             = false;
    points          = 0;
}

Object2D::~Object2D() {
    printf("OBJECT\n");
}