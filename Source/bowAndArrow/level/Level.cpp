#include "Level.h"

#include <Core/Engine.h>

Level::Level() {
    printf("Created Level\n");
	current = 0; // start from level 1.
	//current = 1;	// start from level 2.
	//current = 2;	// start from endless level.
    durationSeconds = 0;
	stopTimer = false;
    endless = false;
    // Because it needs to have it's objects vector generated by LevelFactory.
    started = true;     // DO NOT CHANGE THIS LINE
    ended   = true;     // DO NOT CHANGE THIS LINE
    // ------------------------------------------------------------------------
}

Level::~Level() {
    for (Object2D* obj : objects) {
        delete obj;
    }
    printf("Deleted Level\n");
}

void Level::update(float deltaTimeSeconds, float mouseHoldSeconds,
					glm::vec2 resolution, Bow2D* bow, int* score) {
	if (!ended) {
		updateTimer(deltaTimeSeconds, mouseHoldSeconds);
		updateObjects(deltaTimeSeconds, mouseHoldSeconds, resolution, bow,
						score);
		tryToEnd();
	} else {
		if (started) { // started and finished.
			generateNextLevel(resolution, bow);
		}
	}
}

void Level::updateTimer(float deltaTimeSeconds, float mouseHoldSeconds) {
	if (!stopTimer && !endless) {
		float newDuration = durationSeconds - deltaTimeSeconds;
		// Will never go negative.
		if (newDuration < 0) {
			durationSeconds = 0.0f;
			stopTimer = true;
		} else durationSeconds = newDuration;
	}
}

void Level::updateObjects(float deltaTimeSeconds, float mouseHoldSeconds,
						glm::vec2 resolution, Bow2D* bow, int* score) {
	for (Object2D* obj : objects) {
		// Check bow collisions.
		bow->hitByObject(*obj, score);
		// "obj" won't be arrow or bow.
		for (Arrow2D* arrow : bow->arrows) {
			obj->hitByObject(*arrow, score);
			arrow->hitByObject(*obj, score);
		}
		// Check collision with other objects.
		for (Object2D* otherObj : objects) {
			if (obj != otherObj) {
				obj->hitByObject(*otherObj, score);
			}
		}

		obj->update(deltaTimeSeconds, mouseHoldSeconds, resolution);

		if (stopTimer && !obj->stopSpawn) {
			// Timer stopped, object hasn't stopped spawn.
			obj->stopSpawn = true;
		}
	}
}

void Level::generateNextLevel(glm::vec2 resolution, Bow2D* bow) {
	durationSeconds =
		LevelFactory::generateNextLevel(++current, resolution, objects);
	if (durationSeconds == -100) endless = true;
	else endless = false;
	started = false; // Must press N to set it to true.
	stopTimer = false;
	for (Arrow2D* arrow : bow->arrows) {
		arrow->collided = false; // Forces all arrows to recheck if collided.
		arrow->speed.ox = 0;
		arrow->speed.oy = 0;
		arrow->speed.composed = Constants::gravity;
	}
	// We do NOT set level->ended to false here.
}

void Level::tryToEnd() {
    /* Check if we can end the game (all destructible objects finished
    animations). */
    if (stopTimer) {
        int expiredObj = 0;
        for (Object2D* obj : objects) {
            if (obj->indestructible) {
                ++expiredObj;
            } else if (obj->expired) {
                ++expiredObj;
            }
        }
        if (expiredObj == objects.size()) {
            ended = true;
        }
    }
}
