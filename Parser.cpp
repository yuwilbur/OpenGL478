/*
	Parser.cpp

	Name: Wilbur Yu
	Student Number: 15556079
	
	Class: EECE478
	Instructor: Dr. Gregor Miller
	Teacher's Assistant: Andrew Ho
	Assignment: 3

	DESCRIPTION:
		Contains: 	Loading Model Files
					Parser
					Creating Parsed Model File (For Debugging)
*/

#include "Parser.h"

using namespace std;

void Parser::NORMALIZE(Model debugModel){	
	cout << "\tCreating NORMALIZE File...";
	float max_x = debugModel.GetXmax();
	float min_x = debugModel.GetXmin();
	float max_y = debugModel.GetYmax();
	float min_y = debugModel.GetYmin();
	float max_z = debugModel.GetZmax();
	float min_z = debugModel.GetZmin();
	
	ofstream outfile("NORMALIZE.txt",ios::app); //DEBUG Comment out if NOT Debugging
	
	float x_avg = (max_x+min_x)/2;
	float y_avg = (max_y+min_y)/2;
	float z_avg = (max_z+min_z)/2;

	outfile << debugModel.GetPath() << "\t" << x_avg << "\t" << y_avg << "\t" << z_avg << "\t" << min_x << "\t" << min_y << "\t" << min_z << "\t" << max_x << "\t" << max_y << "\t" << max_z << endl;

	outfile.close();	
	cout << "COMPLETE" << endl;
}

void Parser::DEBUG(Model debugModel){	
	cout << "\tCreating DEBUG File...";
	string * texture = debugModel.GetTextureFile();
	float ** vertice = debugModel.GetVertice();
	float ** normal = debugModel.GetNormal();
	float *** triangle = debugModel.GetTriangle();
	int * size = debugModel.GetSize(); 

	ofstream outfile(debugModel.GetName(),ios::out); //DEBUG Comment out if NOT Debugging
	
	outfile << name << endl << endl;

	outfile << "<Textures> " << texture_size << endl;
	for( int i = 0; i < texture_size ; i++ ){
		outfile << texture[i] << endl;
	}
	outfile << endl;

	outfile << "<Vertices> " << vertice_size << endl;
	for( int i = 0; i < vertice_size; i++ ){
		for( int j = 0; j < 3 ; j++ ){
			outfile << vertice[i][j] << " ";
		}
		outfile << endl;
	}
	outfile << endl;

	outfile << "<Normals> " << normal_size << endl;
	for( int i = 0; i < normal_size ; i++ ){
		for( int j = 0; j < 3 ; j++ ){
			outfile << normal[i][j] << " ";
		}
		outfile << endl;
	}
	outfile << endl;

	outfile << "<Triangles (Sorted)> " << triangle_size << endl;
	for( int i = 0; i < texture_size ; i++ ){
		for( int j = 0; j < size[i] ; j++ ){
			for( int k = 0; k < 11 ; k++ ){
				outfile << triangle[i][j][k] << " ";
			}
			outfile << endl;
		}
		outfile << endl << endl << endl;
	}

	outfile.close();	
	cout << "COMPLETE" << endl;
}

