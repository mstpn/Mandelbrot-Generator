/******************************************************************************
 Name: Matthew Stepan
 File name: mandel.cpp
 Purpose:
        Create a computer image of the Mandelbrot set.
 Details:
	This program computes the image of the Mandelbrot set using 
	recursive functions in lieu of any form of looping structure (for,
	while, etc.).

	The Mandelbrot set (https://en.wikipedia.org/wiki/Mandelbrot_set)
	can be mapped as an image in the 2d plane by using the x plane
	to represent the real portion of a complex number c, and the 
	y plane to represent the imaginary portion of the complex number
	c. More details about the the mathematics can be found in the 
	inMandel function details.

	Increasing the amount of repetitions that the program checks
	whether a number is in the Mandelbrot set will increase the accuracy
	of the image. This will result in a more intricate graph as long
	as the image size is sufficient.

	In order to preserve the resolution of the image with a high number
	of repetitions, the graph is colored based on the number of
	repetitions performed to determine whether a number was
	found in the set.

 Colouring method: See details section of the colorImage function.

 Input recommendations:
	Quality	     Size        Iterations   File Size   Est. Time
	Quick	     500px       100	      ~125 KiB	  Instantaneous
	Standard     1000px	 250	      ~500 KiB    5 s
	Better-      5000px      100	      ~6 Mib      1 min
	Better+      5000px      500          ~7 Mib      2.5 min
	Mandelbrot   20000px     250          ~20 Mib	  30+ min
	
	Quick: Instantaneous, small file size. Still viewable. Looks best
	zoomed out.

	Standard: Recommended unless you need the small file size of quick.
	Gives you some detail when zoomed in. Better expression of coloring,
	clearer lines.

	Better-: Better then standard due to resolution. Much sharper image,
	detail of set "better".

	Better+: More detail and better coloring when compared with Better-.
	Similar file size. Cost is that it takes 2.5x longer to calculate.

	Mandelbrot: The way it is meant to be seen. Long calculation. Reducing
	iterations can help. You are are waiting 15+ minutes regardless,
	so take the time to get the full experience of a better set depth.
	This may cause you to hit the caps of system (INS) storage/resource 
	allocation. Do so at your own risk.

 Assumptions and limitations:
        Excessively large input values (i.e. 100k size with 10k iterations)
	can cause a crash due to system resource limitaions (at least on INS,
	YMMV on different systems).
	Large enough values may also lead to a stack overflow, as there will
	be enough recurive calls to functions that the stack will be filled.
 Known bugs:
         N/A
******************************************************************************/
// save diagnostic state
#pragma GCC diagnostic push

// turn off specific warnings.
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wconversion"
#include <Magick++.h>
using namespace Magick;

// turn the warnings back 
#pragma GCC diagnostic pop

using namespace std;
#include <iostream>
#include <cmath>
#include "ioutil.h"

const double MIN_REAL = -1.5;	// Minimum real value
const double MAX_REAL = 0.5;	// Maximum real value
const double MIN_IM = -1;	// Minimum imaginary value
const double MAX_IM = 1;	// Maximim imaginary value
const int BOUND = 2;		// Sequence approaches infinity if greater than

void createImage(Image &mandel);
void getInput(int &size, int &maxIterations);
void yLoop(Image &mandel, int maxXY, int x, int y, double scaleReal,
	   double scaleIm, int maxIterations);
void xLoop(Image &mandel, int maxXY, int x, int y, double scaleReal,
	   double scaleIm, int maxIterations);
void colorImage(Image &mandel, int x, int y, int iterations, int maxIterations);
void inMandel(int maxIterations, double cReal, double cIm, double zReal,
	      double zIm, bool &inSet, int &iterations);

int main()
{
    Image mandel("1x1", "black");   // Mandelbrot set image

    createImage(mandel);

    mandel.write("mstep997_a3.png");
    
    return 0;
}


