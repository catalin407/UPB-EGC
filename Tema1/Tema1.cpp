#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;       // logic x
	logicSpace.y = 0;       // logic y
	logicSpace.width = resolution.x;   // logic width
	logicSpace.height = resolution.y;  // logic height

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	wallLimitX = 1080;
	wallLimitY = 1920;
	playerOffsetX = 75;
	playerOffsetY = 50;

	spawnBullet = false;
	spawnEnemy = false;

	speedX = 200;
	speedY = 200;
	lifeTime = -1;
	enemySpeed = 150;
	bulletSpeed = 0;
	bulletVelocity = 800;
	enemyLifeTime = 10;

	translateX = logicSpace.width / 2;
	translateY = logicSpace.height / 2;
	translateBulletX = logicSpace.width / 2;
	translateBulletY = logicSpace.height / 2;
	translateHealthX = window->GetResolution().x - 150;
	translateHealthY = window->GetResolution().y - 75;

	posX[0] = 300;
	posX[1] = 700;
	posX[2] = 800;
	posX[3] = 1000;
	posY[0] = 300;
	posY[1] = 1000;
	posY[2] = 1500;
	posY[3] = 100;

	speedEnemy[0] = rand() % 100 + 30;
	speedEnemy[1] = rand() % 100 + 30;
	speedEnemy[2] = rand() % 100 + 30;
	speedEnemy[3] = rand() % 100 + 30;

	scaleHealthX = 5;

	angularStep = 0;
	playerAngle = 0;
	enemyAngle = 0;

	Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);

	Mesh* bullet = object2D::CreateSquare("bullet", corner, squareSide, glm::vec3(0, 1, 1), true);
	AddMeshToList(bullet);

	Mesh* border = object2D::CreateSquare("border", corner, squareSide, glm::vec3(0, 0, 0), true);
	AddMeshToList(border);

	Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(square2);

	Mesh* health = object2D::CreateSquare("health", corner, squareSide / 2, glm::vec3(0, 1, 0), true);
	AddMeshToList(health);

	Mesh* healthBorder = object2D::CreateSquare("healthBorder", corner, squareSide / 2, glm::vec3(0, 1, 0), false);
	AddMeshToList(healthBorder);

	Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1), true);
	AddMeshToList(square3);

	Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0, 1, 1));
	AddMeshToList(square4);
}


void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	// The viewport is now the right half of the window

	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0.5f), true);

	// Compute uniform 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

	if (scaleHealthX > 0)
	{
		CalculatePlayerAngle();
		DrawScene(visMatrix);
		SpawnBullet(deltaTimeSeconds, visMatrix);
		SpawnEnemy(visMatrix, deltaTimeSeconds, &posX[0], &posY[0], speedEnemy[0]);
		SpawnEnemy(visMatrix, deltaTimeSeconds, &posX[1], &posY[1], speedEnemy[1]);
		SpawnEnemy(visMatrix, deltaTimeSeconds, &posX[2], &posY[2], speedEnemy[2]);
		SpawnEnemy(visMatrix, deltaTimeSeconds, &posX[3], &posY[3], speedEnemy[3]);
		SpawnPlayer(visMatrix);
	}
	else cout << "---------- GAME OVER! ----------\n";

}

void Tema1::DrawScene(glm::mat3 visMatrix)
{

	DrawObstacles(visMatrix);
	DrawWalls(visMatrix);
	DrawHealthBar(visMatrix);

}

void Tema1::DrawObstacles(glm::mat3 visMatrix)
{

	//Terrain

	//0
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[0], obstacleY[0]); // 400 800
	modelMatrix *= transform2D::Scale(scaleX[0], scaleY[0]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	//0
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[1], obstacleY[1]); // 400 850
	modelMatrix *= transform2D::Scale(scaleX[1], scaleY[1]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	//0
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[2], obstacleY[2]); // 400 950
	modelMatrix *= transform2D::Scale(scaleX[2], scaleY[2]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


	//1
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[3], obstacleY[3]); // 400 500
	modelMatrix *= transform2D::Scale(scaleX[3], scaleY[3]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	///1
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[4], obstacleY[4]); // 550 400
	modelMatrix *= transform2D::Scale(scaleX[4], scaleY[4]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	//2
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[5], obstacleY[5]); // 400 1300
	modelMatrix *= transform2D::Scale(scaleX[5], scaleY[5]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	//2
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[6], obstacleY[6]); // 650 1400
	modelMatrix *= transform2D::Scale(scaleX[6], scaleY[6]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	//2
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(obstacleX[7], obstacleY[7]); // 650 1400
	modelMatrix *= transform2D::Scale(scaleX[7], scaleY[7]);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

}

