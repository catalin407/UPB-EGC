#pragma once

class Maze
{
public:
    Maze(int mazeNumber);
    Maze();
    ~Maze();
    void GenerateStartingPoint();

public:
    int maze[8][8];
    float mazeX, mazeY, mazeZ;
    int startX, startZ;
    int enemyX, enemyZ;
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

};