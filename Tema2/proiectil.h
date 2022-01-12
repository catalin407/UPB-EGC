#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transformTema.h"

class Proiectil
{
public:
    Proiectil();
    ~Proiectil();


public:
    glm::mat4 matrix;
    glm::vec3 spawnPos;
    glm::vec3 move;
    float proiectilAngle;
    float lifeTime;
    bool shoot;
    float speed;
    float velocity;

};