/******************************************************************************
 Name:
        createImage
 Purpose:
	Handle creation of a Mandelbrot image.
 Details:
	Retrieve user inputs:
		Users must enter a valid width (size, as image is square), and
		number of iterations as integers.
	Generate Mandelbrot image:
		Call recursive functions to calculate whether the complex number
		that represents a pixel of the image is in the Mandelbrot set.
	Print image:
		Colour the pixel according to the number of iterations for it
	       	took to determine whether the complex number was in the set.
 Input:
	Integer for the size of the image in pixels. Can be garbage on input.
		Pass by reference.
	Integer for the maximum number of iterations to check whether a number
	is in the Mandelbrot set. Garbage on input.
		Pass by reference.
 Output:
	Updated integer for the image size.
	Updated integer for the maximum number of iterations.
******************************************************************************/
void createImage(Image &mandel)
{
    int size,	 	// 'n' in the 'n x n' size of the image, pixels
	maxIterations,	// how many times to check if number in mandel. set
	x = 0,		// x coordinate of mandelbrot image
	y = 0,		// y coordinate of mandelbrot image
	maxXY;		// maximum dimensions for Magick++ images, size - 1
    double scaleReal, 	// scale real numbers to pixel coordinates
	scaleIm;	// scale imaginary numbers to pixel coordinates
    
    getInput(size, maxIterations);
    
    mandel.extent(Geometry(size, size), Color("black"));
    maxXY = size - 1;
    scaleReal = (MAX_REAL - MIN_REAL) / (size - 1);
    scaleIm = (MAX_IM - MIN_IM) / (size - 1);
    
    yLoop(mandel, maxXY, x, y, scaleReal, scaleIm, maxIterations);
    
}


/******************************************************************************
 Name:
        getInput
 Purpose:
        Get user input for dimensions and max iterations of the Mandelbrot 
    	image.
 Details:
	The ioutil module was used in order to ensure valid integer inputs.
	EOF is not considered a valid input, and the user will be prompted
	to enter new values until this condition is cleared.
 Input:
	Integer for the size of the image in pixels. Can be garbage on input.
		Pass by reference.
	Integer for the maximum number of iterations to check whether a number
	is in the Mandelbrot set. Garbage on input.
		Pass by reference.
 Output:
	Updated integer for the image size.
	Updated integer for the maximum number of iterations.
******************************************************************************/
void getInput(int &size, int &maxIterations)
{
    bool eof;	// did the user pass eof to as input
    
    size = readInt("Enter the width of the image (pixels, integer): "
		   , eof);
    while(eof)
    {
	cout << "\nEOF is not a valid width... Please try again.\n";
	size = readInt("Enter the width of the image (pixels, integer): ",
		       eof);
    }

    maxIterations = readInt("Enter the maximum number of iterations: ",
			    eof);
    while(eof)
    {
	cout << "\nEOF is not a valid iteration number... Please try again.\n";
	maxIterations = readInt("Enter the maximum number of iterations: ",
				eof);	
    }
    
    return;
}


/******************************************************************************
 Name:
        inMandel
 Purpose:
	Determine if a complex number is in the Mandelbrot set.
 Details:
	A complex number, c, is in the Mandelbrot set if the sequence
		Z_n+1 = Z_n^2 + c
	does not approach infinity. To simplify, if any value of the sequence
	is greater than 2 from the origin, then the sequence will approach
	infinity. This is done via the pythagorean theorem.

	The complex number
		a + bi
	has been represented by two real values, where
		a = the real portion, x component of cartesian coordinate
		b = the imaginary/portion, y component of cartesian coordinate

	Scaling factors are used in order to map these complex numbers to 
	their position on a cartesian coordinate grid, with the MIN_IM, MAX_IM,
	MIN_REAL, MAX_REAL constants representing the bounds of the grid.

	This is a recursive function:
	Base case:
	1)	Iterations greater than max iterations or number is in
		the set.
			Skip processing, end function.
	2)	Z is found to be greater than 2 (BOUND)
			Set inSet bool to false, end function.

	Recursive case:
		Z has not been found to be greater than 2 (BOUND)
			Calculate the next set of Z values, increase 
			iterations by one, call inMandel again with 
			the new values.
 Input:
	An integer representing the maximum number of times to check if the 
	number is in the set.
		Passed by value.
	A double representing the real portion of the complex constant c.
		Passed by value.
	A double representing the imaginary portion of the complex constant c.
		Passed by value,
	A double representing the real portion of the variable z.
		Passed by value.
	A double representing the imaginary portion of the variable z.
		Passed by value.
	A bool indicating whether the number was found to be in the set. The
	first (non-recursive) caller must set this as true.
		Passed by reference.
	An integer for the number of times the function has been called for
	a given number.
		Passed by reference.
 Output:
	An updated bool whether the number was found to be in the set.
	An updated integer for the number of times the function has been 
	called.
******************************************************************************/
void inMandel(int maxIterations, double cReal, double cIm, double zReal,
	      double zIm, bool &inSet, int &iterations)
{
    double zReal2,	// square of real component of Z
	zIm2,		// square of imaginary component of Z
	absZ;		// absolute value of Z

    if(iterations < maxIterations && inSet == true)
    {
    	zReal2 = pow(zReal, 2);
    	zIm2 = pow(zIm, 2);
    	absZ = sqrt(zReal2 + zIm2);
	
    	if(absZ > BOUND)
    	{
    	    inSet = false;
    	}
    	else
    	{
    	    zIm = (2 * zReal * zIm) + cIm;
    	    zReal = zReal2 - zIm2 + cReal;
    	    iterations++;
    	    inMandel(maxIterations, cReal, cIm, zReal, zIm, inSet, iterations);
    	}
    }
    
    return;
}


