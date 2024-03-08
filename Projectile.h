#pragma once

#include "utils/glm_utils.h"

class Projectile {
public:
    Projectile();
    ~Projectile();
    Projectile(glm::vec3 position, glm::vec3 direction);
    void Move(float spd);
    float GetLifeTime() const;
    glm::vec3 GetPosition() const;

private:
    glm::vec3 position;
    glm::vec3 direction;
    float lifeTime;
};
