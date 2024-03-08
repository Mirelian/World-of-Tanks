#include "World of Tanks.h"

#include <vector>
#include <iostream>
#include <cstdlib>


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


WOT::WOT()
{
}


WOT::~WOT()
{
}


void WOT::Init()
{

    srand((unsigned)time(NULL));

    camera = new cam::Camera();
    camera->Set(glm::vec3(-1.5f-5.f, 1.f, 3.f/8), glm::vec3(100, 0, 0), glm::vec3(0, 1, 0));

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    {
        meshes["building"] = object3D::CreateBuilding("building");
    }

    {
        meshes["body"] = object3D::CreatePrismBase("body");
    }

    {
        meshes["turret"] = object3D::CreatePrismBase("turret",0.5f);
    }

    {
        meshes["tread"] = object3D::CreateTreads("tread");
    }

    {
        meshes["cylinder"] = object3D::CreateCylinder("cylinder",16);
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("ShaderT2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    mainTank = new Tank(glm::vec3(-5.f, 0, 0), 0.f);

    for (int i = 0;i < 5;i++) {
        tanks[i] = new Tank(glm::vec3(15.f + fmod(rand(), 5.f), 0, -10 + 3 * i + fmod(rand(), 5.f)), glm::pi<float>());
    }

    buildings = new Building[8];

    for (int i = 0;i < 8;i++) {
        buildings[i].position = glm::vec3(fmod(rand(),10.f)-4.f,0,fmod(rand(),20.f)-10.f);
        buildings[i].scale = glm::vec3(fmod(rand(), 2.f) + 0.5f, fmod(rand(), 2.f) + 1.5f, fmod(rand(), 2.f) + 0.5f);
    }

}


void WOT::FrameStart()
{

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);

    glFlush();
}


void WOT::Update(float deltaTimeSeconds)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (fmod(Engine::GetElapsedTime(), 5.f) < 0.1f) {
        for (int i = 0;i < 5;i++) {
            if (glm::abs(bodyChange[i]) <= 0.5f)
                bodyChange[i] = fmod(rand(), 60) - 30;
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0,0));
        object3D::RenderFunction(meshes["plane"], shaders["ShaderT2"], camera, modelMatrix, projectionMatrix, glm::vec3(1.f / 2, 13.f / 16, 15.f / 32));
    }

    for (int i = 0;i < projectiles.size();i++)
    {
        projectiles[i].Move(deltaTimeSeconds);
        if (projectiles[i].GetLifeTime() <= 0){
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }
        if (collision::Tank_Projectile(mainTank->GetPosition(), projectiles[i].GetPosition())) {
            projectiles.erase(projectiles.begin() + i);
            mainTank->TakeDmg();
            i--;
            continue;
        }
    }

    for (int i = 0;i < 5;i++) {
        for (int j = 0;j < projectiles.size();j++) {
            if (collision::Tank_Projectile(tanks[i]->GetPosition(), projectiles[j].GetPosition())) {
                projectiles.erase(projectiles.begin() + j);
                tanks[i]->TakeDmg();
                j--;
                continue;
            }
        }

        for (int j = i + 1;j < 5;j++) {
            glm::vec3 P = collision::Tank_Tank(tanks[i]->GetPosition(), tanks[j]->GetPosition());

            if (P.z>0) {
                tanks[i]->Displace(P.z*glm::vec3(P.x, 0.f, P.y) * -0.5f);
                tanks[j]->Displace(P.z*glm::vec3(P.x, 0.f, P.y) * 0.5f);
            }
        }

        if(tanks[i]->GetHP()>0) {
            if (glm::abs(bodyChange[i]) > 0.5f) {
                tanks[i]->RotateBody(glm::sign(bodyChange[i])*deltaTimeSeconds / 8);
                bodyChange[i] -= glm::degrees(glm::sign(bodyChange[i]) * deltaTimeSeconds / 8);
            }

            glm::vec3 position = tanks[i]->GetPosition();

            if ((position.x < -10 || position.x>20) && bodyChange[i] <= 0.5f)
                bodyChange[i] += 180;
            else if((position.z < -10 || position.z>10) && bodyChange[i]<=0.5f)
                bodyChange[i] += 180;

            if(bodyChange[i]<=30)
                tanks[i]->Move(deltaTimeSeconds / 2);
        }

        if (glm::distance(mainTank->GetPosition(), tanks[i]->GetPosition()) <= 15.f) {

            glm::vec3 P = collision::Tank_Tank(mainTank->GetPosition(), tanks[i]->GetPosition());
            float angleChange = glm::pi<float>() - glm::atan(P.y, P.x) - tanks[i]->GetTurretAngle();

            if (tanks[i]->GetHP() > 0) {
                if (glm::abs(angleChange) > 0.01f)
                    tanks[i]->RotateTurret(glm::sign(angleChange) * deltaTimeSeconds / 4);
                else
                    tanks[i]->Fire(projectiles);
            }

            if (P.z > 0) {
                mainTank->Displace(P.z * glm::vec3(P.x, 0.f, P.y) * -0.5f);
                camera->Displace(P.z * glm::vec3(P.x, 0.f, P.y) * -0.5f);
                tanks[i]->Displace(P.z * glm::vec3(P.x, 0.f, P.y) * 0.5f);
            }
        }

    }

    for (int i = 0;i < 8;i++) {
        for (int j = 0;j < projectiles.size();j++) {
            if (collision::Building_Projectile(projectiles[j].GetPosition(),buildings[i].position,buildings[i].scale)) {
                projectiles.erase(projectiles.begin() + j);
                j--;
                continue;
            }
        }

        for (int j = 0;j < 5;j++) {
            glm::vec3* P = collision::Building_Tank(tanks[j]->GetPosition(), buildings[i].position, buildings[i].scale);
            if (P) {
                tanks[j]->Displace(*P * -0.5f);
            }
            delete P;
        }

        {
            glm::vec3* P = collision::Building_Tank(mainTank->GetPosition(), buildings[i].position, buildings[i].scale);
            if (P) {
                mainTank->Displace(*P * -0.5f);
                camera->Displace(*P * -0.5f);
            }
            delete P;
        }

    }

    mainTank->RenderTank(meshes["body"], meshes["tread"], meshes["turret"], meshes["cylinder"], shaders["ShaderT2"], camera, projectionMatrix);
    mainTank->Reload(deltaTimeSeconds);

    for (int i = 0;i < 5;i++) {
        tanks[i]->RenderTank(meshes["body"], meshes["tread"], meshes["turret"], meshes["cylinder"], shaders["ShaderT2"], camera, projectionMatrix);
        tanks[i]->Reload(deltaTimeSeconds);
    }

    for (int i = 0;i < projectiles.size();i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectiles[i].GetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        object3D::RenderFunction(meshes["sphere"], shaders["ShaderT2"], camera, modelMatrix, projectionMatrix, glm::vec3(0));
    }

    for (int i = 0;i < 8;i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildings[i].position);
        modelMatrix = glm::scale(modelMatrix, buildings[i].scale);
        object3D::RenderFunction(meshes["building"], shaders["ShaderT2"], camera, modelMatrix, projectionMatrix, glm::vec3(1));
    }

    glDisable(GL_CULL_FACE);
}


