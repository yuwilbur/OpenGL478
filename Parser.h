/*
	Parser.h

	Name: Wilbur Yu
	Student Number: 15556079
	
	Class: EECE478
	Instructor: Dr. Gregor Miller
	Teacher's Assistant: Andrew Ho
	Assignment: 3

	DESCRIPTION:
		Contains: 	Name
					Vertices
					Normals
					Triangles
					Textures (Files and Points)
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "Model.h"

using namespace std;

class Parser{

private:
	string name;
	int texture_size;
	int vertice_size;
	int normal_size;
	int triangle_size;
	void DEBUG(Model debugModel);
	void NORMALIZE(Model debugModel);
public:	
	// Constructor
	Model Parse(string path, bool normalize = 0, bool debug = 0);
};

#endif