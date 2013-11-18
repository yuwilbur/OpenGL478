/*
	Model.h

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

#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>

using namespace std;

class Model{

private:

	string	m_name;
	string* m_texturefile;
	float**	m_vertice;
	float** m_normal;
	float***m_triangle;
	int*		m_size;
	int		m_texture_size;
	float	m_x_max;
	float	m_x_min;
	float	m_y_max;
	float	m_y_min;
	float	m_z_max;
	float	m_z_min;
	string	m_path;
	GLuint * m_Textures;
	

public:
	Model(){}

	// Constructor
	Model(
		string name,
		string * texturefile,
		float ** vertice,
		float ** normal,
		float *** triangle,
		int* size,
		int texture_size,
		float x_max,
		float x_min,
		float y_max,
		float y_min,
		float z_max,
		float z_min,
		string path
		);
	
	string GetName();
	string * GetTextureFile();
	float ** GetVertice();
	float ** GetNormal();
	float *** GetTriangle();
	int* GetSize();
	int GetTextureSize();
	float GetXmax();
	float GetXmin();
	float GetYmax();
	float GetYmin();
	float GetZmax();
	float GetZmin();
	string GetPath();
	void SetTexturesID(GLuint * TextureIDs);
	GLuint GetTexturesID(int index);
};

#endif