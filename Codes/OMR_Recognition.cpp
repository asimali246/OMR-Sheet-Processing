#include "OMR_Recognition.h"

OMR::OMR() { 
	//Initialization

	density = 0.0 ;
	input_path = output_path = "" ;
	path = "" ;

}

void OMR::remove_smudges() {
	
	int i, j, color ;
	
	for( i = 0; i < omr_x ; ++i ) {
	
		for( j = 0; j < omr_y; ++j ) {
		
			color = get_pixel_color_value( i, j ) ;

			if( color >= 225 ) {

				omr.at<uchar>( Point( j, i ) ) = 255 ;
			
			}
		
		}
	}

	//imwrite( "C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/new.jpg",  omr) ;
	
}


void OMR::omr_read() {
	
	DIR *directory ;
	struct dirent *entry ;

	if( directory = opendir( input_path.c_str() ) ) { 

		while( entry = readdir( directory ) ) {
			
			if( strcmp( entry -> d_name , "Input" ) == 0 )
				continue ;

			if( strcmp( entry -> d_name, "." ) == 0 )
				continue ;
			
			if( strcmp( entry -> d_name, ".." ) == 0 )
				continue ;
			
			string name = entry -> d_name ;
			
			omr = imread( input_path + name , -1 ); 
			
			string output = name.substr( 0, name.find(".") ) + ".txt" ;
			
			freopen( ( output_path + output ).c_str(), "w", stdout ) ;

			omr_x = omr.rows ;
			omr_y = omr.cols ;

			dx = dy = 0 ;
			
			remove_smudges() ;
			
			search_displacement() ;

			id_processing() ;

			omr_processing() ;

			putchar('\n') ;

		}

	} else {

		printf( "The given directory is not present!!\n" ) ;
		printf( "Please make sure that the specified directory to the OMR images is CORRECT!!\n" ) ;
		throw "Error" ;

	}
}


void OMR::search_displacement() {
	compute_skew() ;

	int i, j, displacement_y = 0 ;
	int x, y, color ;

	x = omr_x / 4 ;
	j = 1 ; 

	while( j <= 3 ) {

			for( i = 10; ; ++i ) {

				color = get_pixel_color_value(x, i) ;
				
				if( color <= 100 ) 
					break ;

				if( i > 500 ) 
					break ;
			}

			displacement_y += i ;
			++j ;
			x += ( omr_x / 4 ) ;

	}
	displacement_y /= 3 ;
	
	y = displacement_y ;
	displacement_y -= 225 ;

	y = 371 + displacement_y ;

	for( i = 0; ; ++i ) {

		color = get_pixel_color_value( i, y ) ;
		
		if( color <= 200 ) 
			break ;
		
		if( i > 500 ) 
			break ;

	}

	displacement_x = i ;

	displacement_x -= 130 ;
	
	dx = displacement_x ;
	dy = displacement_y ;

}

void OMR::debug() {  
	
	omr = imread( "C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/scan0001.jpg" , -1 ) ;
	
	for( int i = 1; i <= 10; ++i ) {

		Scalar v = omr.at<uchar>( 370, 546 + i ) ;
		cout << v << endl ;

	}

}

void OMR::omr_input_path_init() { 

	path += "/" ;
	
	input_path = path ;
	
	string temp = "" ;

	_mkdir("Input") ;
	system("rmdir /S /Q Input") ;

	_mkdir( "Input" ) ;
	temp = "Input/" ;

	output_path = temp ;

}

int OMR::get_pixel_color_value( int x, int y ) {  

	x += dx ;
	y += dy ;

	Scalar Location;
	
	Location = omr.at<uchar>( Point( y, x ) ) ;
	
	return Location[0] ;

}

void OMR::parameters_read() {
	try {

		if( ! freopen( "input.txt", "r", stdin ) )
			throw "Error" ;
		
		}catch(...) {

			printf( "Input file not found error!!\n" ) ;
			printf( "Please make sure that 'input.txt' file is present in the path where your program is!!\n" ) ;
			throw "Error" ;

		} ;

	try {

		char s[1010], foo[1010] ;
		
		int l, i ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %d %s", &id_startx, &id_starty, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &id_diff_x, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &id_diff_y, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %d %s", &section_start_x, &section_start_y, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &cell_diff_x, foo ) ;

		if( !gets(s) ) 
			throw "Error" ;
		sscanf( s, "%d %s", &cell_diff_y, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &block_diff_x, foo) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &number_of_cells, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &number_of_questions, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &number_of_sections, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &section_diff_x, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		sscanf( s, "%d %s", &section_diff_y, foo ) ;

		if( !gets(s) )
			throw "Error" ;
		path = s;

	}
	catch(...) {

		printf( "Format of 'input.txt' does not match the required format!!\n" ) ;	
		throw "Error" ;

	} ;
}