/******************************************************************************
 Name:
        yLoop
 Purpose:
	Traverse through all rows of pixels in the image of the Mandelbrot set.
 Details:
	This is a recursive function:
	Base case:
		Pixel y is greater than the maximum height of the image
			Don't process, end funciton
	Recursive case:
		Pixel y is within the bounds of the image height
			Call the recursive function to traverse the columns
			of the image.
       			Increase the row number, call yLoop again with this
			new value.
 Input:
	Magick++ image of the Mandelbrot set.
		Passed by reference.
	Integer for the maximum width/height of the image.
		Passed by value.
	Integer for the pixel coordinate in the x plane,
		Passed by value.
	Integer for the pixel coordinate in the y plane.
		Passed by value.
	Double for the scaling factor to convert between complex number
	and cartesian coordinate in the x (real) plane.
		Passed by value.
	Double for the scaling factor to convert between complex number
	and cartesian coordinate in the y (imaginary) plane.
		Passed by value.
	An integer representing the maximum number of times to check if the 
	number is in the set.
		Passed by value.
 Output:
	An updated image of the Mandelbrot set.
******************************************************************************/
void yLoop(Image &mandel, int maxXY, int x, int y, double scaleReal,
	   double scaleIm, int maxIterations)
{
    if(y <= maxXY)
    {
	xLoop(mandel, maxXY, x, y, scaleReal, scaleIm, maxIterations);
	y++;
	yLoop(mandel, maxXY, x, y, scaleReal, scaleIm, maxIterations);
    }
    
    return;
}


/******************************************************************************
 Name:
        xLoop
 Purpose:
	Traverse through all columns in the image of the Mandelbrot set.
 Details:
	This is a recursive function:
	Base case:
		Pixel x is greater than the maximum width of the image.
			Don't process, end funciton
	Recursive case:
		Pixel x is within the bounds of the image width
			Check whether the number at the x,y position is in
			the set.
			Color image appropriately.
			Call xLoop recursively.
 Input:
	Magick++ image of the Mandelbrot set.
		Passed by reference.
	Integer for the maximum width/height of the image.
		Passed by value.
	Integer for the pixel coordinate in the x plane,
		Passed by value.
	Integer for the pixel coordinate in the y plane.
		Passed by value.
	Double for the scaling factor to convert between complex number
	and cartesian coordinate in the x (real) plane.
		Passed by value.
	Double for the scaling factor to convert between complex number
	and cartesian coordinate in the y (imaginary) plane.
		Passed by value.
	An integer representing the maximum number of times to check if the 
	number is in the set.
		Passed by value.
 Output:
	An updated image of the Mandelbrot set.
******************************************************************************/
void xLoop(Image &mandel, int maxXY, int x, int y, double scaleReal,
	   double scaleIm, int maxIterations)
{
    double zReal,	// real portion of the complex number z
	zIm,		// imaginary portion of the complex number z
	cReal,		// real portion of the complex number c
	cIm;		// imaginary portion of the complex number c
    bool inSet;	        // is the current complex number in the mandelbrot set
    int iterations;	// amount that a number was checked to be in the set

    if(x <= maxXY)
    {
	cIm = MAX_IM - (y * scaleIm);
	cReal = MIN_REAL + (x * scaleReal);
	inSet = true;
	zReal = cReal;
	zIm = cIm;
	iterations = 0;

	inMandel(maxIterations, cReal, cIm, zReal,zIm, inSet, iterations);
	
 	colorImage(mandel, x, y, iterations, maxIterations);
    
	x++;
	xLoop(mandel, maxXY, x, y, scaleReal, scaleIm, maxIterations);
    }

    return;
}


