#include "Arrow2D.h"

#include <Core/Engine.h>
#include "Obstacle2D.h"
#include "Balloon2D.h"

Arrow2D::Arrow2D(float angle,
                glm::vec2 pos,
                glm::vec3 tipColor,
                glm::vec3 color, 
                glm::vec3 rightFeathersColor,
                glm::vec3 leftFeathersColor) {
    printf("Arrow\n");
    {
        float x = Constants::arrowBodyWeightCenter * glm::cos(glm::radians(angle));
        float y = Constants::arrowBodyWeightCenter * glm::sin(glm::radians(angle));
        this->pos.x = pos.x + x;
        this->pos.y = pos.y - y;
    }
    this->tipColor = tipColor;
    this->color = color;
    this->rightFeathersColor = rightFeathersColor;
    this->leftFeathersColor = leftFeathersColor;
    speed.ox = 0.0f;
    speed.oy = 0.0f;
    speed.composed = 0.0f;
    this->angle = angle;
    isShot = false;
    cancelShot = false;
    collided = false;
    visible = true;
    expired = false;
    lifetime = Constants::arrowLifeTime;
    shotByData.pos = pos;
    shotByData.angle = angle;
    recalculateHitbox();
}

Arrow2D::~Arrow2D() {
    printf("Deleted: Arrow ");
}

void Arrow2D::recalculateHitbox() {
    // Note: We are initially at the weight center of the arrow.
    float fullTriangleTipHeight
    {
        Constants::arrowTipHeight
        - Constants::arrowTipCenterToCornerOnOx
    };
    float smallTriangleTipHeight
    {
        Constants::arrowTipHeight
        - Constants::arrowTipHoleRadius
    };
    float endPointOfBodyLen
    {
        Constants::arrowBodyLength - Constants::arrowBodyWeightCenter
    };
    float tipRadiusToCornerLen
    {
        Constants::arrowTipHoleRadius - Constants::arrowTipCenterToCornerOnOx
    };
    hitbox.tipPos.x = pos.x + (smallTriangleTipHeight + endPointOfBodyLen)
        * glm::cos(glm::radians(angle));
    hitbox.tipPos.y = pos.y - (smallTriangleTipHeight + endPointOfBodyLen)
        * glm::sin(glm::radians(angle));

    hitbox.tipCenterPos.x = pos.x + (smallTriangleTipHeight / 2.0f
        + endPointOfBodyLen) * glm::cos(glm::radians(angle));
    hitbox.tipCenterPos.y = pos.y - (smallTriangleTipHeight / 2.0f
        + endPointOfBodyLen) * glm::sin(glm::radians(angle));

    // We move both centers to be corresponding on Ox axis with arrow corners.
    hitbox.tipDownCornerPos.x = pos.x + (endPointOfBodyLen - tipRadiusToCornerLen)
        * glm::cos(glm::radians(angle));
    hitbox.tipDownCornerPos.y = pos.y
        - (endPointOfBodyLen - tipRadiusToCornerLen) * glm::sin(glm::radians(angle));
    hitbox.tipUpCornerPos.x = hitbox.tipDownCornerPos.x;
    hitbox.tipUpCornerPos.y = hitbox.tipDownCornerPos.y;

    /* Sin on Ox because when angle = 0, we don't need to move on Ox axis
    because arrow is pointing parallel with Ox axis. */
    hitbox.tipDownCornerPos.x -= Constants::arrowTipBodySideLength / 2
        * glm::sin(glm::radians(angle));
    /* Cos on Oy because when angle = 90, we don't need to move on Oy axis
    because arrow is pointing down, parallel with Oy axis. */
    hitbox.tipDownCornerPos.y -= Constants::arrowTipBodySideLength / 2
        * glm::cos(glm::radians(angle));

    /* Same explanations as above. Same equations, except because it is up
    corner we need to add to x and y.*/
    hitbox.tipUpCornerPos.x += Constants::arrowTipBodySideLength / 2
        * glm::sin(glm::radians(angle));
    hitbox.tipUpCornerPos.y += Constants::arrowTipBodySideLength / 2
        * glm::cos(glm::radians(angle));


}

