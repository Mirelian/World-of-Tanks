#include "object3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

void object3D::RenderFunction(Mesh* mesh, Shader* shader, cam::Camera* camera, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix, const glm::vec3& objColor, int HP) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int location;

	location = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "frag_color");
	glUniform3fv(location, 1, glm::value_ptr(objColor));

	location = glGetUniformLocation(shader->program, "eye_position");
	glm::vec3 eyePosition = camera->GetTargetPosition();
	glUniform3f(location, eyePosition.x, eyePosition.y, eyePosition.z);

	location = glGetUniformLocation(shader->program, "HP");
	glUniform1i(location, HP);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void object3D::CalculateNormals(std::vector<VertexFormat>& vertices, std::vector<unsigned int> indices) {
	for (int i = 0;i < indices.size()-2; i ++) {

		vertices[indices[i]].color=glm::vec3(0);
		vertices[indices[i+1]].color =glm::vec3(0);
		vertices[indices[i+2]].color=glm::vec3(0);

		glm::vec3 v1 = vertices[indices[i+1]].position - vertices[indices[i]].position;
		glm::vec3 v2 = vertices[indices[i+2]].position - vertices[indices[i]].position;
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		vertices[indices[i]].color += normal;
		vertices[indices[i+1]].color += normal;
		vertices[indices[i+2]].color += normal;
	}

	for (int i = 0;i < vertices.size();++i) {
		vertices[i].color = glm::normalize(vertices[i].color);
	}

}


Mesh* object3D::CreateBuilding(const char* name)
{

	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(0, 0,  1)),
		VertexFormat(glm::vec3(1, 0,  1)),
		VertexFormat(glm::vec3(1, 0, 0)),
		//bottom square

		VertexFormat(glm::vec3(1,  1, 0)),
		VertexFormat(glm::vec3(0,  1, 0)),
		VertexFormat(glm::vec3(0,  1,  1)),
		VertexFormat(glm::vec3(1,  1,  1)),
		//top square
	};

	std::vector<unsigned int> indices =
	{
		3,2,0,1,
		//bottom face
		6,2,7,3,
		4,0,5,6,
		//sides
		4,7
		//top face
	};

	object3D::CalculateNormals(vertices, indices);

	Mesh* building = new Mesh(name);
	building->SetDrawMode(GL_TRIANGLE_STRIP);
	building->InitFromData(vertices, indices);
	return building;

}

Mesh* object3D::CreatePrismBase(const char* name,float sclaeXZ)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0,  0)),
		VertexFormat(glm::vec3(0, 0,  (float)3 / 4* sclaeXZ)),
		VertexFormat(glm::vec3(sclaeXZ,  0, (float)3 / 4* sclaeXZ)),
		VertexFormat(glm::vec3(sclaeXZ,  0,  0)),
		//bottom rectangle

		VertexFormat(glm::vec3((float)3 / 4 * sclaeXZ, (float)1 / 4, (float)1 / 8 * sclaeXZ)),
		VertexFormat(glm::vec3((float)1 / 8 * sclaeXZ, (float)1 / 4, (float)1 / 8 * sclaeXZ)),
		VertexFormat(glm::vec3((float)1 / 8 * sclaeXZ, (float)1 / 4, (float)5 / 8 * sclaeXZ)),
		VertexFormat(glm::vec3((float)3 / 4 * sclaeXZ, (float)1 / 4, (float)5 / 8 * sclaeXZ))
		//top smaller rectangle
	};

	std::vector<unsigned int> indices =
	{
		3,2,0,1,
		//bottom face
		6,2,7,3,
		4,0,5,6,
		//sides
		4,7
		//top face
	};

	object3D::CalculateNormals(vertices, indices);

	Mesh* prismBase = new Mesh(name);
	prismBase->SetDrawMode(GL_TRIANGLE_STRIP);
	prismBase->InitFromData(vertices, indices);
	return prismBase;
}

