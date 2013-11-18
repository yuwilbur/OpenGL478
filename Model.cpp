/*
	Model.cpp

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

#include "Model.h"

Model::Model(
		string name,
		string * texturefile,
		float ** vertice,
		float ** normal,
		float ***triangle,
		int* size,
		int texture_size,
		float x_max,
		float x_min,
		float y_max,
		float y_min,
		float z_max,
		float z_min,
		string path
		)
{
		m_name = name;
		m_texturefile = texturefile;
		m_vertice = vertice;
		m_normal = normal;
		m_triangle = triangle;
		m_size = size;
		m_texture_size = texture_size;
		m_x_max = x_max;
		m_x_min = x_min;
		m_y_max = y_max;
		m_y_min = y_min;
		m_z_max = z_max;
		m_z_min = z_min;
		m_path = path;
		m_Textures = new GLuint[texture_size];
}

string Model::GetName(){ 
	return m_name; 
}

string * Model::GetTextureFile(){ 
	return m_texturefile; 
}

float ** Model::GetVertice(){ 
	return m_vertice; 
}

float ** Model::GetNormal(){ 
	return m_normal; 
}

float *** Model::GetTriangle(){ 
	return m_triangle; 
}

int* Model::GetSize(){ 
	return m_size; 
}

int Model::GetTextureSize(){
	return m_texture_size;
}

float Model::GetXmax(){ 
	return m_x_max; 
}

float Model::GetXmin(){ 
	return m_x_min; 
}

float Model::GetYmax(){ 
	return m_y_max; 
}

float Model::GetYmin(){ 
	return m_y_min;
}

float Model::GetZmax(){ 
	return m_z_max;
}

float Model::GetZmin(){ 
	return m_z_min;
}

string Model::GetPath(){
	return m_path;
}

void Model::SetTexturesID(GLuint * TextureIDs){
	m_Textures = TextureIDs;
}

GLuint Model::GetTexturesID(int index){
	return m_Textures[index];
}