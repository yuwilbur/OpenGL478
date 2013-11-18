/*
	City.cpp

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

#include "City.h"

City::City(
	string name,
	int city_size,
	string * models,
	float ** scale,
	float ** rotation,
	float ** translateO,
	float ** translateT){
	m_name = name;
	m_city_size = city_size;
	m_models = models;
	m_scale = scale;
	m_rotation = rotation;
	m_translateO = translateO;
	m_translateT = translateT;
}

City City::Parse(string path){
	
	ifstream myfile(path, ios::in);
	if (!myfile.is_open())
	{
		cout << "FAILED: Invalid file" << endl;
		cout << "If model is in a sister directory, use: 'SisterDir/City.city'" << endl;
		cout << "If model is in a parallel directory, use: '../ParallelDir/City.city' or './ParallelDir/City.city'" << endl;
		system("pause");
		exit(0);
	}
	else
	{
		int model_num = 0;
		int state = 0;
		string line;
		int line_pos;
		bool assignPos = 1;
		bool chkModel = 1;
		bool chkScale = 0;
		bool chkRotation = 0;
		bool chkTranslateO = 0;
		bool chkTranslateT = 0;
		int Counter = 0;
		
		while(getline(myfile,line))
		{
			line_pos = 0;
			for( unsigned int i = 0; i < line.size() ; i++ )
			{

				if( line[0] == '#' || line[0] == NULL) break;

				if(state == 0){
					m_name = line;
					state = 1;
					break;
				}
				else if(state == 1 && i == 0){
					m_city_size = atoi(line.c_str());
					m_models = new string[m_city_size];
					m_scale = new float*[m_city_size];
					m_rotation = new float*[m_city_size];
					m_translateO = new float*[m_city_size];
					m_translateT = new float*[m_city_size];
					state = 2;
					break;
				}
				else if(state == 2 && model_num < m_city_size){
					if(assignPos && line[i] != ' ' && line[i] != '\t'){ 
						line_pos = i;
						assignPos = !assignPos; 
					}
					if(line[i] == ' ' || line[i] == '\t' || i == line.size()-1){
						if( i == line.size()-1 && line[i] != ' ' && line[i] != '\t' ) i++;
						if( chkModel ){
							m_models[model_num] = line.substr(line_pos,i-line_pos).c_str();
							chkModel = !chkModel;
							chkScale = !chkScale;
							assignPos = !assignPos;
							m_scale[model_num] = new float[3];
							m_rotation[model_num] = new float[3];
							m_translateO[model_num] = new float[3];
							m_translateT[model_num] = new float[3];
						}
						else if( chkScale ){
							m_scale[model_num][Counter] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							Counter++;
							if(Counter == 3){
								Counter = 0;
								chkScale = !chkScale;
								chkRotation = !chkRotation;
							}
							assignPos = !assignPos;
						}
						else if( chkRotation ){
							m_rotation[model_num][Counter] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							Counter++;
							if(Counter == 3){
								Counter = 0;
								chkRotation = !chkRotation;
								chkTranslateO = !chkTranslateO;
							}
							assignPos = !assignPos;
						}
						else if( chkTranslateO ){
							m_translateO[model_num][Counter] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							Counter++;
							if(Counter == 3){
								Counter = 0;
								chkTranslateO = !chkTranslateO;
								chkTranslateT = !chkTranslateT;
							}
							assignPos = !assignPos;
						}else if( chkTranslateT ){
							m_translateT[model_num][Counter] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							Counter++;
							if(Counter == 3){
								Counter = 0;
								chkTranslateT = !chkTranslateT;
								chkModel = !chkModel;
								model_num++;
							}
							assignPos = !assignPos;
						}
					}
				}
			}
		}
		return City(m_name,m_city_size,m_models,m_scale,m_rotation,m_translateO,m_translateT);
	}
	return City();
}

string City::GetName(){
	return m_name;
}

int City::GetSize(){
	return m_city_size;
}

string * City::GetModels(){
	return m_models;
}

float ** City::GetScale(){
	return m_scale;
}

float ** City::GetRotation(){
	return m_rotation;
}

float ** City::GetTranslateO(){
	return m_translateO;
}

float ** City::GetTranslateT(){
	return m_translateT;
}