/******************************************************************************
 Name:
        colorImage
 Purpose:
	Colour the pixels of the Mandelbrot set image.
 Details:
	The image is colored based off of the number of iterations that it took
	to determine if the complex number (x,y coordinates) was in the 
	Mandelbrot set. 
		If the number of iterations is equal to the maximum
		iterations, then the number was not in the set, and it is not 
		coloured (leave as a background (black) pixel).
	For all other cases, the number of iterations was broken into two sets,
	which will transition between 2 colours each. The value of the first
	section is set to max iterations / 4 rather than the "standard" max
	iterations / 2, as this looks better for lower resolutions and iteration
	values. The second section is the difference between the first and 
	max iterations - 1.

	RGB colors were used for ease of linearly calculating the differnce
	between different colors, and to smoothly transition between one
	another. This also allows for a greater range of colors than relying
	on named X11 colors.

      	For each set, the colors are mapped such that the minimum of the set
	range will correspond to one color, and the maximum iteration value
	in the set will correspond to the other color. All values in between
	are a linear combination of these two colors, such that the midpoint 
	between them will be the RGB color directly between the two colors.

	Color palette from Dracula Theme: https://draculatheme.com/contribute/

 	*American spelling of colour used in the function name to be consistent
	with Magick++ function names.
 Input:
	Magick++ image of the Mandelbrot set.
		Passed by reference.
	Integer for the pixel coordinate in the x plane,
		Passed by value.
	Integer for the pixel coordinate in the y plane.
		Passed by value.
	An integer for the number of times the function has been called for
	a given number.
		Passed by value.
	An integer representing the maximum number of times to check if the 
	number is in the set.
		Passed by value.
 Output:
	An updated image of the Mandelbrot set.
******************************************************************************/
void colorImage(Image &mandel, int x, int y, int iterations, int maxIterations)
{
    double r, g, b, r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;//RGB colors
    Color pColor;		// pixel color based on RGB
    int firstSection,		// size of the first color section
	secondSection;		// size of the second color section

    firstSection = maxIterations / 4;
    secondSection = maxIterations - firstSection -1;

    //colour 1 (dracula bg)
    r1 = 40;
    g1 = 42;
    b1 = 54;

    //colour 2 (dracula purp)
    r2 = 189;
    g2 = 147;
    b2 = 249;

    //colour 3 (dracula green)
    r3 = 80;
    g3 = 250;
    b3 = 123;

    //colour 4 (dracula pink)
    r4 = 255;
    g4 = 121;
    b4 = 198;
    
    if(iterations >= 0 && iterations < firstSection)
    {
	//colour 1 to colour 2
	r = (r1 + ((r2 - r1) *1.0 * iterations / firstSection)) / 255.0; 
	g = (g1 + ((g2 - g1) *1.0 * iterations / firstSection)) / 255.0;
	b = (b1 + ((b2 - b1) *1.0 * iterations / firstSection)) / 255.0;
	pColor = ColorRGB(r, g, b);
	
    	mandel.pixelColor(x, y, pColor);
    }
    else if(iterations >= firstSection && iterations < maxIterations - 1)
    {
	//colour 3 to colour 4
	r = (r3 + ((r4 - r3) *1.0 * (iterations - firstSection) / secondSection))
	    / 255.0; 
	g = (g3 + ((g4 - g3) *1.0 * (iterations - firstSection) / secondSection))
	    / 255.0; 
	b = (b3 + ((b4 - b3) *1.0 * (iterations - firstSection) / secondSection))
	    / 255.0; 
	pColor = ColorRGB(r, g, b);
	
    	mandel.pixelColor(x, y, pColor);
    }
    
    return;
}
// end
