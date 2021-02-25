#include "LevelFactory.h"

#include <Core/Engine.h>

float LevelFactory::generateNextLevel(int level, glm::vec2 resolution, std::vector<Object2D*>& objects) {
    while (!objects.empty()) {
        Object2D* tmpObj = objects.at(objects.size() - 1);
        objects.pop_back();
        delete tmpObj;
    }
    float levelDurationSeconds = -100; // endless mode.
    switch (level) {
    case 1:
        levelDurationSeconds = generateLevel1(resolution, objects);
        printf("Press N to start level 1.\n");
        break;
    case 2:
        levelDurationSeconds = generateLevel2(resolution, objects);
        printf("Press N to start level 2.\n");
        break;
    default:
        generateEndless(resolution, objects);
        printf("Press N to start endless level.\n");
        break;
    }
    printf("Press P to pause/unpause the game.\n");
    return levelDurationSeconds;
}

float LevelFactory::generateLevel1(glm::vec2 resolution, std::vector<Object2D*>& objects) {
    // Obstacles
    objects.push_back(new Obstacle2D(
        glm::vec2(Constants::leftGroundEnd * 0.75f, resolution.y * 0.3f),
        Constants::obstacleLengthOx,
        Constants::obstacleWidthOy));

    objects.push_back(new Obstacle2D(
        glm::vec2(Constants::leftGroundEnd * 0.85f, resolution.y * 0.75f),
        Constants::obstacleLengthOx,
        Constants::obstacleWidthOy * 1.3));

    // Ox ground
    objects.push_back(new Obstacle2D(
        glm::vec2(0, 0),
        Constants::leftGroundEnd,
        Constants::groundWidth));

    // Ox ground
    objects.push_back(new Obstacle2D(
        glm::vec2(Constants::exitHoleEnd, 0),
        resolution.x - Constants::exitHoleEnd,
        Constants::groundWidth));

    // Oy wall
    objects.push_back(new Obstacle2D(
        glm::vec2(resolution.x - Constants::wallLength, 0),
        Constants::wallLength,
        resolution.y));

    float x = Constants::leftGroundEnd + Constants::balloonRadius * 2;
    float y = -Constants::balloonRadius * 2;
    int delay = 2;
    glm::vec3 color;

    while (true) {
        int chance = (int)x % 96 + delay % 5;
        if (chance <= 50) {
            color = Constants::redBalloonColor;
        } else {
            color = Constants::yellowBalloonColor;
        }
        if (x >= Constants::exitHoleEnd - Constants::balloonRadius) break;

        float floatingSpeed = 100 + (int)x % 50 + (delay * 9) % 6;
        float spawnTime = (int)x % 9 + delay % 5; // seconds
        objects.push_back(new Balloon2D(glm::vec2(x, y), color,
            floatingSpeed, spawnTime,
            Constants::balloonRadius));

        x += Constants::balloonRadius;
        ++delay;
    }

    return Constants::level1Duration;
}

float LevelFactory::generateLevel2(glm::vec2 resolution, std::vector<Object2D*>& objects) {
    objects.push_back(new Obstacle2D(
        glm::vec2(resolution.x - Constants::wallLength, 0),
        Constants::wallLength,
        Constants::wallWidth));
    objects.push_back(new Obstacle2D(
        glm::vec2(resolution.x - Constants::wallLength, resolution.y - Constants::wallWidth),
        Constants::wallLength,
        Constants::wallWidth));

    float x = resolution.x + Constants::shurikenRadius * 2;
    float y = Constants::wallWidth + Constants::shurikenRadius * 2;
    int stars = 2;
    glm::vec3 color;

    int maxStars = 7;
    int delay = 2;
    while (true) {
        int chance = (int)y % 96 + delay % 5;
        if (chance <= 50) {
            color = glm::vec3(0.95f, 0.975f, 0.975f);
        } else {
            color = glm::vec3(0.25f, 0.25f, 0.25f);
        }
        if (y >= resolution.y - Constants::wallWidth) break;

        float speed = 100 + (int)y % 50 + (9 * delay) % 21;
        float angularSpeed = speed + 200 + (maxStars - 1 - (stars % maxStars)) * 50;
        float spawnTime = (int)y % 9 + delay % 5; // seconds
        objects.push_back(new Shuriken2D(glm::vec2(x, y), speed,
            angularSpeed,
            stars % maxStars,
            spawnTime,
            Constants::shurikenRadius,
            color));

        stars += (int)y % 32 + delay % 2;
        y += Constants::shurikenRadius;
        ++delay;
    }
    return Constants::level2Duration;
}

void LevelFactory::generateEndless(glm::vec2 resolution, std::vector<Object2D*>& objects) {
    objects.push_back(new Obstacle2D(
        glm::vec2(resolution.x - Constants::wallLength, 0),
        Constants::wallLength,
        Constants::wallWidth));
    objects.push_back(new Obstacle2D(
        glm::vec2(resolution.x - Constants::wallLength, resolution.y - Constants::wallWidth),
        Constants::wallLength,
        Constants::wallWidth));

    // Generate balloons
    {
        float x = Constants::leftGroundEnd + Constants::balloonRadius * 2;
        float y = -Constants::balloonRadius * 2;
        float levelDurationSeconds = 40;
        int delay = 2;
        glm::vec3 color;

        while (true) {
            int chance = (int)x % 96 + delay % 5;
            if (chance <= 50) {
                color = Constants::redBalloonColor;
            } else {
                color = Constants::yellowBalloonColor;
            }
            if (x >= Constants::exitHoleEnd - Constants::balloonRadius) break;

            float floatingSpeed = 100 + (int)x % 50 + (delay * 9) % 6;
            float spawnTime = (int)x % 9 + delay % 5; // seconds
            objects.push_back(new Balloon2D(glm::vec2(x, y), color,
                floatingSpeed, spawnTime,
                Constants::balloonRadius));

            x += Constants::balloonRadius;
            ++delay;
        }
    }

    // Generate shurikens
    {
        float x = resolution.x + Constants::shurikenRadius * 2;
        float y = Constants::wallWidth + Constants::shurikenRadius * 2;
        float levelDurationSeconds = 30;
        int stars = 2;
        glm::vec3 color;
        int maxStars = 7;

        int delay = 2;
        while (true) {
            int chance = (int)y % 96 + delay % 5;
            if (chance <= 50) {
                color = glm::vec3(0.95f, 0.975f, 0.975f);
            } else {
                color = glm::vec3(0.25f, 0.25f, 0.25f);
            }
            if (y >= resolution.y - Constants::wallWidth) break;

            float speed = 300 + (int)y % 50 + (9 * delay) % 21;
            float angularSpeed = speed + 200 + (maxStars - 1 - (stars % maxStars)) * 50;
            float spawnTime = (int)y % 9 + (delay * 3) % 11; // seconds
            objects.push_back(new Shuriken2D(glm::vec2(x, y), speed,
                angularSpeed,
                stars % maxStars,
                spawnTime,
                Constants::shurikenRadius,
                color));

            stars += (int)y % 32 + delay % 2;
            y += Constants::shurikenRadius * 4;
            ++delay;
        }
    }
}