Mesh* object3D::CreateTreads(const char* name)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3((float)-1 / 32, (float)-1 / 32,  (float)-1 / 32)),
		VertexFormat(glm::vec3((float)-1 / 32, (float)-1 / 32,  (float)1 / 32)),
		VertexFormat(glm::vec3((float)-1 / 32, (float)1 / 32,  (float)1 / 32)),
		VertexFormat(glm::vec3((float)-1 / 32, (float)1 / 32,  (float)-1 / 32)),

		VertexFormat(glm::vec3(0, (float)-1 / 16,  (float)-1 / 16)),
		VertexFormat(glm::vec3(0, (float)-1 / 16,  (float)1 / 16)),
		VertexFormat(glm::vec3(0, (float)1 / 16,  (float)1 / 16)),
		VertexFormat(glm::vec3(0, (float)1 / 16,  (float)-1 / 16)),

		VertexFormat(glm::vec3(1, (float)-1 / 16,  (float)-1 / 16)),
		VertexFormat(glm::vec3(1, (float)-1 / 16,  (float)1 / 16)),
		VertexFormat(glm::vec3(1, (float)1 / 16,  (float)1 / 16)),
		VertexFormat(glm::vec3(1, (float)1 / 16,  (float)-1 / 16)),

		VertexFormat(glm::vec3((float)33/ 32, (float)-1 / 32,  (float)-1 / 32)),
		VertexFormat(glm::vec3((float)33/ 32, (float)-1 / 32,  (float)1 / 32)),
		VertexFormat(glm::vec3((float)33/ 32, (float)1 / 32,  (float)1 / 32)),
		VertexFormat(glm::vec3((float)33/ 32, (float)1 / 32,  (float)-1 / 32)),
	};

	std::vector<unsigned int> indices =
	{
		2,3,1,0,

		4,3,7,2,6,1,5,4,

		8,7,11,6,10,5,9,8,

		12,11,15,10,14,9,13,12,

		14,15
	};

	object3D::CalculateNormals(vertices, indices);

	Mesh* tread = new Mesh(name);
	tread->SetDrawMode(GL_TRIANGLE_STRIP);
	tread->InitFromData(vertices, indices);
	return tread;
}

Mesh* object3D::CreateCylinder(const char* name,const int segments)
{
	std::vector<VertexFormat> vertices;

	for (int i = 0;i < segments;i++) {
		float theta = 2.f * i * glm::pi<float>()/segments;
		float z = std::cos(theta) / 16.f;
		float y = std::sin(theta) / 16.f;

		vertices.push_back(VertexFormat(glm::vec3(0, y, z)));
	}
	//front circle

	for (int i = 0;i < segments;i++) {
		float theta = 2.f * i * glm::pi<float>() / segments;
		float z = std::cos(theta) / 16.f;
		float y = std::sin(theta) / 16.f;

		vertices.push_back(VertexFormat(glm::vec3((float)3/4, y, z)));
	}
	//back circle

	std::vector<unsigned int> indices;

	indices.push_back(0);

	for (int i = 1;i <= (segments-1)/2;i++) {
		indices.push_back(i);
		indices.push_back(segments - i);
	}

	indices.push_back(segments / 2);
	//front circle indices

	int poz = segments + segments % 2 - 1;
	int front_index = indices[poz] + segments;
	int back_index = indices[poz] -1;

	for (int i = 0;i < segments-1;i++) {
		indices.push_back(front_index);
		indices.push_back(back_index);

		if (--front_index < segments)
			front_index = 2*segments-1;
		if (--back_index < 0)
			back_index = segments-1;
	}

	indices.push_back(--front_index);
	//cylinder walls indices

	for (int i = 1;i <= (segments - 1) / 2;i++) {
		indices.push_back(front_index-i);
		if (front_index + i > 2 * segments - 1)
			break;
		indices.push_back(front_index+i);
	}

	if (segments % 2 == 0)
		indices.push_back(segments);

	indices.push_back(segments + 1);
	//back circle indices

	object3D::CalculateNormals(vertices, indices);

	Mesh* cylinder = new Mesh(name);
	cylinder->SetDrawMode(GL_TRIANGLE_STRIP);
	cylinder->InitFromData(vertices, indices);
	return cylinder;
}