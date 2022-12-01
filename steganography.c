/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
    Color *pixel = (Color *) calloc(1, sizeof(Color));
    if(pixel != NULL) {
        //Color *original = image->image[col*(image->rows) + row];
        if (((image->image[row*(image->cols) + col]->B) & 1) == 1) {
            pixel->R = 255;
            pixel->G = 255;
            pixel->B = 255;
        } else {
            pixel->R = 0;
            pixel->G = 0;
            pixel->B = 0;
        }
        return pixel;
    }
    return NULL;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
    /*
        1. create new Image obj
        2. go through every Color* in **matrix = image->image
        3. find new val and assign to pixel
                        free(matrix[i]) matrix[i] = evaluateOnePixel()
     */
    
    Image* result = (Image *) calloc(1, sizeof(Image));
    if (result == NULL){
        return NULL;
    }
    result->cols = image->cols;
    result->rows = image->rows;
    Color **matrix = (Color**) calloc(image->cols * image->rows, sizeof(Color*));
    if (matrix == NULL){
        free(result);
        return NULL;
    }
    result->image = matrix;
    
    int x, y;
    for(x = 0; x< (image->rows); x++){
        for(y = 0; y <(image->cols); y++){
            matrix[x*(image->cols) + y] = evaluateOnePixel(image, x, y);
            if(matrix[x*(image->cols) + y] == NULL){
                freeImage(result);
                return NULL;
            }
        }
    }
    
    return result;
}

/*
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    if (argc != 2) {
        exit(-1);
    }
    
    //OG image
    Image* image = readData(argv[1]);
    if (image == NULL) {
        exit(-1);
    }
    //secret message image
    Image* secret = steganography(image);
    if (secret == NULL) {
        freeImage(image);
        exit(-1);
    }
    
    
    writeData(secret);
    if (secret != NULL) {
        freeImage(secret);
    }
    if(image != NULL){
        freeImage(image);
    }
    

    return 0;
}