Model Parser::Parse(string path, bool normalize, bool debug)
{
	clock_t Start_t, End_t;
	// Values that will represent the row values for START and FINISH of NAME, TEXTURE, VERTICES, NORMAL and TRIANGLES
	name = "";
	string * texture_data_ptr = 0;
	int texture_data_x = 0;
	float** vertice_data_ptr = 0;
	int vertice_x = -1;
	int vertice_y = 0;
	float** normal_data_ptr = 0;
	int normal_x = -1;
	int normal_y = 0;
	float** temp_triangle_ptr = 0;
	float*** triangle_data_ptr = 0;
	int triangle_x = -1;
	int triangle_y = 0;
	int * texture_loc = 0;
	int * size = 0;

	// texture_pts share same y coordinates with triangle_data

	// Variables to find size of Name, Texture, Vertice, Normal and Triangle
	bool name_begin = 0;
	bool name_end = 0;
	bool texture_begin = 0;
	bool texture_end = 0;
	texture_size = 0;
	bool vertice_begin = 0;
	bool vertice_end = 0;
	vertice_size = 0;
	bool normal_begin = 0;
	bool normal_end = 0;
	normal_size = 0;
	bool triangle_begin = 0;
	bool triangle_end = 0;
	triangle_size = 0;

	// Maximum/Minimum Coordinates of X, Y and Z
	float x_max = 0;
	float x_min = 0;
	float y_max = 0;
	float y_min = 0;
	float z_max = 0;
	float z_min = 0;
	
	cout << path << endl;
	ifstream myfile(path, ios::in);
	
	if (!myfile.is_open())
	{
		cout << "FAILED: Invalid file" << endl;
		cout << "If model is in a sister directory, use: 'SisterDir/Model.model'" << endl;
		cout << "If model is in a parallel directory, use: '../ParallelDir/Model.model' or './ParallelDir/Model.model'" << endl;
		system("pause");
		exit(0);
	}
	else 
	{
		Start_t = clock();
		int checkEmpty;
		int state = 0;
		string line;
		int line_pos;

		while(myfile.good())
		{
			getline(myfile, line);
			checkEmpty = 0;
			line_pos = 0;
			state = 0;

			if(vertice_begin && !vertice_end && vertice_x != 0){
				vertice_x = 0;
				vertice_data_ptr[vertice_y] = new float[3];
			}
			else if(normal_begin && !normal_end && normal_x != 0){
				normal_x = 0;
				normal_data_ptr[normal_y] = new float[3];
			}
			else if(triangle_begin && !triangle_end && triangle_x != 0){
				triangle_x = 0;
				temp_triangle_ptr[triangle_y] = new float[11];
			}

			for( unsigned int i = 0 ; i < line.size() ; i++)
			{
				// All family headers start with '<' so if found, it means it's the beginning or end of a family
				if( line[i] == '<' )
				{
					//Assign Values to NAME, TEXTURE, VERTICES, NORMAL and TRIANGLES variables. If is in this order because these families appear in this order
					if(!name_begin){ name_begin = 1; }
					else if(!name_end){ name_end = 1; }
					else if(!texture_begin){  texture_begin = 1; cout << "\tTextures"; }
					else if(!texture_end){	texture_end = 1;}
					else if(!vertice_begin){ vertice_begin = 1; cout << "\tVertices"; }
					else if(!vertice_end){ vertice_end = 1;}
					else if(!normal_begin){ normal_begin = 1; cout << "\tNormals";}
					else if(!normal_end){ normal_end = 1;}
					else if(!triangle_begin){ triangle_begin = 1; cout << "\tTriangles"; }
					else if(!triangle_end){ triangle_end = 1;}
					state = 2; //Go to State 2 to initialize array and size
				}
				
				// Logic: Ignore SPACE/TAB until a char (start-of-value) is found. Find SPACE/TAB/EOF which determines the end-of-value. 
				// Assign begin-of-char to end-of-char to a variable
				if( state == 0 && line[i] != ' ' && line[i] != '\t')
				{
					if( line[i] == '#' )
						break;
					//State 0: Find first position of value (i.e. 1 of 123). Go to State 1
					checkEmpty = 1; //If a nonSPACE/TAB is found, it means the line is not empty
					line_pos = i;
					state = 1;
				}

				if( state == 1 )
				{
					//State 1: Find end position of the value (i.e. 3 of 123) then assign (line(end_pos) to line(first_pos)) to active array. Go to State 0 after to look for next value
					if( line[i] == ' ' || line[i] == '\t' || i == line.size()-1 )
					{
						if( i == line.size()-1 && line[i] != ' ' && line[i] != '\t'){ i++; }

						// These IF Statements determines what is the active array
						if( name_begin && !name_end) //Set NAME as active if name_begin has reached but not name_end
						{
							name += line.substr(line_pos,i-line_pos+1);
						}
						else if( texture_begin && !texture_end ){
							//if(texture_data_x == texture_size){std::cout << " ERR[TEXTURE]: INDEX > SIZE"; system("pause"); exit(0);}
							texture_data_ptr[texture_data_x] = line.substr(line_pos,i-line_pos);
							texture_data_x++;
						}
						else if( vertice_begin && !vertice_end ) //Set NAME as active if name_begin has reached but not name_end
						{ 
							//if(vertice_y == vertice_size){std::cout << " ERR[VERTICE]: INDEX > SIZE"; system("pause"); exit(0);}
							vertice_data_ptr[vertice_y][vertice_x] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							if(normalize){
								if(vertice_y == 0 && vertice_x == 0){ 
										x_min = vertice_data_ptr[0][0]; 
										x_max = vertice_data_ptr[0][0]; 
									}
								if(vertice_x == 0){ // Assign to Col 0								
									if(x_max < vertice_data_ptr[vertice_y][vertice_x]) x_max = vertice_data_ptr[vertice_y][vertice_x];
									if(x_min > vertice_data_ptr[vertice_y][vertice_x]) x_min = vertice_data_ptr[vertice_y][vertice_x];
								}
								else if(vertice_x == 1){ // Assign to Col 1
									if(y_max < vertice_data_ptr[vertice_y][vertice_x]) y_max = vertice_data_ptr[vertice_y][vertice_x];
									if(y_min > vertice_data_ptr[vertice_y][vertice_x]) y_min = vertice_data_ptr[vertice_y][vertice_x];
								}
								else if(vertice_x == 2){ // Assign to Col 2
									if(z_max < vertice_data_ptr[vertice_y][vertice_x]) z_max = vertice_data_ptr[vertice_y][vertice_x];
									if(z_min > vertice_data_ptr[vertice_y][vertice_x]) z_min = vertice_data_ptr[vertice_y][vertice_x];
								}
							}
							vertice_x++;
						}
						else if(normal_begin && !normal_end) //Set NORMAL as active if normal_begin has reached but not normal_end
						{
							//if(normal_y == normal_size){std::cout << " ERR[NORMAL]: INDEX > SIZE"; system("pause"); exit(0);}
							normal_data_ptr[normal_y][normal_x] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							normal_x++;
						} 
						else if(triangle_begin && !triangle_end) //Set TRIANGLE as active if triangle_begin has reached but not triangle_end
						{
							//if(triangle_y == triangle_size){std::cout << " ERR[TRIANGLE]: INDEX > SIZE"; system("pause"); exit(0);}
							temp_triangle_ptr[triangle_y][triangle_x] = (float)atof(line.substr(line_pos,i-line_pos).c_str());
							if( triangle_x == 4){
								texture_loc[(int)temp_triangle_ptr[triangle_y][triangle_x]]++;
							}
							triangle_x++;
						}
						state = 0;
					}	
				}
				// State 2: If '<' is found, then you're at the beginning of family or end of family. If "=" is found, it means beginning of a family. Go to State 3 to determine size of array
				else if(state == 2 && line[i] == '='){
					state = 3;
				}
				// State3: Scan until it's not SPACE/TAB which means it's beginning of family_size. Go to State 4 to assign family_size
				else if(state == 3 && line[i] != ' ' && line[i] != '\t')
				{
					line_pos = i;
					state = 4;
				}
				else if(state == 4 && line[i] == '>'){
					if( texture_begin && !texture_end ){
						texture_size = atoi(line.substr(line_pos,i-line_pos).c_str());
						texture_data_ptr = new string[texture_size];
						texture_loc = new int[texture_size];
						size = new int[texture_size];
						triangle_data_ptr = new float**[texture_size];
						for( int i = 0; i < texture_size ; i++ ){
							texture_loc[i] = 0;
						}
						cout << "[" << texture_size << "]";
					}
					if( vertice_begin && !vertice_end ){ //If Vertice is active, assign array size
						vertice_size = atoi(line.substr(line_pos,i-line_pos).c_str());
						vertice_data_ptr = new float*[vertice_size];
						cout << "[" << vertice_size << "]";
					}
					if( normal_begin && !normal_end ){ //If Normal is active, assign array size
						normal_size = atoi(line.substr(line_pos,i-line_pos).c_str());
						normal_data_ptr = new float*[normal_size];
						cout << "[" << normal_size << "]";
					}
					if( triangle_begin && !triangle_end ){ //If Triangle is active, assign array size
						triangle_size = atoi(line.substr(line_pos,i-line_pos).c_str());
						temp_triangle_ptr = new float*[triangle_size];
						cout << "[" << triangle_size << "]" << endl;
					}
					break;	
				}
			}

			// If the line is not empty, assign new array for next line of data
			if(checkEmpty){
				if( vertice_y < vertice_size && !vertice_end){ vertice_y++;}
				else if( normal_y < normal_size && !normal_end){ normal_y++; }
				else if( triangle_y < triangle_size && !triangle_end){ triangle_y++; }
			}		
		}

		cout << "\tSorting Data...";
		if(texture_size == 0){
			cout << "FAILED: Needs a non-zero Texture size" << endl;
			system("pause");
			exit(0);
		}
		for( int i = 0; i < texture_size ; i++ ){
			triangle_data_ptr[i] = new float*[texture_loc[i]];
		}

		for( int i = 0; i < texture_size ; i++ ){
			size[i] = texture_loc[i];
		}

		for(int i = 0; i < triangle_size; i++){
			if(temp_triangle_ptr[i][4] >= texture_size){std::cout << "ERR[SORT]: TEXTURE CALL OUTSIDE OF RANGE"; system("pause"); exit(0);}
			texture_loc[(int)temp_triangle_ptr[i][4]]--;
			triangle_data_ptr[(int)temp_triangle_ptr[i][4]][texture_loc[(int)temp_triangle_ptr[i][4]]] = temp_triangle_ptr[i];
		}

		int pathCounter = 0;
		for(pathCounter = path.length()-1; pathCounter > 0; pathCounter--){
			if(path[pathCounter] == '\\' || path[pathCounter] == '/') break;
		}
		path = path.substr(0,pathCounter+1);
		End_t = clock();
		cout << "COMPLETE [" << (float)(End_t-Start_t)/(1000.0) << "s]";
		if(debug) DEBUG(Model(name, texture_data_ptr, vertice_data_ptr, normal_data_ptr, triangle_data_ptr, size, texture_size, x_max, x_min, y_max, y_min, z_max, z_min, path));
		if(normalize) NORMALIZE(Model(name, texture_data_ptr, vertice_data_ptr, normal_data_ptr, triangle_data_ptr, size, texture_size, x_max, x_min, y_max, y_min, z_max, z_min, path));
	}
	return Model(name, texture_data_ptr, vertice_data_ptr, normal_data_ptr, triangle_data_ptr, size, texture_size, x_max, x_min, y_max, y_min, z_max, z_min, path);
}