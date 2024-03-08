#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace collision {
    inline bool Tank_Projectile(glm::vec3 tankPosition, glm::vec3 projectilePosition, float tankScale = 1) {
        glm::vec3 tankCenter = tankPosition + glm::vec3(1.f / 2, 0, 3.f / 8) * tankScale;
        float distance = glm::sqrt(glm::pow(tankCenter.x - projectilePosition.x, 2) +
            glm::pow(tankCenter.y - projectilePosition.y, 2) +
            glm::pow(tankCenter.z - projectilePosition.z, 2));

        return (distance < (5.f / 8 * tankScale + 1.f / 16));
    }

    inline bool Building_Projectile(glm::vec3 projectilePosition, glm::vec3 buildingPosition, glm::vec3 buildingScale) {
        float x = glm::max(buildingPosition.x, glm::min(projectilePosition.x, buildingPosition.x + buildingScale.x));
        float y = glm::max(buildingPosition.y, glm::min(projectilePosition.y, buildingPosition.y + buildingScale.y));
        float z = glm::max(buildingPosition.z, glm::min(projectilePosition.z, buildingPosition.z + buildingScale.z));

        float distance = glm::sqrt(glm::pow(x - projectilePosition.x, 2) +
            glm::pow(y - projectilePosition.y, 2) +
            glm::pow(z - projectilePosition.z, 2));

        return (distance < 1.f / 16);
    }

    inline glm::vec3 Tank_Tank(glm::vec3 tank1Position, glm::vec3 tank2Position, float tankScale = 1) {
        glm::vec3 tank1Center = tank1Position + glm::vec3(1.f / 2, 0, 3.f / 8) * tankScale;
        glm::vec3 tank2Center = tank2Position + glm::vec3(1.f / 2, 0, 3.f / 8) * tankScale;

        float distance = glm::sqrt(glm::pow(tank1Center.x - tank2Center.x, 2) +
            glm::pow(tank1Center.z - tank2Center.z, 2));

        glm::vec3 Dif = tank2Center - tank1Center;
        float absP = 5.f / 4 * tankScale - distance;    //if absP>0 results collision
        return glm::vec3(glm::normalize(glm::vec2(Dif.x,Dif.z)),absP);

    }

    inline glm::vec3* Building_Tank(glm::vec3 tankPosition, glm::vec3 buildingPosition, glm::vec3 buildingScale, float tankScale = 1) {
        glm::vec3 tankCenter = tankPosition + glm::vec3(1.f / 2, 0, 3.f / 8) * tankScale;

        glm::vec3 box(0);

        box.x = glm::max(buildingPosition.x, glm::min(tankCenter.x, buildingPosition.x + buildingScale.x));
        box.y = glm::max(buildingPosition.y, glm::min(tankCenter.y, buildingPosition.y + buildingScale.y));
        box.z = glm::max(buildingPosition.z, glm::min(tankCenter.z, buildingPosition.z + buildingScale.z));

        float distance = glm::sqrt(glm::pow(box.x - tankCenter.x, 2) +
            glm::pow(box.y - tankCenter.y, 2) +
            glm::pow(box.z - tankCenter.z, 2));

        if (!(distance < 5.f / 8))  //No collision
            return NULL;
        else {
            glm::vec3 Dif = box - tankCenter;
            float absP = 5.f / 8 * tankScale - distance;
            return new glm::vec3(absP * glm::normalize(Dif));
        }
    }

}