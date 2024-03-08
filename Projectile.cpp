#include "Projectile.h"

Projectile::Projectile(){
	
}

Projectile::~Projectile() {

}

Projectile::Projectile(glm::vec3 position, glm::vec3 direction) {
	this->position = position;
	this->direction = direction;
	lifeTime = 6;
}

void Projectile::Move(float spd) {
	position += 3.f*spd * direction;
	lifeTime -= spd;
}

float Projectile::GetLifeTime() const{
	return lifeTime;
}


glm::vec3 Projectile::GetPosition() const{
	return position;
}
