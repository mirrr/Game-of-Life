/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Miranda Cheung
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
    
    //compile:gcc -g -o imageloadertester imageloadertester.c imageloader.c
    //YOUR CODE HERE
    
    FILE *fp = fopen(filename, "r");
    if (NULL == fp) {
        return NULL;
    }
    
    uint32_t width, len;
    fscanf(fp,"%*s %u %u %*u", &width, &len);
    
    Image *result = (Image *) calloc(1, sizeof(Image));
    if (result == NULL){
        fclose(fp);
        return NULL;
    }
    result->rows = len;
    result->cols = width;
    
    //make a matrix of color pixels
    Color **matrix = (Color **) calloc(width*len, sizeof(Color*));
    if (matrix == NULL) {
        free(result);
        fclose(fp);
        return NULL;
    }
    result->image = matrix;
    
    uint8_t red, green, blue;
    int i = 0;
    while (fscanf(fp, "%" SCNu8 "%" SCNu8 "%" SCNu8, &red, &green, &blue) != EOF){
        matrix[i] = (Color*) calloc(1, sizeof(Color));
        if (matrix[i]==NULL){
            freeImage(result);
            return NULL;
        }
        matrix[i]->R = red;
        matrix[i]->G = green;
        matrix[i]->B = blue;
        i++;
    }
    
    fclose(fp);
    return result;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    //YOUR CODE HERE
    printf("P3\n");
    printf("%d %d\n", (image->cols), (image->rows));
    printf("%d\n",255);
    
    Color **matrix = (image->image);
    int x, y;
    //switched if statements
    for(x = 0; x< (image->rows); x++){
        for(y = 0; y <(image->cols); y++){
            //if x!=0 then add 3 spaces in front
            if(0 != y){
                printf("   ");
            }
            //print rbg values of one pixel
            //Color *pixel = matrix[y*(image->rows) + x];
            Color *pixel = matrix[x*(image->cols) + y];
            printf("%3d %3d %3d", pixel->R, pixel->G, pixel->B);
        }
        printf("\n");
    }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
    Color **matrix = (image->image);
    if (matrix){
        int i;
        for (i = 0; i<(image->cols)*(image->rows); i++){
            if(matrix[i]){
                free(matrix[i]);
            }
        }
    }
    free(matrix);
    free(image);
}

