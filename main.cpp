/*
	main.cpp

	Name: Wilbur Yu
	Student Number: 15556079
	
	Class: EECE478
	Instructor: Dr. Gregor Miller
	Teacher's Assistant: Andrew Ho
	Assignment: 3

	DESCRIPTION:
		Main loop of execution.
		Contains: 	GLUT/OpenGL Camera
					Lighting
					Rendering
					Text Display
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <time.h>

#include "Model.h"
#include "Parser.h"
#include "PPM.hpp"
#include "City.h"

#define PI 3.14156926535897

const int RainCount = 10000;

typedef struct{
	double Xpos;
	double Ypos;
	double Zpos;
	double Ymov;
	double Red;
	double Green;
	double Blue;
	double acc;
	bool Visible;
}RAIN;

RAIN Rain[RainCount];

float frames_per_second = 33.3;

bool debug = 0; // Enable for Debugging

// Values for Keyboard/Mouse
bool key_cull = 1;
bool key_lighting = 1;
bool key_wireframe = 0;
bool key_texture = 1;
bool key_show = 1;
bool key_cull_front = 1;
bool key_lighting_move = 0;
bool key_thunder = 0;
bool key_sky2;
bool recordMovie = 0;
bool animateMovie = 0;
bool motionBlur = 0;
bool rain = 0;
float brightness = 0.0;
float darkness = 0.0;
bool light0 = 1;
bool light2 = 0;
bool solar = 0;
bool logo = 0;
float SolarScreenY = 0.0;
bool credits = 0;
int caption = 0;
float fog_end = 2000.0f;

int Window_Width = 1280;
int Window_Height = 720;
const int Window_Position_X = 0;
const int Window_Position_Y = 0;

GLUquadricObj * quadratic; // Storage for Quadratic Objects
GLuint quad_texture[1];

// Set Lighting Position
GLfloat light0_pos[4] =	{0.0, 0.0, 100000.0, 0.0};
GLfloat light1_pos[4] =	{0.0, -10.0, 0.0, 1.0};
GLfloat light2_pos[4] = {0.0, 0.0, 2050, 0};
float ambient = 1.0f;
GLfloat ambientColor[3] = {ambient,ambient,ambient};
GLfloat colorWhite[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat colorGrey[4] = {0.5f,0.5f,0.5f,1.0f};
GLfloat colorDark[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat colorYellow[4] = {1.0f,1.0f,0.5f,1.0f};
GLfloat light0color[4] = {colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3]};
GLfloat light1color[4] = {colorYellow[0], colorYellow[1], colorYellow[2], colorYellow[3]};
GLfloat light2color[4] = {colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3]};
GLfloat light1_dir[] = {0,0,-1};

float personHeight = 8;
float distance_from_sun = 9000;
float spaceRadius = 20000;
float sun_x = 0;
float sun_y = 0;
float sun_z = 0;
float sunRadius = 7500.0;
float mercury_x = distance_from_sun*1;
float mercury_y = 0;
float mercury_z = 0;
float mercuryRadius = 1000.0;
float venus_x = distance_from_sun*2;
float venus_y = 0;
float venus_z = 0;
float venusRadius = 1500.0;
float earth_x = 0;
float earth_y = 0;
float earth_z = 0;
float earthRadius = 2000.0;
float earth_x2 = distance_from_sun*3;
float earth_y2 = 0;
float earth_z2 = 0;
float earthRadius2 = 2000.0;
float sky_x = earth_x;
float sky_y = earth_y;
float sky_z = earth_z;
float skyRadius = 3000.0;
float mars_x = 100000;
float mars_y = 0;
float mars_z = 100000;
float marsRadius = 2000.0;
float mars_x2 = distance_from_sun*4;
float mars_y2 = 0;
float mars_z2 = 0;
float marsRadius2 = 2000.0;
float moon_x = earth_x;
float moon_y = earth_y;
float moon_z = earth_z;
float moonRadius = 200.0;
float moon_x2 = earth_x2;
float moon_y2 = earth_y2;
float moon_z2 = earth_z2;
float moonRadius2 = 750.0;
float jupiter_x = distance_from_sun*5;
float jupiter_y = 0;
float jupiter_z = 0;
float jupiterRadius = 4000.0;
float saturn_x = distance_from_sun*6;
float saturn_y = 0;
float saturn_z = 0;
float saturnRadius = 3500.0;
float uranus_x = distance_from_sun*7;
float uranus_y = 0;
float uranus_z = 0;
float uranusRadius = 1500.0;
float neptune_x = distance_from_sun*8;
float neptune_y = 0;
float neptune_z = 0;
float neptuneRadius = 1500.0;
float pluto_x = distance_from_sun*9;
float pluto_y = 0;
float pluto_z = 0;
float plutoRadius = 1000.0;

float offset_x = 0;
float offset_y = 0;
float offset_z = 0;

float doorY = 0.0;
// Values for Animation
//float animationCounter = 3986;
float animationCounter = -150;

namespace Camera{
// Values for Camera Position
float x;
float y;
float z;

// Values for Camera Angle
float dx = 0.0f;
float dy = 0.0f;
float dz = -1.0f;
float viewX = 0.0f;
float viewY = 0.0f;
float viewZ = 0.0f;
float x_up = 0.0f;
float y_up = 1.0f;
float z_up = 0.0f;

// Values for Mouse Position
float mouse_move = 0;
float mouse_x;
float previous_x;
float previous_mouse_x = 0.0f;
float mouse_y;
float previous_y;
float previous_mouse_y = 0.0f;

// Values for Move(Forward/Back), Mouse(Angles), Strafe(Left/Right) and Fly(Up/Down)
float speedModifier = 1.0f;
const float moveSpd = 1.0f;
const float mouseSpd = 0.005f;
const float strafeSpd = 0.1f;
const float flySpd = 0.5f;
float moveSpeed = moveSpd;
float mouseSpeed = mouseSpd;
float strafeSpeed = strafeSpd;
float flySpeed = flySpd;

// Values for Change in Move, Strafe and Fly
float deltamove = 0;
float deltastrafemove = 0;
float deltafly = 0.0f;

void adjustSpeed(float speed){
	// Speed can be adjusted with speedModifier
	Camera::speedModifier = speed;
	Camera::strafeSpeed = Camera::speedModifier*Camera::strafeSpd;
	Camera::moveSpeed = Camera::speedModifier*Camera::moveSpd;
	Camera::flySpeed = Camera::speedModifier*Camera::flySpd;
}

// Computes Camera's Position and Direction
void computeCamera(){
	// Fly goes straight up and straight down. X and Z will not affect Fly and vice versa
	Camera::x += deltamove * Camera::dx * 0.1f - deltastrafemove * Camera::dz;
	Camera::y += Camera::deltafly * 0.1f;
	Camera::z += deltamove * Camera::dz * 0.1f + deltastrafemove * Camera::dx;
	
	Camera::dx = sin(Camera::previous_mouse_x + Camera::mouse_x);
	Camera::dy = sin(Camera::previous_mouse_y + Camera::mouse_y);
	Camera::dz = -cos(Camera::previous_mouse_x + Camera::mouse_x);

	Camera::viewX = Camera::x + Camera::dx;
	Camera::viewY = Camera::y + Camera::dy;
	Camera::viewZ = Camera::z + Camera::dz;
}

void pressNormalKeys(unsigned char key, int x, int y){
	if(key == 27){ // ESC Button
		exit(0);
	}
	switch(key)
	{
	case 'q':		Camera::deltafly = Camera::flySpeed; break; // q: Up
	case 'e':		Camera::deltafly = -Camera::flySpeed; break; // e: Down
	case 'a':		Camera::deltastrafemove = -Camera::strafeSpeed; break; //a: Strafe Left
	case 'd':		Camera::deltastrafemove = Camera::strafeSpeed; break; //d: Strafe Right
	case 'w':		Camera::deltamove = Camera::moveSpeed; break; //w: Forward
	case 's':		Camera::deltamove = -Camera::moveSpeed; break; //s: Backward

	case 'f':		key_wireframe = !key_wireframe; key_wireframe ? glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) : glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
					break; 
	case 'c':		key_cull = !key_cull; 
					key_cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
					break;
	case 'l':		key_lighting = !key_lighting; 
					break;
	case 't':		key_texture = !key_texture; 
					key_texture ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
					break; 
	case 'b':		key_cull_front = !key_cull_front; 
					glFrontFace(key_cull_front ? GL_CCW : GL_CW);
					break;
	case 'r':		rain = !rain; break;
	case 'h':		key_thunder = !key_thunder; break;
	case 'k':		key_sky2 = !key_sky2; break;
	case 'g':		fog_end = 300; break;


	case '+':		Camera::adjustSpeed(Camera::speedModifier*2); break;
	case '-':		Camera::adjustSpeed(Camera::speedModifier/2); break;
		
	case '0':		Camera::x = sun_x; Camera::y = sun_y; Camera::z = sun_z; break;
	case '1':		Camera::x = mercury_x; Camera::y = mercury_y; Camera::z = mercury_z; break;
	case '2':		Camera::x = venus_x; Camera::y = venus_y; Camera::z = venus_z; break;
	case '3':		Camera::x = earth_x; Camera::y = earth_y; Camera::z = earth_z; break;
	case '4':		Camera::x = mars_x; Camera::y = mars_y; Camera::z = mars_z; break;
	case '5':		Camera::x = jupiter_x; Camera::y = jupiter_y; Camera::z = jupiter_z; break;
	case '6':		Camera::x = saturn_x; Camera::y = saturn_y; Camera::z = saturn_z; break;
	case '7':		Camera::x = uranus_x; Camera::y = uranus_y; Camera::z = uranus_z; break;
	case '8':		Camera::x = neptune_x; Camera::y = neptune_y; Camera::z = neptune_z; break;
	case '9':		Camera::x = pluto_x; Camera::y = pluto_y; Camera::z = pluto_z; break;
	}
}

// Let go to Stop
void releaseNormalKeys(unsigned char key, int x, int y){
	switch(key)
	{
	case 'q':		Camera::deltafly = 0.0f; break;
	case 'e':		Camera::deltafly = 0.0f; break;
	case 'a':		Camera::deltastrafemove = 0.0f; break;
	case 'd':	 	Camera::deltastrafemove = 0.0f; break;
	case 'w':		Camera::deltamove = 0.0f; break;
	case 's':		Camera::deltamove = 0.0f; break;
	}
}

void mouseButton(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON)
	{
		if( state == GLUT_DOWN)
		{ 
			//Save current location when Left-Button is pressed
			Camera::previous_x = x;
			Camera::previous_y = y;
			Camera::mouse_move = 1;
		}
		else
		{
			//Set current direction after Left-Button is released
			Camera::mouse_move = 0;
			Camera::previous_mouse_x += Camera::mouse_x;
			Camera::previous_mouse_y += Camera::mouse_y;
			Camera::mouse_x = 0;
			Camera::mouse_y = 0;
		}	
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if( state == GLUT_DOWN )
		{
			key_show = 0;
		}
		else
		{
			key_show = 1;
		}
	}
}

void mouseMove(int x, int y){
	if( mouse_move ){
		Camera::mouse_x = (float(x) - Camera::previous_x)*mouseSpeed;
		Camera::mouse_y = -(float(y) - Camera::previous_y)*mouseSpeed;
	}
}

} // namespace Camera

namespace Init{

// Declaration of index for list
GLuint list_index;
int list_index_size;

string cityname;

GLUquadricObj * envSpace;
GLuint spaceTexture;
GLUquadricObj * envSun;
GLuint sunTexture;
GLUquadricObj * envMercury;
GLuint mercuryTexture;
GLUquadricObj * envVenus;
GLuint venusTexture;
GLUquadricObj * envEarth;
GLuint earthTexture;
GLUquadricObj * envEarth2;
GLuint earthTexture2;
GLUquadricObj * envSky;
GLuint skyTexture;
GLuint skyTexture2;
GLUquadricObj * envMoon;
GLuint moonTexture;
GLUquadricObj * envMoon2;
GLuint moonTexture2;
GLUquadricObj * envMars;
GLuint marsTexture;
GLUquadricObj * envMars2;
GLuint marsTexture2;
GLUquadricObj * envJupiter;
GLuint jupiterTexture;
GLUquadricObj * envSaturn;
GLuint saturnTexture;
GLUquadricObj * envUranus;
GLuint uranusTexture;
GLUquadricObj * envNeptune;
GLuint neptuneTexture;
GLUquadricObj * envPluto;
GLuint plutoTexture;

GLuint doorTexture;
GLuint logoTexture;

void CreateRain(){
	for(int i = 1; i < RainCount; i++){
		Rain[i].Xpos = rand()%(int)earthRadius - earthRadius/2;
		Rain[i].Ypos = earthRadius*2;
		Rain[i].Zpos = rand()%(int)earthRadius - earthRadius/2;
		Rain[i].Ymov = 0;
		Rain[i].Red = 0;
		Rain[i].Green = 0;
		Rain[i].Blue = 1;
		Rain[i].acc = -rand()%10-9.8;
	}
}

void createTextures(GLuint &Texture, string path){
	int width, height;
	glGenTextures(1,&Texture);
	unsigned char * Pixel = PPM::Read(path, width, height);
	
	glBindTexture(GL_TEXTURE_2D,Texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,Pixel);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete Pixel;
}

void SetEnvironment(){
	createTextures(logoTexture, "logo.ppm");

	envSpace = gluNewQuadric();
	gluQuadricNormals( envSpace, GLU_SMOOTH);
	gluQuadricTexture( envSpace, GL_TRUE);
	createTextures(spaceTexture, "space.ppm");

	envSun = gluNewQuadric();
	gluQuadricNormals( envSun, GLU_SMOOTH);
	gluQuadricTexture( envSun, GL_TRUE);
	createTextures(sunTexture, "planet_sun.ppm");

	envMercury = gluNewQuadric();
	gluQuadricNormals( envMercury, GLU_SMOOTH);
	gluQuadricTexture( envMercury, GL_TRUE);
	createTextures(mercuryTexture, "planet_mercury.ppm");

	envVenus = gluNewQuadric();
	gluQuadricNormals( envVenus, GLU_SMOOTH);
	gluQuadricTexture( envVenus, GL_TRUE);
	createTextures(venusTexture, "planet_venus.ppm");

	envEarth = gluNewQuadric();
	gluQuadricNormals( envEarth, GLU_SMOOTH);
	gluQuadricTexture( envEarth, GL_TRUE);
	createTextures(earthTexture, "planet_earth.ppm");

	envEarth2 = gluNewQuadric();
	gluQuadricNormals( envEarth2, GLU_SMOOTH);
	gluQuadricTexture( envEarth2, GL_TRUE);
	createTextures(earthTexture2, "planet_earth.ppm");

	envMoon = gluNewQuadric();
	gluQuadricNormals( envMoon, GLU_SMOOTH);
	gluQuadricTexture( envMoon, GL_TRUE);
	createTextures(moonTexture, "planet_moon.ppm");

	envMoon2 = gluNewQuadric();
	gluQuadricNormals( envMoon2, GLU_SMOOTH);
	gluQuadricTexture( envMoon2, GL_TRUE);
	createTextures(moonTexture2, "planet_moon.ppm");

	envMars = gluNewQuadric();
	gluQuadricNormals( envMars, GLU_SMOOTH);
	gluQuadricTexture( envMars, GL_TRUE);
	createTextures(marsTexture, "planet_mars.ppm");

	envMars2 = gluNewQuadric();
	gluQuadricNormals( envMars2, GLU_SMOOTH);
	gluQuadricTexture( envMars2, GL_TRUE);
	createTextures(marsTexture2, "planet_mars.ppm");

	envSky = gluNewQuadric();
	gluQuadricNormals( envSky, GLU_SMOOTH);
	gluQuadricTexture( envSky, GL_TRUE);
	createTextures(skyTexture, "planet_sky.ppm");
	createTextures(skyTexture2, "planet_sky2.ppm");

	envJupiter = gluNewQuadric();
	gluQuadricNormals( envJupiter, GLU_SMOOTH);
	gluQuadricTexture( envJupiter, GL_TRUE);
	createTextures(jupiterTexture, "planet_jupiter.ppm");

	envSaturn = gluNewQuadric();
	gluQuadricNormals( envSaturn, GLU_SMOOTH);
	gluQuadricTexture( envSaturn, GL_TRUE);
	createTextures(saturnTexture, "planet_saturn.ppm");

	envUranus = gluNewQuadric();
	gluQuadricNormals( envUranus, GLU_SMOOTH);
	gluQuadricTexture( envUranus, GL_TRUE);
	createTextures(uranusTexture, "planet_uranus.ppm");

	envNeptune = gluNewQuadric();
	gluQuadricNormals( envNeptune, GLU_SMOOTH);
	gluQuadricTexture( envNeptune, GL_TRUE);
	createTextures(neptuneTexture, "planet_neptune.ppm");

	envPluto = gluNewQuadric();
	gluQuadricNormals( envPluto, GLU_SMOOTH);
	gluQuadricTexture( envPluto, GL_TRUE);
	createTextures(plutoTexture, "planet_pluto.ppm");
	
	createTextures(doorTexture, "door.ppm");
}

void SetLighting(){
	ambientColor[0] = ambient;
	ambientColor[1] = ambient;
	ambientColor[2] = ambient;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientColor);

	if(light0) glEnable(GL_LIGHT0);	 
	else glDisable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);
	
	if(light2) glEnable(GL_LIGHT2);	 
	else glDisable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2color);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2color);
}

void SetSpotlight(){
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);
	
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);
}

void SetCamera(){
	std::cout << "Set Camera...";
	//Initialize Camera Positions to Face Model
	Camera::x = mars_x;
	Camera::dx = 0.0;
	Camera::y = marsRadius+8;
	Camera::dy = 0.0f;
	Camera::z = mars_z;
	Camera::dz = -1.0f;
	
	//Modify Movement Speed depending on Model Size
	Camera::adjustSpeed(400);
	std::cout << "COMPLETE" << endl;
}

void SetControls(){
	std::cout << "Set Controls...";
	// Keyboard Press and Release Functions
	// For WASD Movements
	glutKeyboardFunc(Camera::pressNormalKeys);
	glutKeyboardUpFunc(Camera::releaseNormalKeys);
	
	// Mouse
	glutMouseFunc(Camera::mouseButton);
	glutMotionFunc(Camera::mouseMove);
	std::cout << "COMPLETE" << endl;
}

void SetFog(){
	std::cout << "Set Fog...";
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogfv(GL_FOG_COLOR,colorGrey);
	glHint(GL_FOG_HINT,GL_NICEST);
	glFogf(GL_FOG_START, 0.0f);
	glFogf(GL_FOG_END, fog_end);
	std::cout << "COMPLETE" << endl;
}

GLuint * LoadTextures(Model inputModel){
	int texture_size = inputModel.GetTextureSize();
	GLuint * Texture = new GLuint[texture_size];
	int width, height;
	int test_width;
	int test_height;
	glGenTextures(inputModel.GetTextureSize(),Texture);

	for(int i = 0; i < texture_size; i++){

		unsigned char * Pixel = PPM::Read(inputModel.GetPath()+inputModel.GetTextureFile()[i], width, height);
	
		test_width = width;
		test_height = height;
	
		if(test_width > 512){
			cout << "WARNING(WidthSize): \t" << inputModel.GetName() << "/" << inputModel.GetTextureFile()[i] << endl;
		}

		if(test_height > 512){
			cout << "WARNING(HeightSize): \t" << inputModel.GetName() << "/" << inputModel.GetTextureFile()[i] << endl;
		}

		while( test_width != 1 && test_width <= 512){
			if( test_width%2 != 0 ){ 
				cout << "WARNING(Width2^): \t" << inputModel.GetName() << "/" << inputModel.GetTextureFile()[i] << endl;
				break;
			}
			test_width /= 2;
		}

		while( test_height != 1 && test_height <= 512){
			if( test_height%2 != 0 ){ 
				cout << "WARNING(Height2^):\t" << inputModel.GetName() << "/" << inputModel.GetTextureFile()[i] << endl;
				break;
			}
			test_height /= 2;
		}

		glBindTexture(GL_TEXTURE_2D,Texture[i]);
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,Pixel);
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete Pixel;
	}

	return Texture;
}

void renderModel(Model inputModel, GLuint * Texture, float *S, float *R, float *O, float *T){
	std::cout << "\tRendering...";
	float ** normal_data_ptr = inputModel.GetNormal();
	float *** triangle_data_ptr = inputModel.GetTriangle();
	float ** vertice_data_ptr = inputModel.GetVertice();
	int texture_size = inputModel.GetTextureSize();
	int * size = inputModel.GetSize();
	
	int x = 0;
	int y = 1;
	int z = 2;

	glPushMatrix();
	glTranslatef(T[x], T[y], T[z]);
	glScalef(S[x], S[y], S[z]);
	glRotatef(R[z],0,0,1);
	glRotatef(R[y],0,1,0);
	glRotatef(R[x],1,0,0);
	glTranslatef(O[x], O[y], O[z]);
	for(int i = 0; i < texture_size; i++){

		glBindTexture(GL_TEXTURE_2D, Texture[i]);

		glBegin(GL_TRIANGLES);
		for( int j = 0; j < size[i] ; j++)
		{
			glNormal3f(
				normal_data_ptr[(int)triangle_data_ptr[i][j][3]][0],
				normal_data_ptr[(int)triangle_data_ptr[i][j][3]][1],
				normal_data_ptr[(int)triangle_data_ptr[i][j][3]][2]);
			
			glTexCoord2f(triangle_data_ptr[i][j][5],triangle_data_ptr[i][j][6]);
			glVertex3f(
				vertice_data_ptr[(int)triangle_data_ptr[i][j][0]][0],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][0]][1],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][0]][2]);
				
			glTexCoord2f(triangle_data_ptr[i][j][7],triangle_data_ptr[i][j][8]);
			glVertex3f(
				vertice_data_ptr[(int)triangle_data_ptr[i][j][1]][0],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][1]][1],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][1]][2]);

			glTexCoord2f(triangle_data_ptr[i][j][9],triangle_data_ptr[i][j][10]);
			glVertex3f(
				vertice_data_ptr[(int)triangle_data_ptr[i][j][2]][0],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][2]][1],
				vertice_data_ptr[(int)triangle_data_ptr[i][j][2]][2]);

		}
		glEnd();
	}
	glPopMatrix();
	
	std::cout << "COMPLETE" << endl;
}

void SetModels(){
	
	City myCity;
	Model * Models;
	Parser myParser;
	clock_t Start_t, End_t;
	GLuint ** Texture;

	//Parse City
	std::cout << "Loading City..." << endl;
	Start_t = clock();
	myCity = myCity.Parse(Init::cityname);
	std::cout << "\t" << myCity.GetName() << " [Size: " << myCity.GetSize() << "]" << endl;
	Models = new Model[myCity.GetSize()];
	Texture = new GLuint*[myCity.GetSize()];

	std::cout << "Set Models..." << endl << endl;
	//Generate List
	Init::list_index_size = myCity.GetSize();
	Init::list_index = glGenLists(Init::list_index_size);
	for(int i = 0; i < myCity.GetSize(); i++){
		Models[i] = myParser.Parse(myCity.GetModels()[i]); // Parse Models
		Texture[i] = LoadTextures(Models[i]); // Create Textures
		glNewList(Init::list_index+i, GL_COMPILE);
			renderModel(Models[i],Texture[i],myCity.GetScale()[i],myCity.GetRotation()[i],myCity.GetTranslateO()[i],myCity.GetTranslateT()[i]);
		glEndList();
	}


	End_t = clock();
	std::cout << "Set Models...COMPLETE [" << (float)(End_t-Start_t)/(1000.0) << "s]" << endl;
}

void Initialize(){	
	std::cout << "-----INITIALIZE-----" << endl << endl;
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);	
	if(key_texture) glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	if(key_cull) glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	SetModels();
	SetEnvironment();
	CreateRain();
	SetSpotlight();
	SetFog();
	if(!animateMovie) SetCamera();
	SetControls();
	std::cout << endl;
}

void Instructions(){
	std::cout << "-----START-----" << endl << endl;
	std::cout << "Keyboard Controls:" << endl;
	std::cout << "\tW: Move Forward" << endl;
	std::cout << "\tA: Strafe Left" << endl;
	std::cout << "\tS: Move Backward" << endl;
	std::cout << "\tD: Strafe Right" << endl;
	std::cout << "\tQ: Move Up" << endl;
	std::cout << "\tE: Move Down" << endl;
	std::cout << endl;
	std::cout << "Mouse Controls:" << endl;
	std::cout << "\tLeft Button: Rotate Camera" << endl;
	std::cout << "\tRight Button: Toggle Wireframe" << endl;
}

} // namespace Init

namespace Draw{

// Variables for Animation
float prev_x;
float prev_y;
float prev_z;
float prev_x2;
float prev_y2;
float prev_z2;
float v_x;
float v_y;
float v_z;
float prev_dx;
float prev_dy;
float prev_dz;

// Variables for FPS
int frame;
long time, timebase;
char fps[20];

// Keep Ratio as Window Resizes
void changeSize(int w, int h)
{
	Window_Width = w;
	Window_Height = h;

	float ratio = w*1.0/h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w,h);
	gluPerspective(45.0f, ratio, 0.1f,spaceRadius*1.1);
	glMatrixMode(GL_MODELVIEW);
}

// Function that takes in position, font, string and color. Then displays it 2D
void renderBitmapString(float x, float y, void *font, char *string, float r, float g, float b){
	glColor3f(r,g,b);
	glRasterPos2f(x,y);
	while(*string != '\0')
	{
		glutBitmapCharacter(font, *string);
		string++;
	}
}

// Write FPS and Assignment Information
void writeInformation(){
	glDisable(GL_LIGHTING); // Disable Lighting so Information will retain its Color

	// Setting Projection to 2D Orthogonal
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, Window_Width, Window_Height, 0);
	glMatrixMode(GL_MODELVIEW);

	// Calculate FPS
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000){
		sprintf_s(fps,"FPS: %4.2f",frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}	
	
	//Converting Position to String
	char x_pos[20] = {0};
	char y_pos[20] = {0};
	char z_pos[20] = {0};
	sprintf_s(x_pos, "X: %4.2f", Camera::x);
	sprintf_s(y_pos, "Y: %4.2f", Camera::y);
	sprintf_s(z_pos, "Z: %4.2f", Camera::z);

	//String for Keyboard Settings
	char lighting[20] = {0};
	char cull[20] = {0};
	char face[20] = {0};
	char texture[20] = {0};
	char wireframe[20] = {0};

	sprintf_s(lighting, "[L] Lighting: %i", key_lighting);
	sprintf_s(cull, "[C] Cull: %i", key_cull);
	key_cull_front ? sprintf_s(face, "[B] Cull Dir: CCW") : sprintf_s(face, "[B] Cull Dir: CW");
	sprintf_s(texture, "[T] Texture: %i", key_texture);
	sprintf_s(wireframe, "[F] Wireframe: %i", key_wireframe);


	// Write Course, Assignment, Name and Student Number, FPS and Location
	glPushMatrix();
	glLoadIdentity();	
	
	renderBitmapString(Window_Width-110,Window_Height-10, GLUT_BITMAP_TIMES_ROMAN_24, "Wilbur Yu", 1.0f, 1.0f, 1.0f);
	renderBitmapString(Window_Width-108,Window_Height-8, GLUT_BITMAP_TIMES_ROMAN_24, "Wilbur Yu", 0.0f, 0.0f, 0.0f);
	if(!animateMovie){
		renderBitmapString(Window_Width-125,20, GLUT_BITMAP_TIMES_ROMAN_24, x_pos, 1.0f, 0.0f, 0.0f); //Display Current X Position
		renderBitmapString(Window_Width-125,40, GLUT_BITMAP_TIMES_ROMAN_24, y_pos, 1.0f, 0.0f, 0.0f); //Display Current Y Position
		renderBitmapString(Window_Width-125,60, GLUT_BITMAP_TIMES_ROMAN_24, z_pos, 1.0f, 0.0f, 0.0f); //Display Current Z Position
		renderBitmapString(0,20,GLUT_BITMAP_TIMES_ROMAN_24, fps, 0.0f, 1.0f, 0.0f); //Display Frames-Per-Second
		renderBitmapString(0,Window_Height-10,GLUT_BITMAP_TIMES_ROMAN_24, lighting, 1.0f, 1.0f, 1.0f);
		key_cull ? renderBitmapString(0,Window_Height-35,GLUT_BITMAP_TIMES_ROMAN_24, face, 1.0f, 1.0f, 1.0f) : renderBitmapString(0,Window_Height-35,GLUT_BITMAP_TIMES_ROMAN_24, face, 0.5f, 0.5f, 0.5f); //Display Frames-Per-Second
		renderBitmapString(0,Window_Height-60,GLUT_BITMAP_TIMES_ROMAN_24, cull, 1.0f, 1.0f, 1.0f);
		renderBitmapString(0,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, texture, 1.0f, 1.0f, 1.0f);
		renderBitmapString(0,Window_Height-110,GLUT_BITMAP_TIMES_ROMAN_24, wireframe, 1.0f, 1.0f, 1.0f);
	}

	if(caption == 0){
	}
	else if(caption == 1){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Where am I?", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Where am I?", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 2){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Mars?!", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Mars?!", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 3){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Well, at least the spacecraft is still working.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Well, at least the spacecraft is still working.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 4){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Computer, take me back to Earth.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Computer, take me back to Earth.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 5){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 6){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..2..", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..2..", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 7){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..2..1...", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: Acknowledged. Entering hyperspace in 3..2..1...", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 8){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Gorgeous ain't she?", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Gorgeous ain't she?", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 9){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: If you're asking me, I can do better.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: If you're asking me, I can do better.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 10){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: SYSTEM FAILURE. SYSTEM FAILURE. SYSTEM FAILURE.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Computer: SYSTEM FAILURE. SYSTEM FAILURE. SYSTEM FAILURE.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 11){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh crap!", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh crap!", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 12){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "*CRASH*", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "*CRASH*", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 13){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Argh...", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Argh...", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 14){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: What a crash...", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: What a crash...", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 15){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Great. My spacecraft is down, and it's raining. How worse can it get?", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Great. My spacecraft is down, and it's raining. How worse can it get?", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 16){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: ...Nope nope nope nope nope", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: ...Nope nope nope nope nope", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 17){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "*SLAM*", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "*SLAM*", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 18){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Somehow the door locked itself. From the outside.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Somehow the door locked itself. From the outside.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 19){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "*CLICK*", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "*CLICK*", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 20){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: ...Random flashlight...", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: ...Random flashlight...", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 21){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 22){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Pu", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Pu", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 23){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puu", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puu", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 24){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuu", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuu", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 25){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuuu", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuuu", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 26){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuuurdy~~~", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Oh~~~ Puuuurdy~~~", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 27){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Lights please?", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Lights please?", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 28){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Thanks!", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Thanks!", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 29){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Pluto, you'll always be a planet to me.", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: Pluto, you'll always be a planet to me.", 0.0f, 0.0f, 0.0f);
	}
	else if(caption == 30){
		renderBitmapString(50,Window_Height-85,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: So misty in here...", 1.0f, 1.0f, 1.0f);
		renderBitmapString(52,Window_Height-83,GLUT_BITMAP_TIMES_ROMAN_24, "Insignificant Guy: So misty in here...", 0.0f, 0.0f, 0.0f);
	}

	if(credits){
		renderBitmapString(Window_Width/2-100,Window_Height/2,GLUT_BITMAP_TIMES_ROMAN_24, "Thanks for Watching!", 1.0f, 1.0f, 1.0f);
		renderBitmapString(Window_Width/2-98,Window_Height/2+2,GLUT_BITMAP_TIMES_ROMAN_24, "Thanks for Watching!", 0.0f, 0.0f, 0.0f);
	}

	glColor3f(1,1,1);
	if(logo){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,Init::logoTexture);
	}
	else{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f,1.0f,1.0f,brightness);
		if(darkness>0) glColor4f(0.0f,0.0f,0.0f,darkness);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex2f(0.0,0.0);
		glTexCoord2f(0.0,1.0);
		glVertex2f(0.0,Window_Height);
		glTexCoord2f(1.0,1.0);
		glVertex2f(Window_Width,Window_Height);
		glTexCoord2f(1.0,0.0);
		glVertex2f(Window_Width,0.0);
	glEnd();
	glDisable(GL_BLEND);
	if(key_texture) glEnable(GL_TEXTURE_2D);

	// Restore Matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	if(key_lighting) glEnable(GL_LIGHTING); //Re-enable Lighting
}

void recordScene(string filename){
	unsigned char * Pixel = new unsigned char[Window_Width*Window_Height*3];
	glReadPixels(0,0,Window_Width,Window_Height,GL_RGB,GL_UNSIGNED_BYTE,Pixel);
	PPM::Write(filename,Pixel,Window_Width,Window_Height);
	delete Pixel;
}

void renderSolarSystem(float scale = 1.0f){	
	float earth_orbit = 0.1;
	// Rendering Sun
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, Init::sunTexture);
	gluSphere(Init::envSun,sunRadius*scale,100,100);
	glPopMatrix();

	// Rendering Mercury
	float mercury_orbit = 0.24*earth_orbit;
	glPushMatrix();
	glTranslatef(mercury_x*cos(animationCounter/mercury_orbit*PI/360),mercury_y,mercury_x*sin(animationCounter/mercury_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::mercuryTexture);	
	gluSphere(Init::envMercury,mercuryRadius*scale,100,100);
	glPopMatrix();

	// Rendering Venus
	float venus_orbit = 0.61*earth_orbit;
	glPushMatrix();
	glTranslatef(venus_x*cos(animationCounter/venus_orbit*PI/360),venus_y,venus_x*sin(animationCounter/venus_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::venusTexture);	
	gluSphere(Init::envVenus,venusRadius*scale,100,100);
	glPopMatrix();

	// Rendering Earth
	glPushMatrix();
	glTranslatef(earth_x2*cos(animationCounter/earth_orbit*PI/360),earth_y2,earth_x2*sin(animationCounter/earth_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::earthTexture2);	
	gluSphere(Init::envEarth2,earthRadius2*scale,100,100);
	glPopMatrix();

	// Rendering Moon
	float moon_orbit = 0.075*earth_orbit;
	glPushMatrix();
	moon_x2 = earth_x2*cos(animationCounter/earth_orbit*PI/360);
	moon_y2 = earth_y2;
	moon_z2 = earth_x2*sin(animationCounter/earth_orbit*PI/360);
	glTranslatef(earthRadius2*3*sin(animationCounter/moon_orbit*PI/360)+moon_x2,moon_y2,earthRadius2*3*cos(animationCounter/moon_orbit*PI/360)+moon_z2);
	glRotatef(90,1,0,0);
	glRotatef(animationCounter,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::moonTexture2);	
	gluSphere(Init::envMoon2,moonRadius2*scale,100,100);
	glPopMatrix();

	// Rendering Mars
	float mars_orbit = 1.88*earth_orbit;
	glPushMatrix();
	glTranslatef(mars_x2*cos(animationCounter/mars_orbit*PI/360),mars_y2,mars_x2*sin(animationCounter/mars_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::marsTexture2);	
	gluSphere(Init::envMars2,marsRadius2*scale,100,100);
	glPopMatrix();

	// Rendering Jupiter
	float jupiter_orbit = 11.87*earth_orbit;
	glPushMatrix();
	glTranslatef(jupiter_x*cos(animationCounter/jupiter_orbit*PI/360),jupiter_y,jupiter_x*sin(animationCounter/jupiter_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::jupiterTexture);	
	gluSphere(Init::envJupiter,jupiterRadius*scale,100,100);
	glPopMatrix();

	// Rendering Saturn
	float saturn_orbit = 29.48*earth_orbit;
	glPushMatrix();
	glTranslatef(saturn_x*cos(animationCounter/saturn_orbit*PI/360),saturn_y,saturn_x*sin(animationCounter/saturn_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::saturnTexture);	
	gluSphere(Init::envSaturn,saturnRadius*scale,100,100);
	glPopMatrix();

	// Rendering Uranus
	float uranus_orbit = 84.38*earth_orbit;
	glPushMatrix();
	glTranslatef(uranus_x*cos(animationCounter/uranus_orbit*PI/360),uranus_y,uranus_x*sin(animationCounter/uranus_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::uranusTexture);	
	gluSphere(Init::envUranus,uranusRadius*scale,100,100);
	glPopMatrix();

	// Rendering Neptune
	float neptune_orbit = 164.9*earth_orbit;
	glPushMatrix();
	glTranslatef(neptune_x*cos(animationCounter/neptune_orbit*PI/360),neptune_y,neptune_x*sin(animationCounter/neptune_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::neptuneTexture);	
	gluSphere(Init::envNeptune,neptuneRadius*scale,100,100);
	glPopMatrix();

	// Rendering Pluto
	float pluto_orbit = 248.25*earth_orbit;
	glPushMatrix();
	glTranslatef(pluto_x*cos(animationCounter/pluto_orbit*PI/360),pluto_y,pluto_x*sin(animationCounter/pluto_orbit*PI/360));
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::plutoTexture);	
	gluSphere(Init::envPluto,plutoRadius*scale,100,100);
	glPopMatrix();
}

void renderEnvironment(){
	// Rendering Spacedome
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glTranslatef(Camera::x+offset_x,Camera::y+offset_y,Camera::z+offset_z);
	glRotatef(90,1,0,0);
	glBindTexture(GL_TEXTURE_2D, Init::spaceTexture);
	gluSphere(Init::envSpace,spaceRadius,100,100);
	if(key_cull) glEnable(GL_CULL_FACE);
	if(key_lighting) glEnable(GL_LIGHTING);
	glPopMatrix();

	// Rendering Earth
	glPushMatrix();
	glTranslatef(earth_x,earth_y,earth_z);
	glRotatef(90,1,0,0);
	glBindTexture(GL_TEXTURE_2D, Init::earthTexture);	
	gluSphere(Init::envEarth,earthRadius,100,100);
	glPopMatrix();

	// Rendering Moon
	glPushMatrix();
	glTranslatef(earthRadius*3*sin(2*PI*animationCounter/600)+moon_x,moon_y,earthRadius*3*cos(2*PI*animationCounter/600)+moon_z);
	glRotatef(90,1,0,0);
	glRotatef(animationCounter,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::moonTexture);	
	gluSphere(Init::envMoon,moonRadius,100,100);
	glPopMatrix();

	// Rendering Sky
	glPushMatrix();	
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	float DistanceEarth = pow((pow(Camera::x-earth_x,2) + pow(Camera::y-earth_y,2) + pow(Camera::z-earth_z,2)),0.5f);
	float transparency = 0.0f;
	if(DistanceEarth < skyRadius) transparency = 1.0; 
	else transparency = 2 - DistanceEarth/skyRadius;
	glColor4f(1.0f,1.0f,1.0f,transparency);
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/4,0,0,1);
	glTranslatef(sky_x,sky_y,sky_z);
	if(!key_sky2) glBindTexture(GL_TEXTURE_2D, Init::skyTexture);
	else glBindTexture(GL_TEXTURE_2D, Init::skyTexture2);
	gluSphere(Init::envSky,skyRadius,100,100);
	glDisable(GL_BLEND);
	if(key_cull) glEnable(GL_CULL_FACE);
	if(key_lighting) glEnable(GL_LIGHTING);
	glPopMatrix();

	// Rendering Mars
	glPushMatrix();
	glTranslatef(mars_x,mars_y,mars_z);
	glRotatef(90,1,0,0);
	glRotatef(animationCounter/2,0,0,1);
	glBindTexture(GL_TEXTURE_2D, Init::marsTexture);	
	gluSphere(Init::envMars,marsRadius,100,100);
	glPopMatrix();
}

void DrawRain(){
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	for(int i = 0; i < RainCount; i++){
		Rain[i].Ypos = Rain[i].Ypos + Rain[i].acc;
		if(Rain[i].Ypos < sqrt(earthRadius*earthRadius - Rain[i].Xpos*Rain[i].Xpos - Rain[i].Zpos*Rain[i].Zpos)){
			if(rain){
			glColor3f(0.25, 0.25, 1);
			glPushMatrix();
			glTranslatef(Rain[i].Xpos, sqrt(earthRadius*earthRadius - Rain[i].Xpos*Rain[i].Xpos - Rain[i].Zpos*Rain[i].Zpos), Rain[i].Zpos);
			glBegin(GL_QUADS);
				glVertex3f(-2,0,0);
				glVertex3f(-2,1,0);
				glVertex3f(-1,1,0);
				glVertex3f(-1,0,0);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(2,0,0);
				glVertex3f(2,1,0);
				glVertex3f(1,1,0);
				glVertex3f(1,0,0);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0,0,2);
				glVertex3f(0,1,2);
				glVertex3f(0,1,1);
				glVertex3f(0,0,1);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0,0,-2);
				glVertex3f(0,1,-2);
				glVertex3f(0,1,-1);
				glVertex3f(0,0,-1);
			glEnd();
			glPopMatrix();
			}
			Rain[i].Xpos = rand()%(int)earthRadius - earthRadius/2;
			Rain[i].Ypos = earthRadius*1.5;
			Rain[i].Zpos = rand()%(int)earthRadius - earthRadius/2;
			Rain[i].Ymov = 0;
			Rain[i].Red = 0;
			Rain[i].Green = 0;
			Rain[i].Blue = 0.9;
			Rain[i].acc = -rand()%10-20;
		}
		if(rain){
		glColor3f(Rain[i].Red, Rain[i].Green, Rain[i].Blue);
		glPushMatrix();
		glTranslatef(Rain[i].Xpos, Rain[i].Ypos, Rain[i].Zpos);
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,30,0);
			glVertex3f(0.1,30,0);
			glVertex3f(0.1,0,0);
		glEnd();
		glPopMatrix();
		}
	}
	if(key_lighting) glEnable(GL_LIGHTING);
	if(key_cull) glEnable(GL_CULL_FACE);
}

void DrawDoor(float Tx=0, float Ty=0, float Tz=0, float Rx=0, float Ry=0, float Rz=0, float Sx = 1){
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glTranslatef(Tx,Ty,Tz);
	glRotatef(Rx,1,0,0);
	glRotatef(Ry,0,1,0);
	glRotatef(Rz,0,0,1);
	glScalef(Sx,1,1);
	glBindTexture(GL_TEXTURE_2D,Init::doorTexture);
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.5,0);
		glVertex3f(6.25,0,0);
		glTexCoord2f(0.5,0.5);
		glVertex3f(6.25,14,0);
		glTexCoord2f(0,0.5);
		glVertex3f(0,14,0);
		glTexCoord2f(0,0);
		glVertex3f(0,0,0);
	glEnd();
	if(key_cull) glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void thunder(){
	if(key_thunder){
		key_texture = 0;
		rain = 0;
	}
}

void forkedLightning(){
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex3f(450,3000,0);
		glVertex3f(430,3000,0);
		glVertex3f(390,2500,0);
		glVertex3f(410,2500,0);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,0.5);
		glVertex3f(455,3000,-1);
		glVertex3f(425,3000,-1);
		glVertex3f(388,2500,-1);
		glVertex3f(415,2500,-1);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex3f(410,2500,0);
		glVertex3f(390,2500,0);
		glVertex3f(250,2250,0);
		glVertex3f(270,2250,0);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,0.5);
		glVertex3f(415,2500,-1);
		glVertex3f(388,2500,-1);
		glVertex3f(245,2250,-1);
		glVertex3f(275,2250,-1);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex3f(270,2250,0);
		glVertex3f(250,2250,0);
		glVertex3f(430,2000,0);
		glVertex3f(450,2000,0);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,0.5);
		glVertex3f(275,2250,-1);
		glVertex3f(245,2250,-1);
		glVertex3f(425,2000,-1);
		glVertex3f(455,2000,-1);
	glEnd();
	glPopMatrix();
	glColor3f(1.0,1.0,1.0);
	if(key_cull) glEnable(GL_CULL_FACE);
	if(key_lighting) glEnable(GL_LIGHTING);
	if(key_texture) glEnable(GL_TEXTURE_2D);
}

void checkFog(){
	glFogf(GL_FOG_END, fog_end);
}

void animate(){
	float t;
	int mars_time = 720;

	if(animationCounter < 0){
		logo = 1;
		darkness = 1;
	}
	else if(animationCounter <= 135){
		logo = 0;
		darkness = 0;
		t = animationCounter;
		if(t == 22){
			caption = 1;
		}
		Camera::x = mars_x;
		Camera::y = mars_y+marsRadius+personHeight;
		Camera::z = mars_z;

		Camera::viewX = mars_x+sin(t*2*2*PI/360);
		Camera::viewY = mars_y+marsRadius+personHeight;
		Camera::viewZ = mars_z-1;
	}
	else if(animationCounter <= 180){
		t = animationCounter-135;
		Camera::viewX = mars_x+sin((t+135)*2*2*PI/360);
		Camera::viewY = mars_y+marsRadius+personHeight-sin(t*2*2*PI/360);
		Camera::viewZ = mars_z-1;
	}
	else if(animationCounter <= 200){
		caption = 0;
		Camera::viewX = mars_x;
		Camera::viewY = mars_y;
		Camera::viewZ = mars_z;
	}
	else if(animationCounter <= 360){
		t = animationCounter-200;
		if((int)animationCounter%2 == 0){
			Camera::x = mars_x + (marsRadius/100)*(160-t)/160;
		}
		else{
			Camera::x = mars_x - (marsRadius/100)*(160-t)/160;
		}
		Camera::y += 30;
		Camera::z += 10;
		
		prev_y = Camera::y;
	}
	else if(animationCounter <= 450){
		t = animationCounter - 360;	
		caption = 2;
		Camera::x = mars_x;
		if(-t+25 >= -30)
			Camera::y = 25*t - 0.5*1*t*t + prev_y;
		else
			Camera::y -= 30;
		Camera::z += 45;
		prev_x2 = Camera::x;
		prev_y2 = Camera::y;
		prev_z2 = Camera::z;
	}
	else if(animationCounter <= 1096){
		t = animationCounter - 450;
		if(Camera::y > 0){
			Camera::y = -30*t + 0.5*0.08*t*t + prev_y2;
		}
		else{
			Camera::y = 0;
		}
		if( t <= 323 ){
			caption = 3;
			Camera::x = (prev_z2-mars_z)*sin((sqrt(40000+t*t)-200)*2*PI/360)+mars_x;
			Camera::z = (prev_z2-mars_z)*cos((sqrt(40000+t*t)-200)*2*PI/360)+mars_z;
		}
		else if (t <= 646){
			caption = 0;
			if( t <= 484.5) Camera::y = 0.5*0.25*(t-324)*(t-324);
			else Camera::y = 40.125*(t-484.5)-0.5*0.25*(t-484.5)*(t-484.5)+3220;
			Camera::x = (prev_z2-mars_z)*sin((sqrt(40000+(646-t)*(646-t))-200)*2*PI/360+PI)+mars_x;
			Camera::z = (prev_z2-mars_z)*cos((sqrt(40000+(646-t)*(646-t))-200)*2*PI/360)+mars_z;
		}
		prev_z = Camera::z;
	}
	else if(animationCounter <= 1276){
		caption = 4;
		t = animationCounter - 1096;
		Camera::viewY = 6439*sin(t*PI/360);
		prev_dy = Camera::viewY;
	}
	else if(animationCounter <= 1516){
		if(animationCounter < 1456) caption = 5;
		else if(animationCounter < 1486) caption = 6;
		else if(animationCounter < 1516) caption = 7;
	}
	else if(animationCounter <= 1696){
		caption = 0;
		t = animationCounter - 1516;
		motionBlur = 1;
		brightness = t/180;
		offset_z += 30;
	}
	else if(animationCounter <= 1756){
		t = animationCounter - 1696;
		motionBlur = 0;
		Camera::x = earthRadius*6;
		Camera::y = earthRadius*6;
		Camera::z = 0.5*3.49*t*t-6283.2;
		Camera::viewX = earth_x;
		Camera::viewY = earth_y;
		Camera::viewZ = earth_z;
		brightness = 1.0 - t/60;
		offset_z = 0;
	}
	else if(animationCounter <= 1846){
		caption = 8;
		t = animationCounter - 1756;
		Camera::x = earthRadius*6*cos(t*2*PI/360);
		Camera::y = earthRadius*6;
		Camera::z = earthRadius*6*sin(t*2*PI/360);
	}
	else if(animationCounter <= 2116){
		caption = 0;
		t = animationCounter - 1756; 
		Camera::x = earthRadius*6*cos(t*2*PI/360);
		Camera::y -= earthRadius*8/270;
		Camera::z = earthRadius*6*sin(t*2*PI/360);
	}
	else if(animationCounter <= 2476){
		caption = 9;
		t = animationCounter-2116;
		Camera::x = earthRadius*6*cos(t*2*PI/360)*(1-t/480);
		Camera::z = earthRadius*6*sin(t*2*PI/360)*(1-t/480);
	}
	else if(animationCounter <= 2656){
		caption = 0;
		t = animationCounter-2116;
		Camera::x = earthRadius*1.5*cos(t*2*PI/360);
		Camera::y += earthRadius*2/180;
		Camera::z = earthRadius*1.5*sin(t*2*PI/360);
	}
	else if(animationCounter <= 2716){
		caption = 10;
		t = animationCounter - 2656;
		Camera::x = earthRadius*1.5 - earthRadius*0.1*t/60;
		Camera::y = 0;
		if( (int)t%2 == 0)	
			Camera::z = earthRadius/100;
		else if((int)t%2 == 1)
			Camera::z = -earthRadius/100;
		Camera::viewY = earthRadius*t/60;
		Camera::viewX = earth_x;
	}
	else if(animationCounter <= 2870){
		caption = 11;
		t = animationCounter-2716;
		Camera::x = earthRadius*1.4*cos(t*PI/360)*(1-t/540);
		Camera::y = earthRadius*1.4*sin(t*PI/360)*(1-t/540);
		if( (int)t%2 == 0)
			Camera::z = earthRadius/100;
		else if((int)t%2 == 1)
			Camera::z = -earthRadius/100;
		//Camera::viewX = Camera::x - sin(t*PI/360);
		//Camera::viewY = Camera::y + cos(t*PI/360)/2;
		Camera::viewX = earth_x;
		Camera::viewY = earth_y + earthRadius;
		Camera::viewZ = 0;
	}
	else if(animationCounter <= 2910){
		glEnable(GL_FOG);
		caption = 12;
		darkness = 1.0;
	}
	else if(animationCounter <= 2970){
		caption = 0;
		rain = 1;
		t = animationCounter - 2910;
		Camera::x = 0;
		Camera::y = earthRadius * cos(5*2*PI/360);
		Camera::z = earthRadius * sin(5*2*PI/360);
		Camera::viewX = Camera::x + sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
		darkness -= 0.004;
	}
	else if(animationCounter <= 3030){
		caption = 0;
		rain = 1;
		t = animationCounter - 2910;
		Camera::viewX = Camera::x + sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
	}
	else if(animationCounter <= 3090){
		rain = 1;
		t = animationCounter - 2910;
		Camera::viewX = Camera::x + sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
		darkness += 0.004;
	}
	else if(animationCounter <= 3150){
		caption = 13;
		rain = 1;
		t = animationCounter - 3090;
		Camera::x = 0;
		Camera::y = earthRadius * cos(5*2*PI/360);
		Camera::z = earthRadius * sin(5*2*PI/360);
		Camera::viewX = Camera::x - sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
		darkness -= 0.004;
	}
	else if(animationCounter <= 3210){
		rain = 1;
		t = animationCounter - 3090;
		Camera::viewX = Camera::x - sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
	}
	else if(animationCounter <= 3270){
		rain = 1;
		t = animationCounter - 3090;
		Camera::viewX = Camera::x - sin(t*PI/360);
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
		darkness += 0.004;
	}
	else if(animationCounter <= 3390){
		caption = 14;
		rain = 1;
		Camera::x = 0;
		Camera::y = earthRadius * cos(5*2*PI/360);
		Camera::z = earthRadius * sin(5*2*PI/360);
		Camera::viewX = Camera::x;
		Camera::viewY = Camera::y + 1;
		Camera::viewZ = Camera::z + 1;
		darkness -= 0.008;
	}
	else if(animationCounter <= 3420){
		rain = 1;
		t = animationCounter - 3390;
		darkness = 0.0;
		Camera::viewY = Camera::y + (1-t/20);
	}
	else if(animationCounter <= 3450){
		caption = 15;
		t = animationCounter - 3420;
		Camera::y = earthRadius * cos(5*2*PI/360) + personHeight*(t/15);
		Camera::viewY = Camera::y - 0.5*(1-t/30); // 0.75
	}
	else if(animationCounter <= 3510){
		t = animationCounter - 3450;
		Camera::viewX = Camera::x + sin(t*3*2*PI/360);
		Camera::viewY = Camera::y;
		Camera::viewZ = Camera::z + cos(t*3*2*PI/360);
	}
	else if(animationCounter <= 3570){
		Camera::viewX += 0.02;
		Camera::viewY += 0.05;
	}
	else if(animationCounter <= 3575){
		key_sky2 = 1;
	}
	else if(animationCounter <= 3580){
		caption = 0;
		key_sky2 = 0;
		key_thunder = 1;
	}
	else if(animationCounter <= 3700){
		t = animationCounter - 3580;
		key_thunder = 0;
		key_texture = 1;
		rain = 1;
		brightness = (1-t/120);
	}
	else if(animationCounter <= 3765){
	}
	else if(animationCounter <= 3825){
		caption = 16;
		Camera::viewX -= 0.04;
	}
	else if(animationCounter <= 3885){
		Camera::viewX += 0.02;
		Camera::viewY -= 0.045;
	}
	else if(animationCounter <= 3900){
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.50;
		Camera::y += 0.30;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3910){
		Camera::viewY -= 0.3;
		Camera::viewZ -= 0.50;
		Camera::y -= 0.3;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3925){
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.50;
		Camera::y += 0.30;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3935){
		Camera::viewY -= 0.3;
		Camera::viewZ -= 0.50;
		Camera::y -= 0.3;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3950){
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.50;
		Camera::y += 0.30;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3960){
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.50;
		Camera::y -= 0.30;
		Camera::z -= 0.50;
	}
	else if(animationCounter <= 3975){
		Camera::viewX -= 0.3;
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.50;
		Camera::x -= 0.3;
		Camera::y += 0.30;
		Camera::z -= 0.30;
	}
	else if(animationCounter <= 3985){
		Camera::viewX -= 0.3;
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.30;
		Camera::x -= 0.3;
		Camera::y -= 0.30;
		Camera::z -= 0.30;
	}
	else if(animationCounter <= 4030){
		fog_end = 250;
		caption = 0;
		t = animationCounter - 3985;
		Camera::x = -7.5;
		Camera::y = 2014.39;
		Camera::z = 129.31;
		Camera::viewX = -7.49998;
		Camera::viewY = 2014.69;
		Camera::viewZ = 125.31;
		rain = 0;
		doorY = t*2;
	}
	else if(animationCounter <= 4040){
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.40;
		Camera::y += 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4050){
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.40;
		Camera::y -= 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4060){
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.40;
		Camera::y += 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4070){
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.40;
		Camera::y -= 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4075){
		caption = 17;
		light0 = 0;
		doorY = 0;
	}
	else if(animationCounter <= 4090){
		t = animationCounter - 4075;
		doorY = 0;
		Camera::y = 2015;
		Camera::viewX = Camera::x + sin(t*12*2*PI/360);
		Camera::viewY = Camera::y;
		Camera::viewZ = Camera::z - cos(t*12*2*PI/360);
	}
	else if(animationCounter <= 4120){
		caption = 18;
	}
	else if(animationCounter <= 4180){
		t = animationCounter - 4120;
		Camera::y = 2015;
		Camera::viewX = Camera::x - sin(-t*3*2*PI/360);
		Camera::viewZ = Camera::z + cos(-t*3*2*PI/360);
	}
	else if(animationCounter <= 4200){
		caption = 19; 
		glEnable(GL_LIGHT1);
	}
	else if(animationCounter <= 4210){
		caption = 20;
		Camera::viewX += 0.35;
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.40;
		Camera::x += 0.30;
		Camera::y += 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4220){
		Camera::viewX += 0.30;
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.40;
		Camera::x += 0.3;
		Camera::y -= 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4230){
		Camera::viewX += 0.30;
		Camera::viewY += 0.30;
		Camera::viewZ -= 0.40;
		Camera::x += 0.30;
		Camera::y += 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4240){
		caption = 0;
		Camera::viewX -= 0.05;
		Camera::viewY -= 0.30;
		Camera::viewZ -= 0.40;
		Camera::y -= 0.30;
		Camera::z -= 0.40;
	}
	else if(animationCounter <= 4400){
		caption = 30;
		t = animationCounter - 4240;
		Camera::viewZ -= 0.40;
		Camera::z -= 0.40;
		if( (int)((t+10)/10)%2 != 0 ){
			Camera::viewY += 0.30;
			Camera::y += 0.30;
		}
		else{
			Camera::viewY -= 0.30;
			Camera::y -= 0.30;
		}
	}
	else if(animationCounter <= 4420){
		caption = 21;
		Camera::viewY += 0.005;
	}
	else if(animationCounter <= 4480){
	}
	else if(animationCounter <= 4840){
		t = animationCounter - 4480;
		Camera::viewX = Camera::x + sin(t*2*PI/360);
		Camera::viewZ = Camera::z - cos(t*2*PI/360);
		if(t < 90){
		}
		else if(t < 180){
			Camera::viewY += 0.0025; 
		}
		else if(t < 270){
			Camera::viewY -= 0.0025;
		}

		if(t < 60) caption = 22;
		else if(t < 120) caption = 23;
		else if(t < 180) caption = 24;
		else if(t < 240) caption = 25;
		else if(t < 300) caption = 26;
	}
	else if(animationCounter <= 5080){
		caption = 27;
		Camera::viewY += 0.0009;
	}
	else if(animationCounter <= 5310){
		caption = 28;
		t = animationCounter - 5080;
		solar = 1;
		light2 = 1;
		ambient = 2;
		glDisable(GL_LIGHT1);
		SolarScreenY = t/5;
	}
	else if(animationCounter <= 5500){
		light2 = 1;
		ambient = 2;
		caption = 29;
		credits = 1;
	}
	else{
		system("pause");
		if(recordMovie) exit(0);
		else animateMovie = 0;
	}
}

void Display(void)
{
	checkFog();
	Init::SetLighting();
	thunder();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();


	animateMovie ? Draw::animate() : Camera::computeCamera();
	
	animationCounter++;
	gluLookAt(
		Camera::x, Camera::y, Camera::z,
		Camera::viewX, Camera::viewY, Camera::viewZ,
		Camera::x_up, Camera::y_up, Camera::z_up
		);	

	if(!key_lighting){ glDisable(GL_LIGHTING); glColor3f(1.0,1.0,1.0); }
	
	// Generate Models
	for(GLuint i = Init::list_index; i < Init::list_index+Init::list_index_size; i++){
		glCallList(i);
	}

	if(key_sky2) forkedLightning();

	DrawDoor(-12.5,2007.5,125,0,doorY,0);
	DrawDoor(0,2007.5,125,0,-doorY,0,-1);
	DrawDoor(2.5,2007.5,125);
	DrawDoor(15,2007.5,125,0,0,0,-1);

	renderEnvironment();

	if(solar){
		glPushMatrix();
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(0.0,SolarScreenY,0.0);
		glColor4f(1.0f,1.0f,1.0f,0.0f);
		glBegin(GL_QUADS);
			glVertex3f(-100,2000,0.0);
			glVertex3f(100,2000,0.0);
			glVertex3f(100,2100,0.0);
			glVertex3f(-100,2100,0.0);
		glEnd();
		glDisable(GL_BLEND);
		if(key_texture) glEnable(GL_TEXTURE_2D);
		if(key_cull) glEnable(GL_CULL_FACE);
		if(key_lighting) glEnable(GL_LIGHTING);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(1.5,2040,-50);
		glScalef(0.0005,0.0005,0.0005);
		glRotatef(30,0,0,1);
		glRotatef(45,1,0,0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); 
		glColor3f(0.0,0.0,1.0);
		renderSolarSystem(0.9);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
		if(key_texture) glEnable(GL_TEXTURE_2D);
		if(key_lighting) glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(0.0,SolarScreenY,0.0);
		glColor4f(1.0f,1.0f,1.0f,0.0f);
		glBegin(GL_QUADS);
			glVertex3f(-100,1998,0.0);
			glVertex3f(100,1998,0.0);
			glVertex3f(100,2098,0.0);
			glVertex3f(-100,2098,0.0);
		glEnd();
		glDisable(GL_BLEND);
		if(key_texture) glEnable(GL_TEXTURE_2D);
		if(key_cull) glEnable(GL_CULL_FACE);
		if(key_lighting) glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.5,2040,-50);
		glScalef(0.0005,0.0005,0.0005);
		glRotatef(30,0,0,1);
		glRotatef(45,1,0,0);
		renderSolarSystem();
		glPopMatrix();
		
	}

	DrawRain();
	if(motionBlur){
		glAccum(GL_MULT,0.99);
		glAccum(GL_ACCUM,1-0.99);
		glAccum(GL_RETURN,1.0);
	}
		
	writeInformation();

	if(recordMovie){
		int counter = (int)animationCounter + 150;
		char recordname[9];
		string dir = "Movie//";
		if( counter < 10 ) sprintf_s(recordname, "000%i.ppm", counter);
		else if( counter < 100 ) sprintf_s(recordname, "00%i.ppm", counter);
		else if( counter < 1000 ) sprintf_s(recordname, "0%i.ppm", counter);
		else if( counter < 10000 ) sprintf_s(recordname, "%i.ppm", counter);
		recordScene(dir+recordname);
	}
	glFlush();
	glutSwapBuffers();
}

} // namespace Draw

void myTimer(int value){
	glutPostRedisplay();
	glutTimerFunc(frames_per_second,myTimer,0);
}

int main(int argc, char **argv)
{
	string Title = "EECE478 Assignment 3";
	std::cout << endl;
	std::cout << "------------------------" << endl;
	std::cout << Title << endl;
	std::cout << "Name:\tWilbur Yu" << endl;
	std::cout << "Number:\t15556079" << endl;
	std::cout << "------------------------" << endl;
	std::cout << endl;

	if(debug){
		std::cout << "DEBUG MODE" << endl << endl;
		Init::cityname =
			"myCity.city";
			//"2011.city";
			//"2010.city";
			//"Gaya.city";
			//"Doug.city";
	}
	
	std::cout << "Loading Argument(s)...";
	if( argc == 1 && !debug ){
		std::cout << "FAILED: Invalid or No Model Pathname Specified" << endl;
	}
	else
	{
		std::cout << "COMPLETE" << endl;
		if( !debug ){
			Init::cityname = argv[1];
			if(Init::cityname[0] == '.' && Init::cityname[1] == '/') Init::cityname.insert(0,"."); // If input file is './dir', change it to '../dir'
			string filename(Init::cityname);
		}

		// Appending Model Name onto Title
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM);
		glutInitWindowPosition(Window_Position_X, Window_Position_Y);
		glutInitWindowSize(Window_Width, Window_Height);
		glutCreateWindow(Title.c_str());
		std::cout << "GLUT Version: " << glGetString(GL_VERSION) << endl << endl;

		std::cout << "Animate Movie? (0 / 1)" << endl;
		std::cin >> animateMovie;

		if(animateMovie){
			std::cout << "Record Movie? (0 / 1)" << endl;
			std::cin >> recordMovie;
		}

		Init::Initialize();
		Init::Instructions();
		
		glutTimerFunc(0,myTimer,0);
		glutDisplayFunc(Draw::Display);	
		glutReshapeFunc(Draw::changeSize);
		glutIdleFunc(Draw::Display);
		glutMainLoop();
	}
	if(debug) _getch();
	return 1;
}