void Tema1::DrawWalls(glm::mat3 visMatrix)
{

	//Playfield Walls
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(0, 1920);
	modelMatrix *= transform2D::Scale(11.05, 0.25f);
	RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(0, 0);
	modelMatrix *= transform2D::Scale(11, 0.25f);
	RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(0, 0);
	modelMatrix *= transform2D::Scale(0.25f, 19.3f);
	RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(1080, 0);
	modelMatrix *= transform2D::Scale(0.25f, 19.3f);
	RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

}

void Tema1::DrawHealthBar(glm::mat3 visMatrix)
{

	//HealthBar
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(translateHealthX + 125, translateHealthY);
	modelMatrix *= transform2D::Scale(scaleHealthX, 1);
	modelMatrix *= transform2D::Translate(-50, -25);
	RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(translateHealthX, translateHealthY);
	modelMatrix *= transform2D::Scale(5, 1);
	modelMatrix *= transform2D::Translate(-25, -25);
	RenderMesh2D(meshes["healthBorder"], shaders["VertexColor"], modelMatrix);

}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	PlayerMovement(deltaTime);
}

void Tema1::SpawnPlayer(glm::mat3 visMatrix)
{
	//Body
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(translateX, translateY);
	modelMatrix *= transform2D::Scale(1, 1);
	modelMatrix *= transform2D::Rotate(playerAngle);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

	//Eye1
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(translateX, translateY);
	modelMatrix *= transform2D::Scale(0.25f, 0.25f);
	modelMatrix *= transform2D::Rotate(playerAngle);
	modelMatrix *= transform2D::Translate(150, 200);
	modelMatrix *= transform2D::Rotate(0);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	//Eye2
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(translateX, translateY);
	modelMatrix *= transform2D::Scale(0.25f, 0.25f);
	modelMatrix *= transform2D::Rotate(playerAngle);
	modelMatrix *= transform2D::Translate(-150, 200);
	modelMatrix *= transform2D::Rotate(0);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

}

bool Tema1::CheckCollision(float objOneX, float objOneY, float objOneSizeX, float objOneSizeY, float objTwoX, float objTwoY, float objTwoSizeX, float objTwoSizeY)
{

	bool collisionX = objOneX + objOneSizeX >= objTwoX &&
		objTwoX + objTwoSizeX >= objOneX;

	bool collisionY = objOneY + objOneSizeY >= objTwoY &&
		objTwoY + objTwoSizeY >= objOneY;

	/*if (objTwoX < objOneX + objOneSizeY &&
		objTwoX + objTwoSizeY > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.h + rect1.y > rect2.y)*/

	return collisionX && collisionY;

}

void Tema1::SpawnEnemy(glm::mat3 visMatrix, float deltaTime, float* posX, float* posY, float speed)
{

	enemyAngle = atan2(translateX - *posX, translateY - *posY);
	*posX += speed * sin(enemyAngle) * deltaTime;
	*posY += speed * cos(enemyAngle) * deltaTime;

	//Body
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(*posX, *posY);
	modelMatrix *= transform2D::Scale(1, 1);
	modelMatrix *= transform2D::Rotate(enemyAngle);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

	//Eye1
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(*posX, *posY);
	modelMatrix *= transform2D::Scale(0.25f, 0.25f);
	modelMatrix *= transform2D::Rotate(enemyAngle);
	modelMatrix *= transform2D::Translate(150, 200);
	modelMatrix *= transform2D::Rotate(0);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	//Eye2
	modelMatrix = visMatrix;
	modelMatrix *= transform2D::Translate(*posX, *posY);
	modelMatrix *= transform2D::Scale(0.25f, 0.25f);
	modelMatrix *= transform2D::Rotate(enemyAngle);
	modelMatrix *= transform2D::Translate(-150, 200);
	modelMatrix *= transform2D::Rotate(0);
	modelMatrix *= transform2D::Translate(-50, -50);
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

}

