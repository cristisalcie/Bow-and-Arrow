#pragma once

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "../common/Constants.h"
#include "../objects2D/Object2D.h"
#include "../objects2D/Balloon2D.h"
#include "../objects2D/Shuriken2D.h"
#include "../objects2D/Obstacle2D.h"

namespace LevelFactory {
    // Returns level duration time.
    float generateNextLevel(int level, glm::vec2 resolution, std::vector<Object2D*>& objects);

    // Returns level duration time.
    float generateLevel1(glm::vec2 resolution, std::vector<Object2D*>& objects);

    // Returns level duration time.
    float generateLevel2(glm::vec2 resolution, std::vector<Object2D*>& objects);

    void generateEndless(glm::vec2 resolution, std::vector<Object2D*>& objects);
};
