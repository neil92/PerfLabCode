/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "StarU",              /* Team name */

    "Neil A. Patel",     /* First member full name */
    "neil92@bu.edu",  /* First member email address */

    "Kyle L. Holzinger",                   /* Second member full name (leave blank if none) */
    "kylelh@bu.edu"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, ii, jj, bsize = 32;

	for (ii = 0; ii < dim; ii+= bsize)
	for (jj = 0; jj < dim; jj+= bsize)
	for (i = ii; i < ii + bsize; i+=4)
	for (j = jj; j < jj + bsize; j+=8) {
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
		dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
		dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
		dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
		
		dst[RIDX(dim-1-j-1, i, dim)] = src[RIDX(i, j+1, dim)];
		dst[RIDX(dim-1-j-1, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
		dst[RIDX(dim-1-j-1, i+2, dim)] = src[RIDX(i+2, j+1, dim)];
		dst[RIDX(dim-1-j-1, i+3, dim)] = src[RIDX(i+3, j+1, dim)];
		
		dst[RIDX(dim-1-j-2, i, dim)] = src[RIDX(i, j+2, dim)];
		dst[RIDX(dim-1-j-2, i+1, dim)] = src[RIDX(i+1, j+2, dim)];
		dst[RIDX(dim-1-j-2, i+2, dim)] = src[RIDX(i+2, j+2, dim)];
		dst[RIDX(dim-1-j-2, i+3, dim)] = src[RIDX(i+3, j+2, dim)];
		
		dst[RIDX(dim-1-j-3, i, dim)] = src[RIDX(i, j+3, dim)];
		dst[RIDX(dim-1-j-3, i+1, dim)] = src[RIDX(i+1, j+3, dim)];
		dst[RIDX(dim-1-j-3, i+2, dim)] = src[RIDX(i+2, j+3, dim)];
		dst[RIDX(dim-1-j-3, i+3, dim)] = src[RIDX(i+3, j+3, dim)];
		
	    dst[RIDX(dim-1-j-4, i, dim)] = src[RIDX(i, j+4, dim)];
		dst[RIDX(dim-1-j-4, i+1, dim)] = src[RIDX(i+1, j+4, dim)];
		dst[RIDX(dim-1-j-4, i+2, dim)] = src[RIDX(i+2, j+4, dim)];
		dst[RIDX(dim-1-j-4, i+3, dim)] = src[RIDX(i+3, j+4, dim)];
		
		dst[RIDX(dim-1-j-5, i, dim)] = src[RIDX(i, j+5, dim)];
		dst[RIDX(dim-1-j-5, i+1, dim)] = src[RIDX(i+1, j+5, dim)];
		dst[RIDX(dim-1-j-5, i+2, dim)] = src[RIDX(i+2, j+5, dim)];
		dst[RIDX(dim-1-j-5, i+3, dim)] = src[RIDX(i+3, j+5, dim)];
		
		dst[RIDX(dim-1-j-6, i, dim)] = src[RIDX(i, j+6, dim)];
		dst[RIDX(dim-1-j-6, i+1, dim)] = src[RIDX(i+1, j+6, dim)];
		dst[RIDX(dim-1-j-6, i+2, dim)] = src[RIDX(i+2, j+6, dim)];
		dst[RIDX(dim-1-j-6, i+3, dim)] = src[RIDX(i+3, j+6, dim)];
		
		dst[RIDX(dim-1-j-7, i, dim)] = src[RIDX(i, j+7, dim)];
		dst[RIDX(dim-1-j-7, i+1, dim)] = src[RIDX(i+1, j+7, dim)];
		dst[RIDX(dim-1-j-7, i+2, dim)] = src[RIDX(i+2, j+7, dim)];
		dst[RIDX(dim-1-j-7, i+3, dim)] = src[RIDX(i+3, j+7, dim)];
	}
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    naive_smooth(dim, src, dst);
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

