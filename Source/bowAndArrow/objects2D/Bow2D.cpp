#include "Bow2D.h"

#include <Core/Engine.h>
#include "Shuriken2D.h"

Bow2D::Bow2D(glm::vec2 pos, glm::vec3 bodyColor, glm::vec3 stringColor) {
    printf("Bow\n");
    this->pos = pos;
    initialPos = pos;
    this->bodyColor = bodyColor;
    this->stringColor = stringColor;
    lookAt = glm::vec2(pos.x + 1000, pos.y);  // Looks to the front.
    stringPulled = false;
    maxStretched = false;
    shotArrow = false;
    visible = true;
    hit = false;
    cooldown = Constants::bowCooldownLimit;
    reloadCooldown = Constants::bowReloadCooldownLimit;
    angle = Constants::bowInitialAngle;
    lifes = Constants::bowLifes;
    arrows.push_back(new Arrow2D(angle, pos));  // Starts with 1 arrow.

    powerBar.color = glm::vec3(0.0f, 1.0f, 0.0f);


    powerBar.lengthOx = Constants::powerBarLengthOx;
    powerBar.widthOy  = Constants::powerBarWidthOy;
    powerBar.stretch  = 0.0f;
}

Bow2D::~Bow2D() {
    for (auto arrow : arrows) {
        delete arrow;
    }
    printf("Deleted: Bow ");
}

void Bow2D::reload() {
    shotArrow = false;
    reloadCooldown = Constants::bowReloadCooldownLimit;
    if (expiredArrows.empty()) {
        arrows.push_back(new Arrow2D(angle, pos));
    } else {
        int lastArrId = expiredArrows.size() - 1;
        expiredArrows.at(lastArrId)->respawn(angle, pos);
        expiredArrows.pop_back();
    }
}

void Bow2D::updateArrowsShooterData() {
    for (Arrow2D* arrow : arrows) {
        if (!arrow->isShot && !arrow->expired && !arrow->collided) {
            arrow->updateShotByData(pos, angle);
        }
    }
}

void Bow2D::update(float deltaTimeSeconds, float mouseHoldSeconds,
					glm::vec2 resolution) {
	// Arrow update.
	for (Arrow2D* arrow : arrows) {
		arrow->update(deltaTimeSeconds, mouseHoldSeconds, resolution);

		/* We don't check here if we must reset the arrow because
		we could end up resetting multiple arrows, having them be
		put on the bow string when they are not needed. Hence we
		let the bow instance handle that situation when reloading. */
		if (arrow->collided && !arrow->expired) {
			if (arrow->checkExpired(resolution)) {
				auto it = expiredArrows.begin();
				expiredArrows.insert(it, arrow);
			}
		}
	}
	// ------------------------------------------------------------------------


	// Reloading algorithm.
	if (shotArrow) { reloadCooldown -= deltaTimeSeconds; }
	if (reloadCooldown <= 0.0f) { reload(); }
	// ------------------------------------------------------------------------


	// Power bar position update.
    powerBar.pos.x = pos.x;
    powerBar.pos.y = pos.y - Constants::bowRadius * Constants::bowFactor
        - 2 * Constants::powerBarWidthOy;
	// ------------------------------------------------------------------------


	// Routine booleans update.
	maxStretched        = mouseHoldSeconds > Constants::bowStretchTime;
	stringPulled        = mouseHoldSeconds > 0;
    powerBar.stretch    = mouseHoldSeconds * powerBar.lengthOx
                        / Constants::bowStretchTime;
	// ------------------------------------------------------------------------


    // Bow respawning
    if (hit) {
        cooldown -= deltaTimeSeconds;
        if (cooldown <= 0) {
            hit = false;
            cooldown = Constants::bowCooldownLimit;
        }
    }

	// ------------------------------------------------------------------------


	updateArrowsShooterData();
}

bool Bow2D::hitObject(Arrow2D& arrow, int* score) {
    return false;
}

bool Bow2D::hitObject(Balloon2D& balloon, int* score) {
    return false;
}

bool Bow2D::hitObject(Obstacle2D& obstacle, int* score) {
    return false;
}

bool Bow2D::hitObject(Bow2D& bow, int* score) {
    return false;
}

bool Bow2D::hitObject(Shuriken2D& shuriken, int* score) {
    if (shuriken.hit) return shuriken.hit;
    float dx = pos.x - shuriken.pos.x;
    float dy = pos.y - shuriken.pos.y;
    float distance = glm::sqrt(dx * dx + dy * dy);
    if (distance < Constants::bowRadius + shuriken.radius) {
        shuriken.hit = true;
        return shuriken.hit;
    }
    return shuriken.hit;
}

bool Bow2D::hitByObject(Object2D& object, int* score) {
    return object.hitObject(*this, score);
}

bool Bow2D::checkVisible(glm::vec2 resolution) {
    return true;
}

bool Bow2D::checkExpired(glm::vec2 resolution) {
    return false;
}

void Bow2D::animate(float deltaTimeSeconds, float mouseHoldSeconds) {
}

