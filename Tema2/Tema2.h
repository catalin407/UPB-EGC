#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/Tema2/maze.h"
#include "lab_m1/Tema2/Player.h"
#include "lab_m1/Tema2/proiectil.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene, public Maze
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderMaze();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderPlayer();
        void RenderEnemy(float deltaTime);
        void RenderProiectil(float deltaTime);
        void drawHealth(float health);
        bool Collision(Player player, float bMinX, float bMinY, float bMinZ, float bMaxX, float bMaxY, float bMaxZ);

    protected:
        GLenum polygonMode;
        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float cameraSpeed;
        bool projectionMode;
        float FOV;
        float playerAngle;
        int number;
        float enemyPos[6][2], enemyPosC[6][2];
        Maze labrth;
        Player player;
        Player enemy[6];
        int enemyMove;
        Proiectil proiectil;
        float nearZ, farZ;
        int k;
    };
}   // namespace m1