void OMR::id_processing() {
	
	int startx, starty, sx, sy, i, pi, pj ;
	
	startx = id_startx ;
	starty = id_starty ;

	for( i = 1; i <= 16; ++i ) {   
	
		sx = startx ;
		sy = starty + ( i - 1 ) * ( id_diff_y + 5 ) ;

		if( i >= 6 ) {

			sy = sy + 2 ;
			sx = sx - 2 ;
			sx = sx - 2 ;

		}

		if( i >= 10 ) {
			
			sy = sy + 2 ;
		
		}

		for( pi = 1 ; pi <= id_diff_y ; ++pi ) 

			for( pj = 1 ; pj <= id_diff_x ; ++pj ) 

				printf( "%dx", get_pixel_color_value( sx + pj, sy + pi ) ) ;
	
		printf("n") ;

	}

}


void OMR::section_processing( int startx, int starty ) {

	int i, j, k, pi, pj ;
	int sx, sy, f = 1, s = 1 ;  

	for( i = 1 ; i <= number_of_questions ; ++i ) { 
	
		sx = startx + (i - 1) * block_diff_x ; 
		
		for( j = 1 ; j <= number_of_cells ; ++j ) {  
			
			sy = starty + (j - 1) * ( cell_diff_y + 4 ) ;
				
			for( pi = 1 ; pi <= cell_diff_y ; ++pi ) { 
				
				for( pj = 1 ; pj <= cell_diff_x ; ++pj ) {  
				
					printf( "%dx", get_pixel_color_value( sx + pj, sy + pi ) ) ;

				}
			
			}
			
			printf("n") ;
			
		} 

		if( f == ( 6 * s ) ) { 

				startx = startx - 8 ;
				s = s + 1 ;

		}

		++f ;

	}
	
}

void OMR::omr_processing() {
	
	int startx , starty, i ;

	startx = section_start_x ;
	starty = section_start_y ;

	for( i = 1; i <= number_of_sections ; ++i ) { 
		
		section_processing( startx, starty ) ;
		
		startx += section_diff_x ; 
		starty += section_diff_y ;

	}

}

void OMR::omr_output_path_init() {

	string temp = path.substr( 0, path.find( "Good" ) ) ;

	_mkdir( ( temp + "Output" ).c_str() ) ;

	input_path = "Input/" ;

	output_path = temp + "Output/" ;

}

void OMR::id_evaluation( FILE *input ) {

	int i , counter = 1, position = 0, value ;
	int black_pixel_count , total_pixel_count , ID ;

	black_pixel_count = total_pixel_count = ID =  0 ;

	while( 1 ) {
	
		if( counter == 17 )
			break ;

		character = fgetc( input ) ;
		
		if( character == 'x' ) {

			pixel[ position ] = '\0' ;

			value = atoi( pixel ) ;

			if( value <= 200 ) 
				black_pixel_count++ ;

			position = 0 ;

			total_pixel_count++ ; 

		}
		else
		if( character == 'n' ) {
		
			density =  (double) black_pixel_count / (double) total_pixel_count ;

			if( density >= 0.5 ) 
				ID += (int) pow( 2, 16 - counter ) ;

			black_pixel_count = total_pixel_count = 0 ;

			++counter ; 

		}
		else
			pixel[ position++ ] = character ;

	} 
	
	printf( "%d,", ID );

}