void Arrow2D::respawn(float angle,
                    glm::vec2 pos,
                    glm::vec3 tipColor,
                    glm::vec3 bodyColor, 
                    glm::vec3 rightFeathersColor,
                    glm::vec3 leftFeathersColor) {
    printf("Reseted arrow\n");
    this->pos = pos;
    this->tipColor = tipColor;
    this->color = bodyColor;
    this->rightFeathersColor = rightFeathersColor;
    this->leftFeathersColor = leftFeathersColor;
    speed.ox = 0.0f;
    speed.oy = 0.0f;
    speed.composed = 0.0f;
    this->angle = angle;
    isShot = false;
    cancelShot = false;
    collided = false;
    expired = false;
    lifetime = Constants::arrowLifeTime;
    visible = true;
    recalculateHitbox();
}



void Arrow2D::updateShotByData(glm::vec2 pos, float angle) {
    shotByData.pos = pos;
    shotByData.angle = angle;
}

void Arrow2D::update(float deltaTimeSeconds, float mouseHoldSeconds,
                        glm::vec2 resolution) {
    if (!collided) {
        animate(deltaTimeSeconds, mouseHoldSeconds);
        recalculateHitbox();

        // Check if is out of bounds.
        // checkVisible basically checks if it hit invisible wall.
        checkVisible(resolution);
    } else if (!expired) {
        // Update arrow lifetime.
        float newLifetime = lifetime - deltaTimeSeconds;
        lifetime = max(0, newLifetime);
    }
}

bool Arrow2D::hitObject(Arrow2D& arrow, int* score) {
    return false;
}

bool Arrow2D::hitObject(Obstacle2D& obstacle, int* score) {
    // Obstacle collision check
    bool collidedOnObstacleOyAxis
    {
        // Arrow hit obstacle on Oy axis.
        (hitbox.tipCenterPos.y < obstacle.pos.y + obstacle.widthOy)
        && (hitbox.tipCenterPos.y > obstacle.pos.y)
    };

    bool collidedOnObstacleOxAxis
    {
        // Arrow hit obstacle on Ox axis.
        (hitbox.tipCenterPos.x > obstacle.pos.x)
        && (hitbox.tipCenterPos.x < obstacle.pos.x + obstacle.lengthOx)
    };

    if (collidedOnObstacleOxAxis && collidedOnObstacleOyAxis) {
        // Arrow hit obstacle.
        collided = true;
        if (!isShot) {
            isShot = true;
        }
    }
    return collided;
}

bool Arrow2D::hitObject(Bow2D& bow, int* score) {
    return false;
}

bool Arrow2D::hitObject(Balloon2D& balloon, int* score) {
    if (balloon.hit) return balloon.hit;
    if (!balloon.visible) return balloon.hit;

    float dx{ balloon.pos.x - hitbox.tipPos.x };
    float dy{ balloon.pos.y - hitbox.tipPos.y };
    float distance = glm::sqrt(dx * dx + dy * dy);

    balloon.hit = distance < balloon.hitboxRadius;

    if (!balloon.hit) {
        dx = balloon.pos.x - hitbox.tipCenterPos.x;
        dy = balloon.pos.y - hitbox.tipCenterPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        balloon.hit = balloon.hit || distance < balloon.hitboxRadius;
    }

    if (!balloon.hit) {
        dx = balloon.pos.x - hitbox.tipUpCornerPos.x;
        dy = balloon.pos.y - hitbox.tipUpCornerPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        balloon.hit = balloon.hit || distance < balloon.hitboxRadius;
    }

    if (!balloon.hit) {
        dx = balloon.pos.x - hitbox.tipDownCornerPos.x;
        dy = balloon.pos.y - hitbox.tipDownCornerPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        balloon.hit = balloon.hit || distance < balloon.hitboxRadius;
    }

    if (balloon.hit) {
        if ((int)(speed.composed) % 2) balloon.leftDirection = false;
        else balloon.leftDirection = true;

        if ((int)(angle) % 2) balloon.upDirection = true;
        else balloon.upDirection = false;
    }

    if (balloon.hit) {
        *score += balloon.points;
        printf("Score: %d\n", *score);
    }

    return balloon.hit;
}

