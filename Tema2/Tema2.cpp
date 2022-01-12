#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/transformTema.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}


void Tema2::Init()
{
	FOV = 60.0f;
	farZ = 200.0f;
	nearZ = 0.01f;
	k = 0;

	polygonMode = GL_FILL;
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
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

	number = rand() % 5 + 1;
	labrth = Maze(number);
	labrth.GenerateStartingPoint();
	cout << labrth.maze[labrth.startX][labrth.startZ] << endl;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (labrth.maze[i][j] == 2)
			{
				enemyPos[k][0] = i;
				enemyPos[k][1] = j;
				enemyPosC[k][0] = 0;
				enemyPosC[k][1] = 0;
				k = k + 1;
			}

	player = Player();
	for (int i = 0; i < 6; i++)
		enemy[i] = Player();

	proiectil = Proiectil();

	camera = new implemented::Camera();
	camera->Set(glm::vec3(labrth.startX + 0.3f, 0.5f, labrth.startZ + 2.5f), glm::vec3(labrth.startX + 0.3f, 1, labrth.startZ), glm::vec3(0, 1, 0));

	cameraSpeed = 5;
	playerAngle = 0.0f;
	enemyMove = 0;

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("TemaShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}

	projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, nearZ, farZ);

}

void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sets the screen area where to draw
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	if (player.thirdPerson == true)
		RenderPlayer();

	RenderProiectil(deltaTimeSeconds);
	RenderEnemy(deltaTimeSeconds);
	RenderMaze();
	player.timeV -= deltaTimeSeconds * 0.3;

}

void Tema2::RenderPlayer()
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.75f, 0));
	modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
	modelMatrix *= transformTema::RotateOY(playerAngle);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));

	player.playerBody = modelMatrix;

	player.SpawnPlayer();

	player.minX = player.hitboxX;
	player.minY = player.hitboxY;
	player.minZ = player.hitboxZ;
	player.maxX =  player.hitboxX;
	player.maxY = player.hitboxY;
	player.maxZ = player.hitboxZ;

	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerBody, player.playerBodyColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerArmL, player.playerArmColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerHandL, player.playerHandColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerArmR, player.playerArmColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerHandR, player.playerHandColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerHead, player.playerHeadColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerLegL, player.playerLegColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerLegR, player.playerLegColor);
	RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerHealth, glm::vec3(0.976, 0.003, 0.003));
	if (player.timeV > 0)
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.time, glm::vec3(0, 0, 1));
	//RenderSimpleMesh(meshes["box"], shaders["TemaShader"], player.playerHitbox, glm::vec3(0.976, 0.003, 0.003)); 
}

bool Tema2::Collision(Player player, float bMinX, float bMinY, float bMinZ, float bMaxX, float bMaxY, float bMaxZ)
{

	if (abs(camera->GetTargetPosition().x - bMinX) < player.hitboxX + bMaxX)
	{
		//check the Y axis
		if (abs(camera->GetTargetPosition().y - bMinY) < player.hitboxY + bMaxY)
		{
			//check the Z axis
			if (abs(camera->GetTargetPosition().z - bMinZ) < player.hitboxZ + bMaxZ)
			{
				return true;
			}
		}
	}

	return false;

}

void Tema2::RenderProiectil(float deltaTime)
{
	if (proiectil.shoot == true && player.firstPerson == true)
	{
		if (proiectil.lifeTime > 0.0f)
		{
			proiectil.speed += proiectil.velocity * deltaTime;

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, proiectil.speed * proiectil.move);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.2f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(proiectil.spawnPos.x, proiectil.spawnPos.y, proiectil.spawnPos.z));
			modelMatrix *= transformTema::RotateOY(proiectil.proiectilAngle);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(.2f));
			RenderSimpleMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.050, 0.050, 0.050));
			proiectil.lifeTime -= deltaTime;
		}

		if (proiectil.lifeTime < 0.0f)
			proiectil.shoot = false;
	}

}


