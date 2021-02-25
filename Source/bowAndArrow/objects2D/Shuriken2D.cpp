#include "Shuriken2D.h"

#include <Core/Engine.h>
#include "Bow2D.h"
#include "Balloon2D.h"

Shuriken2D::Shuriken2D(glm::vec2 pos,
                    float angularSpeed,
                    float speedOx,
                    int stars,
                    float spawnTime,
                    float radius,
                    glm::vec3 color) {
    printf("Shuriken\n");

    expired = true; // We have a timer to respawn it.

    this->pos = pos;
    this->angularSpeed = angularSpeed;
    this->color = color;
    this->stars = stars;
    this->radius = radius;
    this->spawnTime = spawnTime;
    hit = false;
    speed.ox = speedOx;
    initialPos = pos;
    spawnTimer = 0.0f;
    spawned = false;
    accelerationOy = 0.0;
    points = Constants::shurikenPoints * stars;
}

Shuriken2D::~Shuriken2D() {
    printf("Deleted: Shuriken ");
}

void Shuriken2D::update(float deltaTimeSeconds, float mouseHoldSeconds,
                        glm::vec2 resolution) {
    checkVisible(resolution);
    checkExpired(resolution);
    animate(deltaTimeSeconds, mouseHoldSeconds);
}

bool Shuriken2D::hitObject(Arrow2D& arrow, int* score) {
    return false;
}

bool Shuriken2D::hitObject(Balloon2D& balloon, int* score) {
    if (balloon.hit) return balloon.hit;
    float dx = pos.x - balloon.pos.x;
    float dy = pos.y - balloon.pos.y;
    float distance = glm::sqrt(dx * dx + dy * dy);
    if (distance < Constants::bowRadius + radius) {
        balloon.hit = true;
    }
    return balloon.hit;
}

bool Shuriken2D::hitObject(Obstacle2D& obstacle, int* score) {
    return false;
}

bool Shuriken2D::hitObject(Bow2D& bow, int* score) {
    if (bow.hit) return bow.hit;
    float dx = pos.x - bow.pos.x;
    float dy = pos.y - bow.pos.y;
    float distance = glm::sqrt(dx * dx + dy * dy);
    if (distance < Constants::bowRadius + radius) {
        --bow.lifes;
        printf("%d lifes remaining.\n", bow.lifes);
        bow.hit = true;
        return bow.hit;
    }
    return bow.hit;
}

bool Shuriken2D::hitObject(Shuriken2D& shuriken, int* score) {
    return false;
}

bool Shuriken2D::hitByObject(Object2D& object, int* score) {
    return object.hitObject(*this, score);
}

bool Shuriken2D::checkVisible(glm::vec2 resolution) {
    if (pos.x < 0 - radius
        || pos.x > resolution.x + radius) {
        visible = false;
        return visible;
    }
    if (pos.y > resolution.y + radius * 2) {
        visible = false;
        return visible;
    }
    if (pos.y <= 0 - radius * 2) {
        visible = false;
        return visible;
    }
    // If we got here than the object became visible.
    visible = true;
    return visible;
}

bool Shuriken2D::checkExpired(glm::vec2 resolution) {
    if (expired) return expired;
    /* Shurikens will always go towards the left part of screen. Hence we don't
    check if expired at the right part of the screen from Ox point of view. */
    if (pos.x < 0 - radius) {
        expired = true;
        spawned = false;
        return expired;
    }
    if (pos.y > resolution.y + radius * 2
        || pos.y < 0 - radius * 2) {
        spawned = false;
        expired = true;
        return expired;
    }
    return expired;
}

void Shuriken2D::animate(float deltaTimeSeconds, float mouseHoldSeconds) {
    if (!expired && spawned) {
        // Do animation for the not hitted, not expired shuriken.
        pos.x -= deltaTimeSeconds * speed.ox;
        angle -= deltaTimeSeconds * 3;
        if (hit) {
            accelerationOy += Constants::gravity * deltaTimeSeconds * 1000;
            pos.y -= accelerationOy;
            angle -= accelerationOy / 100;
        }

    } else if (expired && !stopSpawn) { // We respawn.
        spawnTimer += deltaTimeSeconds;
        if (spawnTimer >= spawnTime) {
            respawn();
        }
    }
}

void Shuriken2D::respawn() {
    angle = 0.0f;
    pos = initialPos;
    expired = false;
    spawned = true;
    spawnTimer = 0.0f;
    hit = false;
    accelerationOy = 0.0f;
}