void WOT::FrameEnd()
{
    if (Engine::GetElapsedTime() >= 90)
        exit(0);
}

void WOT::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec2 body = mainTank->GetBodyDirection();
    glm::vec2 offset = mainTank->GetTurretOffset();

    if(mainTank->GetHP()>0)
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            mainTank->Move(deltaTime / 2);
            camera->MoveForward((body.x * camera->right.z - body.y * camera->right.x) * deltaTime / 2);
            camera->TranslateRight((body.y * camera->right.z + body.x * camera->right.x) * deltaTime / 2);
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            mainTank->Move(-deltaTime / 4);
            camera->MoveForward((body.x * camera->right.z - body.y * camera->right.x) * -deltaTime / 4);
            camera->TranslateRight((body.y * camera->right.z + body.x * camera->right.x) * -deltaTime / 4);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            float prev_offset_y = offset.y;
            mainTank->RotateBody(-deltaTime);
            offset = mainTank->GetTurretOffset();
            camera->TranslateRight(offset.y - prev_offset_y);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            float prev_offset_y = offset.y;
            mainTank->RotateBody(deltaTime);
            offset = mainTank->GetTurretOffset();
            camera->TranslateRight(offset.y - prev_offset_y);
        }
    }

}


void WOT::OnKeyPress(int key, int mods)
{
}


void WOT::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void WOT::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

            camera->RotateThirdPerson_OX(-sensivityOY * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
            mainTank->RotateTurret(-sensivityOX * deltaX);

    }
}


void WOT::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (mainTank->GetHP() > 0){
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
            mainTank->Fire(projectiles);
    }
}


void WOT::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void WOT::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void WOT::OnWindowResize(int width, int height)
{
}
