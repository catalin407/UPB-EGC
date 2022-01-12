#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transformTema.h"

class Player
{
public:
	Player();
	~Player();
    void SpawnPlayer();

public:
    glm::mat4 modelMatrix;
    glm::mat4 playerHitbox;
    glm::mat4 playerBody;
    glm::mat4 playerArmL;
    glm::mat4 playerHandL;
    glm::mat4 playerArmR;
    glm::mat4 playerHandR;
    glm::mat4 playerLegL;
    glm::mat4 playerLegR;
    glm::mat4 playerHead;
    glm::vec3 playerBodyColor;
    glm::vec3 playerHandColor;
    glm::vec3 playerArmColor;
    glm::vec3 playerHeadColor;
    glm::vec3 playerLegColor;
    glm::mat4 playerHealth;
    glm::mat4 time;
    float translateX, translateY, translateZ;
    float speed;
    bool firstPerson;
    bool thirdPerson;
    float playerHP;
    float timeV;
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float hitboxX, hitboxY, hitboxZ;

};