bool Arrow2D::hitObject(Shuriken2D& shuriken, int* score) {
    if (shuriken.hit) return shuriken.hit;
    if (!shuriken.visible) return shuriken.hit;

    float dx{ shuriken.pos.x - hitbox.tipPos.x };
    float dy{ shuriken.pos.y - hitbox.tipPos.y };
    float distance = glm::sqrt(dx * dx + dy * dy);

    shuriken.hit = distance < shuriken.radius;

    if (!shuriken.hit) {
        dx = shuriken.pos.x - hitbox.tipCenterPos.x;
        dy = shuriken.pos.y - hitbox.tipCenterPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        shuriken.hit = shuriken.hit || distance < shuriken.radius;
    }

    if (!shuriken.hit) {
        dx = shuriken.pos.x - hitbox.tipUpCornerPos.x;
        dy = shuriken.pos.y - hitbox.tipUpCornerPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        shuriken.hit = shuriken.hit || distance < shuriken.radius;
    }

    if (!shuriken.hit) {
        dx = shuriken.pos.x - hitbox.tipDownCornerPos.x;
        dy = shuriken.pos.y - hitbox.tipDownCornerPos.y;
        distance = glm::sqrt(dx * dx + dy * dy);
        shuriken.hit = shuriken.hit || distance < shuriken.radius;
    }

    if (shuriken.hit) {
        *score += shuriken.points;
        printf("Score: %d\n", *score);
    }

    return shuriken.hit;
}

bool Arrow2D::hitByObject(Object2D& object, int* score) {
    return object.hitObject(*this, score);
}

bool Arrow2D::checkVisible(glm::vec2 resolution) {
    if (collided) return visible; // Has collided on obstacle
    bool hitInvisibleBoundaryOyAxis
    {
        // Too far down from Oy point of view, so arrow collides.
        hitbox.tipCenterPos.y < -Constants::arrowBodyLength * 1.2
    };
    bool hitInvisibleBoundaryOxAxis
    {
        // Too far from Ox point of view, so arrow ccollides.
        hitbox.tipCenterPos.x > resolution.x + Constants::arrowBodyLength * 1.2
    };
    if (hitInvisibleBoundaryOxAxis || hitInvisibleBoundaryOyAxis) {
        collided = true;
        if (!isShot) {
            isShot = true;
        }
        visible = false;
    }
    return visible;
}

bool Arrow2D::checkExpired(glm::vec2 resolution) {
    if (lifetime <= 0) expired = true;
    return expired;
}

void Arrow2D::animate(float deltaTimeSeconds, float mouseHoldSeconds) {
    if (!isShot) {
        // Rotates and moves wherever the bow is.
        angle = shotByData.angle;
        pos.x = shotByData.pos.x;
        pos.y = shotByData.pos.y;

        // The arrow's center must be at intersection with the bow string.
        float x = Constants::arrowBodyWeightCenter
            * glm::cos(glm::radians(angle));
        float y = Constants::arrowBodyWeightCenter
            * glm::sin(glm::radians(angle));
        pos.x += x;
        pos.y -= y;

        // Pull animation.
        float stringFactor{ -mouseHoldSeconds / Constants::bowDeformationFactor.x };
        float stringPull = glm::abs(stringFactor * Constants::bowRadius);
        pos.x -= stringPull * glm::cos(glm::radians(angle));
        pos.y += stringPull * glm::sin(glm::radians(angle));

    } else {
        float v_x = speed.composed * glm::cos(glm::radians(angle));
        float v_y = speed.composed * glm::sin(glm::radians(angle));
        // Update speed on Oy from gravity pulling.
        speed.oy += Constants::gravity * deltaTimeSeconds;
        // And we add it to the arrow position on Oy.
        v_y += speed.oy;

        // Update arrow positions.
        pos.x += v_x;
        pos.y -= v_y;

        // Recalculate angle
        angle = glm::degrees(glm::atan(v_y / v_x));
        // Recalculate speed
        speed.composed = glm::sqrt(v_x * v_x  + v_y * v_y);
    }
}
