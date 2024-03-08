#pragma once

#include <components/simple_scene.h>
#include <utils/glm_utils.h>
#include <core/engine.h>

#include "object3D.h"
#include "Projectile.h"

class Tank {
	public:
		Tank();
		~Tank();
		Tank(glm::vec3 position, float angle);
		void Move(float spd);
		void RotateBody(float angle);
		void RotateTurret(float angle);
		void Displace(glm::vec3 distance);
		void TakeDmg();
		void Reload(float time);
		void UpdatebodyMatrix();
		void UpdateturretMatrix();
		void UpdateOffset();
		int GetHP() const;
		float GetCooldown() const;
		float GetTurretAngle() const;
		glm::vec2 GetTurretOffset() const;
		glm::vec2 GetBodyDirection();
		glm::vec3 GetPosition() const;

		void Fire(std::vector<Projectile>&);

		void RenderTank(Mesh* Body, Mesh* Tread, Mesh* Turret, Mesh* Barrel, Shader* shader, cam::Camera* camera, const glm::mat4& projectionMatrix);
		//Multi-Mesh Render function

	private:
		glm::vec2 offset;
		glm::vec3 position;
		glm::mat4 bodyMatrix;
		glm::mat4 turretMatrix;
		float bodyAngle;
		float turretAngle;
		float cooldown;
		int HP;
};
