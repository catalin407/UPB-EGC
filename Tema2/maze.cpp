#include "lab_m1/Tema2/maze.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Maze::Maze()
{}

Maze::Maze(int mazeNumber)
{

	string myText = "mazes\\" + to_string(mazeNumber) + ".txt";

	ifstream MyFile;
	MyFile.open(myText);

	mazeX = 1.f;
	mazeY = 4.f;
	mazeZ = 1.f;

	while (!MyFile.eof())
	{

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				MyFile >> maze[i][j];

	}

	MyFile.close();
	cout << myText << endl;

}

void Maze::GenerateStartingPoint()
{

	startX = rand() % 8;
	startZ = rand() % 8;
	
	while (maze[startX][startZ] == 1 || maze[startX][startZ] == 2)
	{

		startX = rand() % 8;
		startZ = rand() % 8;

	}

}

Maze::~Maze()
{}