void Tema1::SpawnBullet(float deltaTime, glm::mat3 visMatrix)
{
	if (spawnBullet == true)
	{

		if (lifeTime > 0)
		{
			bulletSpeed += bulletVelocity * deltaTime;

			//Bullet
			modelMatrix1 = visMatrix;
			modelMatrix1 *= transform2D::Translate(bulletSpeed * cos(bulletAngle), bulletSpeed * sin(bulletAngle));
			modelMatrix1 *= transform2D::Translate(translateBulletX, translateBulletY);
			modelMatrix1 *= transform2D::Rotate(bulletAngle1);
			modelMatrix1 *= transform2D::Scale(0.25f, 0.5f);
			modelMatrix1 *= transform2D::Translate(0, 0);
			modelMatrix1 *= transform2D::Rotate(0);
			modelMatrix1 *= transform2D::Translate(-50, -50);
			RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix1);

			lifeTime -= deltaTime;
		}

	}

	if (lifeTime < 0)
	{
		spawnBullet = false;
	}


}


void Tema1::PlayerMovement(float deltaTime)
{

	if (window->KeyHold(GLFW_KEY_A))
	{
		bool collision = false;

		for (int i = 0; i < 8; i++)
			if (CheckCollision(translateX - playerOffsetX, translateY, 50, 50, obstacleX[i], obstacleY[i], scaleX[i] * 100, scaleY[i] * 100))
			{
				collision = true;
				break;
			}

		if (translateX - playerOffsetX > 0 && collision == false)
		{

			translateX -= deltaTime * speedX;
			logicSpace.x -= deltaTime * speedX;
			translateHealthX -= deltaTime * speedX;

		}

	}

	if (window->KeyHold(GLFW_KEY_D))
	{
		bool collision = false;

		for (int i = 0; i < 8; i++)
			if (CheckCollision(translateX + playerOffsetY, translateY, 50, 50, obstacleX[i], obstacleY[i], scaleX[i] * 100, scaleY[i] * 100))
			{
				//cout << "Collision" << endl;
				collision = true;
				break;
			}

		if (translateX + playerOffsetY <= wallLimitX && collision == false)
		{

			translateX += deltaTime * speedX;
			logicSpace.x += deltaTime * speedX;
			translateHealthX += deltaTime * speedX;

		}
	}

	if (window->KeyHold(GLFW_KEY_W))
	{

		bool collision = false;

		for (int i = 0; i < 8; i++)
			if (CheckCollision(translateX, translateY + playerOffsetY, 50 , 50, obstacleX[i], obstacleY[i], scaleX[i] * 100, scaleY[i] * 100))
			{
				//cout << "Collision" << endl;
				collision = true;
				break;
			}

		if (translateY + playerOffsetY <= wallLimitY && collision == false)
		{

			translateY += deltaTime * speedY;
			logicSpace.y += deltaTime * speedY;
			translateHealthY += deltaTime * speedY;

		}
	}

	if (window->KeyHold(GLFW_KEY_S))
	{

		bool collision = false;

		for (int i = 0; i < 8; i++)
			if (CheckCollision(translateX, translateY - playerOffsetY, 50, 50, obstacleX[i], obstacleY[i], scaleX[i] * 100, scaleY[i] * 100))
			{
				//cout << "Collision" << endl;
				collision = true;
				break;
			}

		if (translateY - playerOffsetY >= 0 && collision == false)
		{

			translateY -= deltaTime * speedY;
			logicSpace.y -= deltaTime * speedY;
			translateHealthY -= deltaTime * speedY;

		}
	}

}

void Tema1::CalculatePlayerAngle()
{

	cursorPos = window->GetCursorPosition();

	playerAngle = -atan2(-(logicSpace.x + cursorPos.x - translateX) * playerOffsetY, playerOffsetY * (logicSpace.y + window->GetResolution().y - cursorPos.y - translateY));

}

void Tema1::OnKeyPress(int key, int mods)
{

	if (key == GLFW_KEY_SPACE)
		if (scaleHealthX > 0 && scaleHealthX <= 5)
			scaleHealthX -= 1;

}


void Tema1::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && lifeTime < 0)
	{
		spawnBullet = true;
		translateBulletX = translateX;
		translateBulletY = translateY;
		currentCursorPos = cursorPos;
		bulletAngle1 = playerAngle;
		bulletSpeed = 0;
		lifeTime = 1;
		bulletAngle = atan2(logicSpace.y + (window->GetResolution().y - currentCursorPos.y) - translateBulletY, logicSpace.x + currentCursorPos.x - translateBulletX);
	}

}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}
