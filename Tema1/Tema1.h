#pragma once

#include "components/simple_scene.h"
#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void PlayerMovement(float deltaTime);
        void CalculatePlayerAngle();
        void SpawnPlayer(glm::mat3 visMatrix);
        void SpawnEnemy(glm::mat3 visMatrix, float deltaTime, float *posX, float *posY, float speed);
        void SpawnBullet(float deltaTime, glm::mat3 visMatrix);
        void DrawObstacles(glm::mat3 visMatrix);
        void DrawWalls(glm::mat3 visMatrix);
        void DrawHealthBar(glm::mat3 visMatrix);

        bool CheckCollision(float objOneX, float objOneY, float objOneSizeX, float objOneSizeY, float objTwoX, float objTwoY, float objTwoSizeX, float objTwoSizeY);

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float cx, cy;
        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        glm::mat3 modelMatrix1;
        glm::mat3 modelMatrix2;
        glm::mat3 modelMatrix3;
        float translateX, translateY;
        float translateEnemyX, translateEnemyY;
        float translateBulletX, translateBulletY;
        float translateHealthX, translateHealthY;
        float scaleHealthX;
        float angularStep;
        float speedX;
        float speedY;
        double mouseX;
        double mouseY;
        glm::ivec2 cursorPos;
        float playerAngle;
        float enemyAngle;
        float rotateSpeed;
        bool spawnBullet;
        bool shootBullet;
        glm::ivec2 currentCursorPos;
        glm::ivec2 currentCursorPos1;
        float bulletAngle;
        float bulletAngle1;
        float bulletSpeed;
        float enemySpeed;
        float spawnWalls[18] = { 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1050, 1200, 1300, 1400, 1500 };
        float obstacleX[8] = { 400, 400, 400, 400, 550, 400, 650, 650 };
        float obstacleY[8] = { 800, 850, 950, 500, 400, 1300, 1400, 1400 };
        float lifeTime;
        float posX[4], posY[4];
        float scaleX[8] = { 2, 1, 4, 4, 1, 4, 2, 1 };
        float scaleY[8] = { 1, 2, 1, 1, 1, 1, 1, 3 };
        float wallLimitX, wallLimitY;
        float playerOffsetX, playerOffsetY;
        float bulletVelocity;
        float enemyLifeTime;
        bool spawnEnemy;
        float speedEnemy[4];

    };
}   // namespace m1
