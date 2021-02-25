#include "Obstacle2D.h"

#include <Core/Engine.h>

Obstacle2D::Obstacle2D(glm::vec2 pos, float lengthOx, float widthOy,
                   glm::vec3 color) {
    printf("Obstacle\n");
    this->pos = pos;
    this->lengthOx = lengthOx;
    this->widthOy = widthOy;
    this->color = color;
    indestructible = true;
}

Obstacle2D::~Obstacle2D() {
    printf("Deleted: Obstacle ");
}

void Obstacle2D::update(float deltaTimeSeconds, float mouseHoldSeconds,
    glm::vec2 resolution) {
}


bool Obstacle2D::hitObject(Arrow2D& arrow, int* score) {
    return false;
}

bool Obstacle2D::hitObject(Balloon2D& balloon, int* score) {
    return false;
}

bool Obstacle2D::hitObject(Obstacle2D& obstacle, int* score) {
    return false;
}

bool Obstacle2D::hitObject(Bow2D& bow, int* score) {
    return false;
}

bool Obstacle2D::hitObject(Shuriken2D& shuriken, int* score) {
    return false;
}

bool Obstacle2D::hitByObject(Object2D& object, int* score) {
    return object.hitObject(*this, score);
}

bool Obstacle2D::checkVisible(glm::vec2 resolution) {
    return true;
}

bool Obstacle2D::checkExpired(glm::vec2 resolution) {
    return false;
}

void Obstacle2D::animate(float deltaTimeSeconds, float mouseHoldSeconds) {
}
