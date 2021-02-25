#include "Balloon2D.h"

#include <Core/Engine.h>
#include "Arrow2D.h"

Balloon2D::Balloon2D(glm::vec2 pos,
                    glm::vec3 color,
                    float floatingSpeed,
                    float spawnTime,
                    float radius,
                    float radiusFactor,
                    glm::vec3 stringColor) {
    printf("Balloon\n");

    expired = true; // Because we have a timer until spawning.

    this->pos = pos;
    this->color = color;
    this->spawnTime = spawnTime;
    this->radius = radius;
    this->radiusFactor = radiusFactor;
    this->stringColor = stringColor;
    speed.oy = floatingSpeed;
    hitboxRadius = (radius + radius * radiusFactor) / 2.0f;
    hit = false;
    leftDirection = true;
    upDirection = false;
    size = 1.0f;
    spawned = false;

    initialPos = pos;
    spawnTimer = 0.0f;

    if (color == Constants::redBalloonColor) {
        points = Constants::balloonRedPoints;
    } else if (color == Constants::yellowBalloonColor) {
        points = Constants::balloonYellowPoints;
    }
}

Balloon2D::~Balloon2D() {
    printf("Deleted: Balloon ");
}

void Balloon2D::respawn() {
    angle = 0.0f;
    pos = initialPos;
    size = 1.0f;
    hit = false;
    expired = false;
    spawned = true;
    spawnTimer = 0.0f;
}

void Balloon2D::update(float deltaTimeSeconds, float mouseHoldSeconds,
                        glm::vec2 resolution) {
    checkVisible(resolution);
    checkExpired(resolution);
    animate(deltaTimeSeconds, mouseHoldSeconds);
}

bool Balloon2D::hitObject(Arrow2D& arrow, int* score) {
    return false;
}

bool Balloon2D::hitObject(Balloon2D& balloon, int* score) {
    return false;
}

bool Balloon2D::hitObject(Obstacle2D& obstacle, int* score) {
    return false;
}

bool Balloon2D::hitObject(Bow2D& bow, int* score) {
    return false;
}

bool Balloon2D::hitObject(Shuriken2D& shuriken, int* score) {
    return false;
}

bool Balloon2D::hitByObject(Object2D& object, int* score) {
    return object.hitObject(*this, score);
}

bool Balloon2D::checkVisible(glm::vec2 resolution) {
    if (pos.x < 0 - radius
        || pos.x > resolution.x + radius) {
        visible = false;
        return visible;
    }
    if (pos.y > resolution.y + radius * radiusFactor * 2) {
        visible = false;
        return visible;
    }
    if (pos.y <= 0 - radius * 2) {
        visible = false;
        return visible;
    }
    if (size <= 0) {
        visible = false;
        return visible;
    }
    // If we got here than the object became visible.
    visible = true;
    return visible;
}

bool Balloon2D::checkExpired(glm::vec2 resolution) {
    if (expired) return expired;
    if (pos.x < 0 - radius
        || pos.x > resolution.x + radius) {
        expired = true;
        spawned = false;
        return expired;
    }
    // Balloons always will go up (float) so we only check with up bound.
    if (pos.y > resolution.y + radius * radiusFactor * 2) {
        expired = true;
        spawned = false;
        return expired;
    }
    if (size <= 0) {
        expired = true;
        spawned = false;
        return expired;
    }
    if (hit && !visible) {
        // Was hit and through animations became invisible.
        expired = true;
        spawned = false;
        return expired;
    }
    return expired;
}

void Balloon2D::animate(float deltaTimeSeconds, float mouseHoldSeconds) {
    if (hit && visible) {
        if (leftDirection) {
            pos.x -= deltaTimeSeconds * 30;
            angle -= deltaTimeSeconds * 5;

        } else {
            pos.x += deltaTimeSeconds * 30;
            angle += deltaTimeSeconds * 5;

        }
        if (upDirection) pos.y += deltaTimeSeconds * 15;
        else pos.y -= deltaTimeSeconds * 15;


        if (size > 0.7) {
            float newSize{ size - Constants::balloonDeflateSpeed1 * deltaTimeSeconds };
            size = max(newSize, 0.7f);
        } else if (size > 0.31) {
            float newSize{ size - Constants::balloonDeflateSpeed2 * deltaTimeSeconds };
            size = max(newSize, 0.3f);
        } else if (size > 0.11) {
            float newSize{ size - Constants::balloonDeflateSpeed3 * deltaTimeSeconds };
            size = max(newSize, 0.10f);
        } else {
            float newSize{ size - Constants::balloonDeflateSpeed4 * deltaTimeSeconds };
            size = max(newSize, 0.0f);
        }
    } else if (!hit && !expired && spawned) {
        // do animations for the not hitted, not expired balloon
        pos.y += speed.oy * deltaTimeSeconds;
    } else if (expired && !stopSpawn) {
        spawnTimer += deltaTimeSeconds;
        if (spawnTimer >= spawnTime) {
            respawn();
        }
    }
}
