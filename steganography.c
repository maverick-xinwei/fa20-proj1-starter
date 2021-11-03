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
  Color* pixel;
  pixel = (Color*)malloc(sizeof(Color));
  if ((image->image)[row*image->cols+col]->B%2)
  {
    pixel->R = 255;
    pixel->G = 255;
    pixel->B = 255;
  }
  else
  {
    pixel->R = 0;
    pixel->G = 0;
    pixel->B = 0;
  }

  return pixel;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
  Image* decoded_image;
  decoded_image = (Image*)malloc(sizeof(Image));
  if (decoded_image == NULL)
  {
    return NULL;
  }
  decoded_image->cols = image->cols;
  decoded_image->rows = image->rows;
  decoded_image->image = (Color**)malloc(sizeof(Color*)*image->cols*image->rows);
  if (decoded_image->image == NULL)
  {
    return NULL;
  }
  for(int h=0; h<image->rows;h++)
  {
    for(int w=0; w<image->cols;w++)
    {
      (decoded_image->image)[h*image->cols+w] = evaluateOnePixel(image, h, w);
    }
  }

  return decoded_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
  Image* raw_image;
  Image* decoded_image;

  raw_image = readData(argv[1]);
  if (raw_image == NULL)
  {
    printf("raw image is NULL\n");
    return -1;
  }

  decoded_image = steganography(raw_image);
  if (!decoded_image)
  {
    printf("decoded_image is NULL\n");
    return -1;
  }

  writeData(decoded_image);
  freeImage(raw_image);
  freeImage(decoded_image);
  return 0;
}
