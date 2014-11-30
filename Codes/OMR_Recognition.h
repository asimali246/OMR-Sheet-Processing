#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <cmath>
#include <iostream>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

 using namespace std;
 using namespace cv;

/*
	pair is use to store a pair of 
	information!!
	
	Used to store the number of black pixels and 
	total number of pixels in a cell!!

	first -> stores first parameter.
	second -> stores second parameter.
*/

typedef pair<int, int> PAIR;

class OMR {

private:
	
	/*
		It contains the declaration of data members private to this
		class. 
			
	*/

	Mat omr;
	
	string input_path, output_path, path;

	int omr_x, omr_y ;
	
	char pixel[5], character;
	
	int number_of_cells, number_of_sections, number_of_questions;
	
	bool multiple_choice;

	PAIR pixel_density[10];
	
	double density;
	
	int section_start_x, section_start_y, cell_diff_x, cell_diff_y, block_diff_x, section_diff_y, section_diff_x; // X denotes rows, Y denotes Columns 

	int id_startx, id_starty, id_diff_x, id_diff_y;

	int displacement_x, displacement_y, dx, dy ;

public:

	OMR() ;

private:
	
	void parameters_read() ; // Read the Input Parameters
	
	void omr_input_path_init() ; //Input path to images and output path to txt files.
	
	void omr_read() ; //Read the OMR Sheeet
	
	inline int get_pixel_color_value( int x, int y ) ; // Get pixel color code.
	
	void section_processing( int startx, int starty ) ; // Main Process
	
	void omr_processing() ; //Generate the text files of all the sections
	
	void id_processing() ; // To calculate the id of the OMR sheet



	void omr_output_path_init() ;  //Input path to txt files and output to a csv file.
	
	void answer_evaluation() ; // Evaluate generated text files.

	void id_evaluation( FILE * ) ; // to evalute the id of the OMR sheet

	void search_displacement() ;

	void remove_smudges() ;

	void debug() ; //Only used for debugging!! Remove after completion!!

public:

	void omr_evaluate() ; // Generate the output csv file of the OMR folder.

	void compute_skew() ;//To compute the skew angle of the OMR sheet

	void rotate( Mat &src, double angle, Mat &dst ) ;//Rotates the OMR sheet by given angle

	//void adjust_brightness() ;

};