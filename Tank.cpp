#include "Tank.h"

#include <vector>

Tank::Tank() {

}

Tank::~Tank(){

}

Tank::Tank(glm::vec3 position, float angle = 0) {
	this->position = position + glm::vec3(0.f, (float)1 / 16,0.f);
	bodyAngle = angle;
	turretAngle = angle;
	cooldown = 3;
	HP = 3;

	Tank::UpdatebodyMatrix();
	Tank::UpdateturretMatrix();
	Tank::UpdateOffset();
}

void Tank::Move(float spd) {

	glm::vec2 movement = spd * Tank::GetBodyDirection();

	position.x += movement.x;
	position.z += movement.y;

	Tank::UpdatebodyMatrix();
	Tank::UpdateturretMatrix();
	Tank::UpdateOffset();
}

void Tank::RotateBody(float angle) {
	bodyAngle += angle;
	
	Tank::UpdatebodyMatrix();
	Tank::UpdateOffset();
}

void Tank::RotateTurret(float angle) {
	turretAngle += angle;

	Tank::UpdateturretMatrix();
	Tank::UpdateOffset();
}

void Tank::Displace(glm::vec3 distance) {
	position += distance;

	Tank::UpdatebodyMatrix();
	Tank::UpdateturretMatrix();
	Tank::UpdateOffset();
}

void Tank::UpdatebodyMatrix() {
	bodyMatrix = glm::mat4(1);
	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(0.5f, 0, (float)3 / 8)+position);
	bodyMatrix = glm::rotate(bodyMatrix, bodyAngle, glm::vec3(0, 1, 0));
	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(-0.5f, 0, (float)-3 / 8));
}

void Tank::UpdateturretMatrix() {
	turretMatrix = glm::mat4(1);
	turretMatrix = glm::translate(turretMatrix, glm::vec3(0.5f, 0, (float)3 / 8)+position);
	turretMatrix = glm::rotate(turretMatrix, turretAngle, glm::vec3(0, 1, 0));
	turretMatrix = glm::translate(turretMatrix, glm::vec3(-0.5f, 0, (float)-3 / 8));
}

void Tank::UpdateOffset() {
	offset= glm::vec2(-glm::cos(bodyAngle - turretAngle) / 16, glm::sin(bodyAngle - turretAngle) / 16);
}

void Tank::TakeDmg() {
	if(HP>0)
		HP--;
}

void Tank::Reload(float time) {
	if (cooldown > 0)
		cooldown -= time;
}

int Tank::GetHP() const{
	return HP;
}

float Tank::GetCooldown() const{
	return cooldown;
}

float Tank::GetTurretAngle() const {
	return turretAngle;
}

glm::vec2 Tank::GetBodyDirection() {
	return glm::vec2(glm::cos(bodyAngle), -glm::sin(bodyAngle));
}

glm::vec2 Tank::GetTurretOffset() const{
	return offset;
}

glm::vec3 Tank::GetPosition() const {
	return position;
}

void Tank::Fire(std::vector<Projectile> &projectiles) {

	if(cooldown <= 0){
		glm::vec3 directionBullet = glm::vec3(glm::cos(turretAngle), 0, -glm::sin(turretAngle));
		glm::vec3 positionBullet = position + glm::vec3(1.f / 2 - glm::cos(bodyAngle) / 16, 6.f / 16, 3.f / 8 + glm::sin(bodyAngle) / 16) + directionBullet * 3.f / 4.f;

		cooldown = 3;

		projectiles.push_back(Projectile(positionBullet, directionBullet));
	}
}

void Tank::RenderTank(Mesh* Body,Mesh* Tread, Mesh* Turret, Mesh* Barrel, Shader* shader, cam::Camera* camera, const glm::mat4& projectionMatrix) {

	glm::mat4 renderMatrix;
	
	{
		renderMatrix = glm::translate(turretMatrix, glm::vec3(1.f / 2 + offset.x, 6.f / 16, 3.f / 8 + offset.y));
		object3D::RenderFunction(Barrel, shader, camera, renderMatrix, projectionMatrix, glm::vec3(40.f/51),HP);
	}

	{
		renderMatrix = glm::translate(turretMatrix, glm::vec3(4.f / 16 + offset.x, 1.f / 4, 3.f / 16 + offset.y));
		object3D::RenderFunction(Turret, shader, camera, renderMatrix, projectionMatrix, glm::vec3(1.f/5,4.f/5,1.f/5),HP);
	}

	{
		object3D::RenderFunction(Body, shader, camera, bodyMatrix, projectionMatrix, glm::vec3(0, 20.f / 51, 0), HP);
	}

	{
		renderMatrix = glm::translate(bodyMatrix, glm::vec3(0, 0,1.f / 16));
		object3D::RenderFunction(Tread, shader, camera, renderMatrix, projectionMatrix, glm::vec3(40.f/51));
	}

	{
		renderMatrix = glm::translate(bodyMatrix, glm::vec3(0, 0, 11.f / 16));
		object3D::RenderFunction(Tread, shader, camera, renderMatrix, projectionMatrix, glm::vec3(40.f / 51));
	}
	//Treads remain white when the tank takes damage

}
