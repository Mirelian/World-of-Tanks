#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_camera.h"


namespace object3D
{
	void RenderFunction(Mesh* mesh, Shader* shader, cam::Camera* camera, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix, const glm::vec3& objColor, int HP=3);
	void CalculateNormals(std::vector<VertexFormat>& vertices, std::vector<unsigned int> indices);
	Mesh* CreateBuilding(const char* name);
	Mesh* CreatePrismBase(const char* name,float scaleXZ=1);
	Mesh* CreateTreads(const char* name);
	Mesh* CreateCylinder(const char* name,const int segments);
}