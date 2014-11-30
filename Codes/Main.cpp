#include "OMR_Recognition.h"

/*
	"OMR_Recognition.h" header file includes the OMR 
	class which contains all the functions used for 
	scanning and identifying the marked regions in 
	the given omr sheet. 
*/

/*
	To evaluate all the omr sheets, just set the path
	to the sheets folder in the "input.txt" file and 
	call the omr_evaluate function which in turn will
	call the necessary functions required to evaluate 
	the sheets.
*/

int main() {

	clock_t st = clock(); 

	OMR Object;

	Object.omr_evaluate() ;
	
	clock_t en = clock();

	//fprintf(stdout, "%.3lf\n", (double) ( en - st ) / CLOCKS_PER_SEC );

	getchar();

	return 0;
}