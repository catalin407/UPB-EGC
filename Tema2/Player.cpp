#include "lab_m1/Tema2/Player.h"

Player::Player()
{
    translateX = 1.f;
    translateY = 1.f;
    translateZ = 1.f;
    speed = 5.f;
    playerHP = 5;
    timeV = 5;

    hitboxX = 2.f;
    hitboxY = 3.f;
    hitboxZ = 1.5f;
    thirdPerson = true;
    firstPerson = false;

    playerBodyColor = glm::vec3(0.011, 0.113, 0.729);
    playerHandColor = glm::vec3(0.729f, 0.482f, 0.149f);
    playerArmColor = glm::vec3(0.011, 0.113, 0.729);
    playerLegColor = glm::vec3(0.490, 0.011, 0.729);
    playerHeadColor = glm::vec3(0.729f, 0.482f, 0.149f);

}

Player::~Player()
{

}

void Player::SpawnPlayer()
{

    //playerBody = glm::mat4(1);
    playerBody *= transformTema::Translate(.0f, .5f, .0f);
    playerBody *= transformTema::Scale(.75f, 1.f, 0.5f);
    //playerBody *= transformTema::Translate(translateX, translateY, translateZ);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerBody);

    playerHitbox = playerBody;
    playerHitbox *= transformTema::Scale(hitboxX, hitboxY, hitboxZ);

    playerHealth = playerBody;
    playerHealth *= transformTema::Translate(0, 1.55, 0);
    playerHealth *= transformTema::Scale(0.45f, 0.05f, 0.01f);
    playerHealth *= transformTema::Scale(playerHP, 1, 1);

    time = playerBody;
    time *= transformTema::Translate(0, 2, 0);
    time *= transformTema::Scale(0.45f, 0.05f, 0.01f);
    time *= transformTema::Scale(timeV, 1, 1);

    playerArmL = playerBody;
    playerArmL *= transformTema::Translate(-0.8f, 0.125f, .0f);
    playerArmL *= transformTema::Scale(0.5f, 0.75f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerArmL);

    playerHandL = playerArmL;
    playerHandL *= transformTema::Translate(0.f, -.8f, .0f);
    playerHandL *= transformTema::Scale(1.f, 0.5f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerHandL);

    playerArmR = playerBody;
    playerArmR *= transformTema::Translate(0.8f, 0.125f, .0f);
    playerArmR *= transformTema::Scale(0.5f, 0.75f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerArmR);

    playerHandR = playerArmR;
    playerHandR *= transformTema::Translate(0.f, -.8f, .0f);
    playerHandR *= transformTema::Scale(1.f, 0.5f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerHandR);

    playerHead = playerBody;
    playerHead *= transformTema::Translate(0.f, .8f, .0f);
    playerHead *= transformTema::Scale(.5f, 0.5f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerHead);

    playerLegL = playerBody;
    playerLegL *= transformTema::Translate(-0.25f, -1.f, .0f);
    playerLegL *= transformTema::Scale(.4f, .9f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerLegL);

    playerLegR = playerBody;
    playerLegR *= transformTema::Translate(0.25f, -1.f, .0f);
    playerLegR *= transformTema::Scale(.4f, .9f, 1.f);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], playerLegR);

}