void Tema2::RenderEnemy(float deltaTime)
{

	for (int i = 0; i < 6; i++)
	{
		if (enemyMove == 0)
		{
			enemyPosC[i][0] += deltaTime * 0.3;
			if (enemyPosC[i][0] >= 0.5f) {
				enemyMove = 1;
			}
		}
		if (enemyMove == 1)
		{
			enemyPosC[i][0] -= deltaTime * 0.3;
			if (enemyPosC[i][0] <= -0.5f) {
				enemyMove = 0;
			}
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyPosC[i][0], .0f, enemyPosC[i][1]));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(enemyPos[i][0], .4f, enemyPos[i][1]));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));

		enemy[i].playerBody = modelMatrix;

		enemy[i].SpawnPlayer();

		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerBody, glm::vec3(0.678, 0, 0));
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerArmL, glm::vec3(0.678, 0, 0));
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerHandL, enemy[i].playerHandColor);
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerArmR, glm::vec3(0.678, 0, 0));
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerHandR, enemy[i].playerHandColor);
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerHead, enemy[i].playerHeadColor);
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerLegL, glm::vec3(0.490, 0.011, 0.729));
		RenderSimpleMesh(meshes["box"], shaders["TemaShader"], enemy[i].playerLegR, glm::vec3(0.490, 0.011, 0.729));
	}

}

void Tema2::RenderMaze()
{

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.f));
		RenderSimpleMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0, .755, 0));

	}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (labrth.maze[i][j] == 1)
			{
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0.5f, j));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(labrth.mazeX, labrth.mazeY, labrth.mazeZ));
					RenderSimpleMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.439, 0.345, 0.0395));
				}

			}

}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO(student): Set light position uniform

	int locLightPos = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(locLightPos, 1, glm::value_ptr(lightPosition));


	glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
	// TODO(student): Set eye position (camera position) uniform
	//glm::vec3 eyePosition = GetSceneCamera()->get
	int locEyePos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(locEyePos, 1, glm::value_ptr(eyePosition));

	// TODO(student): Set material property uniforms (shininess, kd, ks, object color)
	int locMaterial = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(locMaterial, materialShininess);

	int locMaterialKd = glGetUniformLocation(shader->program, "material_kd");  // diffuse light
	glUniform1f(locMaterialKd, materialKd);

	int locMaterialKs = glGetUniformLocation(shader->program, "material_ks");  // specular light
	glUniform1f(locMaterialKs, materialKs);

	int locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}
void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// Render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::FrameEnd()
{
	//DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

	//if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 5.f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO(student): Translate the camera forward
			camera->MoveForward(deltaTime * cameraSpeed);

		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO(student): Translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);

		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO(student): Translate the camera backward
			camera->MoveForward(-deltaTime * cameraSpeed);

		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO(student): Translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);

		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO(student): Translate the camera to the right
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO(student): Translate the camera to the right
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}

	}

}


void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event

	if (key == GLFW_KEY_SPACE && proiectil.shoot == false) {
		proiectil.proiectilAngle = playerAngle;
		proiectil.shoot = true;
		proiectil.lifeTime = 1.0f;
		proiectil.speed = 0.0f;
		proiectil.move = camera->forward;
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{

		if (player.thirdPerson == true)
		{

			camera->MoveForward(2.f);
			camera->TranslateUpward(-0.4f);
			player.firstPerson = true;
			player.thirdPerson = false;


		}
		else
		{

			camera->MoveForward(-2.f);
			camera->TranslateUpward(0.4f);
			player.firstPerson = false;
			player.thirdPerson = true;

		}

	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (player.firstPerson == true)
		{
			//camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
			proiectil.spawnPos = camera->position;
		}
		if (player.thirdPerson == true)
		{
			camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
		}

		playerAngle += sensivityOY * deltaX;
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
