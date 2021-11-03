/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
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

static int check_newline(FILE* fh)
{
  char str;
  while(fscanf(fh, "%c", &str) != EOF)
  {
    if (str == '\n')
    {
      return 0;
    }
    else if (str != ' ')
    {
      // Expect no other string than space but got one.
      printf("Expected space but got %s\n", &str);
      return 1;
    }
  }

  return 0;
}

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
  FILE* fh;
  char str[10];
  unsigned int width, height, max_range;
  
  Image* image;

  fh = fopen(filename, "r");
  fscanf(fh, "%s", str);

  if (strcmp(str,"P3"))
  {
    printf("Failed: expected P3 but got %s\n", str);
    return NULL;
  }


  if (check_newline(fh))
  {
    printf("Failed here!\n");
    return NULL;
  }

  fscanf(fh, "%s", str);
  width = atoi(str);
  fscanf(fh, "%s", str);
  height = atoi(str);
  fscanf(fh, "%s", str);
  max_range = atoi(str);

  if (max_range != 255)
  {
    printf("max range is supposed to be 255 but got  %d\n", max_range);
    return NULL;
  }

  //printf("width=%d height=%d max_range=%d\n", width, height, max_range);


  image = (Image*)malloc(sizeof(Image));

  if (image == NULL)
  {
    printf("Failed to allocate mem for image !\n");
    return NULL;
  }

  image -> rows = height;
  image -> cols = width;
  image->image = (Color**)malloc(sizeof(Color*)*width*height);
  if (image->image == NULL)
  {
    printf("Failed to allocate mem\n");
    return NULL;
  }

  for (int h=0; h<height; h++)
  {
    for (int w=0; w<width; w++)
    {
        char str[5];
        (image->image)[image->cols*h+w] = (Color*)malloc(sizeof(Color));

        if ((image->image)[image->cols*h+w] == NULL)
        {
          printf("Failed to alloc mem for a pixel\n");
          return NULL;
        }

        if (fscanf(fh, "%s", str) == 1)
        {
          //printf("R: %s\n", str);
          (image->image)[image->cols*h+w]-> R =atoi(str); 
        }
        else
        {
          printf("Failed to extract pixel string\n");
          return NULL;
        }

        if (fscanf(fh, "%s", str) == 1)
        {
          //printf("G: %s\n", str);
          (image->image)[image->cols*h+w]-> G =atoi(str); 
        }
        else
        {
          printf("Failed to extract pixel string\n");
          return NULL;
        }

        if (fscanf(fh, "%s", str) == 1)
        {
          //printf("B: %s\n", str);
          (image->image)[image->cols*h+w]-> B =atoi(str); 
        }
        else
        {
          printf("Failed to extract pixel string\n");
          return NULL;
        }
    }

    if (check_newline(fh))
    {
      printf("Failed in the end of row!\n");
      return NULL;
    }
  }

  fclose(fh);
  return image;
}


//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
  if (!image)
  {
    printf("Got a NULL image pointer !\n");
    return;
  }
  printf("P3\n");
  printf("%d %d\n", image->cols, image->rows);
  printf("255\n");

  for (int h = 0; h < image->rows; h++)
  {
    for (int w = 0; w < image->cols; w++)
    {
      printf("%3d %3d %3d", (image->image)[h*image->cols + w]->R, (image->image)[h*image->cols + w]->G, (image->image)[h*image->cols + w]->B);
      if (w != image->cols -1)
      {
        printf("   ");
      }
    }

    printf("\n");
  }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
  for (int h = 0; h < image->rows; h++)
  {
    for (int w = 0; w < image->cols; w++)
    {
      free((image->image)[h*image->cols + w]);
    }
  }
  free(image->image);
  free(image);
}
