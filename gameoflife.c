/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
    Color *pixel = (Color *) calloc(1, sizeof(Color));
    if (pixel==NULL){
        return NULL;
    }
    uint8_t pixelR = 0;
    uint8_t pixelG = 0;
    uint8_t pixelB = 0;
    /*
     ?? figure out rule for alive & dead
     9 Color pointers for cell and 8 neighbors
     for each bit b(){
        for colorBit = r,g,b () {
            count = 0
            count number of live bits
            if count matches any of the possible hex values
            count = 1<<count
            ?? if ((dead&&(count&matchDead)!=0) || (alive&&c(count&matchAlive)!=0)){
                make colorBit | (1 << b);
            }
        }
     }
     */
    
    //setting up row&col of neighbors
    int rowUP = row - 1;
    int rowDown = row + 1;
    int colLeft = col - 1;
    int colRight = col + 1;
    if (row == 0){
        rowUP = image->rows - 1;
    } else if(row == image->rows - 1) {
        rowDown = 0;
    }

    if (col == 0){
        colLeft = image->cols - 1;
    } else if(col == image->cols - 1) {
        colRight = 0;
    } 
    
    //8 neighbors
    Color** neighbors = (Color**) calloc(8, sizeof(Color*));
    if (neighbors==NULL){
        free(pixel);
        return NULL;
    }
    //top row
    neighbors[0] = image->image[rowUP*(image->cols)+colLeft];
    neighbors[1] = image->image[rowUP*(image->cols)+col];
    neighbors[2] = image->image[rowUP*(image->cols)+colRight];
    //middle row
    neighbors[3] = image->image[row*(image->cols)+colLeft];
    neighbors[4] = image->image[row*(image->cols)+colRight];
    //bottom row
    neighbors[5] = image->image[rowDown*(image->cols)+colLeft];
    neighbors[6] = image->image[rowDown*(image->cols)+col];
    neighbors[7] = image->image[rowDown*(image->cols)+colRight];
    
    
    int bit;
    for(bit=0; bit<8; bit++){
        //number of live neighbors
        int Rcount = 0;
        int Gcount = 0;
        int Bcount = 0;
        
        int i;
        for(i = 0; i<8; i++){
            Rcount += (neighbors[i]->R >>i) & 1;
            Gcount += (neighbors[i]->G >>i) & 1;
            Bcount += (neighbors[i]->B >>i) & 1;
        }
        
        //check R bit alive/dead, compare to live neighbor count, turn on bit if true
        Rcount = 1 << Rcount;
        if (((image->image[row*(image->cols) + col]->R >> bit)&1) == 1){
            if((Rcount&(rule>>9))!=0){
                pixelR = pixelR | (1 << bit);
            }
        } else {
            if((Rcount&rule)!=0){
                pixelR = pixelR | (1 << bit);
            }
        }
        //check G bit alive/dead, compare to live neighbor count, turn on bit if true
        Gcount = 1 << Gcount;
        if (((image->image[row*(image->cols) + col]->G >> bit)&1) == 1){
            if((Gcount&(rule>>9))!=0){
                pixelG = pixelG | (1 << bit);
            }
        } else {
            if((Gcount&rule)!=0){
                pixelG = pixelG | (1 << bit);
            }
        }
        //check B bit alive/dead, compare to live neighbor count, turn on bit if true
        Bcount = 1 << Bcount;
        if (((image->image[row*(image->cols) + col]->B >> bit)&1) == 1){
            if((Bcount&(rule>>9))!=0){
                pixelB = pixelB | (1 << bit);
            }
        } else {
            if((Bcount&rule)!=0){
                pixelB = pixelB | (1 << bit);
            }
        }
        
    }
    pixel->R = pixelR;
    pixel->G = pixelG;
    pixel->B = pixelB;
    
    free(neighbors);
    return pixel;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
    
    //do we create new Image obj or modify old one?
    
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
            matrix[x*(image->cols) + y] = evaluateOneCell(image, x, y, rule);
            if(matrix[x*(image->cols) + y] == NULL){
                freeImage(result);
                return NULL;
            }
        }
    }
    
    return result;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    //if input wrong
    if (argc!=3){
        printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
        exit(-1);
    }
    
    //OG image
    Image* image = readData(argv[1]);
    if (image == NULL) {
        exit(-1);
    }
    
    //new image
    char* ptr;
    uint32_t num = (uint32_t) strtol(argv[2], &ptr, 16);
    Image* secret = life(image, num);
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
