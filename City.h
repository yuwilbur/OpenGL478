/*
	City.h

	Name: Wilbur Yu
	Student Number: 15556079
	
	Class: EECE478
	Instructor: Dr. Gregor Miller
	Teacher's Assistant: Andrew Ho
	Assignment: 3

	DESCRIPTION:
		Contains: 	Name
					Path to Models
					Scale/Rotation/TranslateO/TranslateT
*/

#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class City{

private:
	string m_name;
	int m_city_size;
	string * m_models;
	float ** m_scale;
	float ** m_rotation;
	float ** m_translateO;
	float ** m_translateT;
public:	
	// Constructor
	City(){}
	City(
		string name,
		int city_size,
		string * models,
		float ** scale,
		float ** rotation,
		float ** translateO,
		float ** translateT
		);
	
	City Parse(string path);

	string GetName();
	int GetSize();
	string * GetModels();
	float ** GetScale();
	float ** GetRotation();
	float ** GetTranslateO();
	float ** GetTranslateT();
};

#endif