void OMR::answer_evaluation() { 
	
	int i, j ;

	int position, black_pixel_count, total_pixel_count, flag, value, cell, display ;

	position = black_pixel_count = total_pixel_count = cell = 0 ;
	
	flag  = 1 ;

	DIR *directory ;
	struct dirent *entry ; 
	
	freopen( ( output_path + "Output.csv" ).c_str() , "w", stdout ) ;

	printf( "Serial No., File Name" ) ;

	for( i = 1 ; i <= number_of_sections ; ++i ) { 
	
		for( j = 1 ; j <= number_of_questions ; ++j )

			printf( ",%c%d", 'A' + i - 1, j ) ;

	}

	putchar('\n') ;

	if( directory = opendir( input_path.c_str() ) ) {
	
		while( entry = readdir( directory ) ) {
		
			if( strcmp( entry -> d_name , "." ) == 0 )
				continue ;

			if( strcmp( entry -> d_name , ".." ) == 0 )
				continue ;

			string filename ;
			filename = "Input/" ;

			string temp = entry -> d_name ;

			for( i= 0 ; i < temp.length() ; ++i )
				filename += temp[i] ;

			FILE *input = fopen( filename.c_str(), "r" ) ;

			if( input == NULL )
				throw "Error" ;

			id_evaluation( input ) ;
			
			printf( "%s", entry -> d_name ) ;

			while(1) {
			
				character = fgetc( input ) ;
				
				if( character == '\n'  || character == NULL )
					break ;

				if( character == 'x' ) {
					
					pixel[position] = '\0' ;

					value = atoi( pixel ) ;

					if( value <= 200 )
						++black_pixel_count ;

					position = 0 ;

					++total_pixel_count ;

				}
				else
				if( character == 'n' ) {
					
					if( flag <= number_of_cells ) {
						
						flag++;

						pixel_density[ cell ].first = black_pixel_count ;
						pixel_density[ cell ].second = total_pixel_count ;

						black_pixel_count = 0;
						total_pixel_count = 0;

						++cell ;

					}
					if( flag == number_of_cells + 1 ) {
						
						flag = 1 ;
						density = 0.0 ;
						display = -1 ;
						cell = 0 ;

						double temp ;

						for( i = 0 ; i < number_of_cells ; ++i ){

							temp = (double) pixel_density[ i ].first / (double) pixel_density[ i ].second ;
							if( temp > density && temp >= 0.3 ) {
								
								density = temp ;
								display = i + 1 ;
							
							}
						
						}

						if( display == -1 )
							printf( ",None" ) ;
						else
							printf( ",%c", (char) ( display + 64 ) ) ;

					}

				}
				else
					pixel[ position++ ] = character ;

			}

			printf( "\n" ) ;

			fclose( input ) ;

		}
	
	
	}

}

void OMR::omr_evaluate() {
	
	try{
		parameters_read() ;
		omr_input_path_init() ;
		omr_read() ;

		omr_output_path_init() ;
		answer_evaluation() ;

	}catch(...) {

		printf( "There is error in the Input file\n" ) ;
		return ;

	}
}

void OMR::compute_skew() {
	
	//Mat src = imread( "C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR_RECOGNITION/ChallengeImages/_scan0005.jpg", 0 ) ;
	Mat src = omr ;

	Size size = src.size() ; 


	bitwise_not( src, src ) ;

	//imwrite( "C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/4.jpg", src ) ;
	vector<Vec4i> lines ;

	HoughLinesP( src, lines, 1, CV_PI / 180, 100, 0.75 * size.width , 20 ) ;
	Mat disp_lines( size, CV_8UC1, Scalar( 0, 0, 0 ) ) ;

	double angle = 0.0 ;
	unsigned nb_lines = lines.size() ;
	for ( unsigned i = 0 ; i < nb_lines ; ++i ) {

		line( disp_lines, Point( lines[i][0], lines[i][1] ),
                 Point( lines[i][2], lines[i][3] ), Scalar( 255, 0 ,0 ) ) ;

		angle += atan2( ( double ) lines[i][3] - lines[i][1],
                       ( double ) lines[i][2] - lines[i][0]) ;

	}

	angle /= nb_lines ;

	//cout << angle << endl ; 

	//cout << "File angle = " << angle * 180 / CV_PI << endl ;

	//imwrite("C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/3.jpg", disp_lines) ;
	
	Mat dst;

	angle = ( angle * 180 ) / CV_PI  ;
	
	if( angle >= 0 ) 
		angle = -90 + angle ;
	else
		angle = 90 + angle ;

	rotate(src, angle , dst) ;

	//imwrite("C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/2.jpg", dst ) ;

	bitwise_not( dst, omr ) ;

	//imwrite("C:/Users/manu/Desktop/my stuff/study/all sem/sem 7/mini project/OMR/Good/1.jpg", omr ) ;

	omr_x = omr.rows ;
	omr_y = omr.cols ;

	dx = dy = 0 ;

}

void OMR::rotate( Mat &src, double angle, Mat &dst ) {

	int len  = max( src.cols, src.rows ) ;

	Point2f pt( len / 2.0 , len / 2.0 ) ;

	Mat r = getRotationMatrix2D( pt, angle, 1.0 ) ;

	warpAffine( src, dst, r, Size( len, len ) ) ;

}



