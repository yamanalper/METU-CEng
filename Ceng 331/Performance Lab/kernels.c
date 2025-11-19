/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following team_t struct
 */
team_t team = {

        "e258115",      /* First student ID */
        "Musa Alper Yaman",       /* First student name */

};


/********************
 * NORMALIZATION KERNEL
 ********************/

/****************************************************************
 * Your different versions of the normalization functions go here
 ***************************************************************/

/*
 * naive_normalize - The naive baseline version of convolution
 */
char naive_normalize_descr[] = "naive_normalize: Naive baseline implementation";
 void naive_normalize(int dim, float *src, float *dst) {
     float min, max;
     min = src[0];
     max = src[0];

     for (int i = 0; i < dim; i++) {
         for (int j = 0; j < dim; j++) {
     
             if (src[RIDX(i, j, dim)] < min) {
                 min = src[RIDX(i, j, dim)];
             }
             if (src[RIDX(i, j, dim)] > max) {
                 max = src[RIDX(i, j, dim)];
             }
         }
     }

     for (int i = 0; i < dim; i++) {
         for (int j = 0; j < dim; j++) {
             dst[RIDX(i, j, dim)] = (src[RIDX(i, j, dim)] - min) / (max - min);
         }
     }
 }

char opt1_normalize_descr[] = "opt1_normalize: opt1 normalize implementation";
void opt1_normalize(int dim, float *src, float *dst) {
    float min = src[0];
    float temp;
    int i;
    int resDim = dim * dim;
    float max = min;

    for (i = 0; i < resDim; i++) {
        temp = src[i];
        if (temp < min) min = temp;
        if (temp > max) max = temp;
    }

    float diff = 1.0 / (max - min);

    for (i = 0; i < resDim; i+= 32) {
        dst[i] = (src[i] - min) * diff;
        dst[i + 1] = (src[i + 1] - min) * diff;
        dst[i + 2] = (src[i + 2] - min) * diff;
        dst[i + 3] = (src[i + 3] - min) * diff;
        dst[i + 4] = (src[i + 4] - min) * diff;
        dst[i + 5] = (src[i + 5] - min) * diff;
        dst[i + 6] = (src[i + 6] - min) * diff;
        dst[i + 7] = (src[i + 7] - min) * diff;
        dst[i + 8] = (src[i + 8] - min) * diff;
        dst[i + 9] = (src[i + 9] - min) * diff;
        dst[i + 10] = (src[i + 10] - min) * diff;
        dst[i + 11] = (src[i + 11] - min) * diff;
        dst[i + 12] = (src[i + 12] - min) * diff;
        dst[i + 13] = (src[i + 13] - min) * diff;
        dst[i + 14] = (src[i + 14] - min) * diff;
        dst[i + 15] = (src[i + 15] - min) * diff;
        dst[i + 16] = (src[i + 16] - min) * diff;
        dst[i + 17] = (src[i + 17] - min) * diff;
        dst[i + 18] = (src[i + 18] - min) * diff;
        dst[i + 19] = (src[i + 19] - min) * diff;
        dst[i + 20] = (src[i + 20] - min) * diff;
        dst[i + 21] = (src[i + 21] - min) * diff;
        dst[i + 22] = (src[i + 22] - min) * diff;
        dst[i + 23] = (src[i + 23] - min) * diff;
        dst[i + 24] = (src[i + 24] - min) * diff;
        dst[i + 25] = (src[i + 25] - min) * diff;
        dst[i + 26] = (src[i + 26] - min) * diff;
        dst[i + 27] = (src[i + 27] - min) * diff;
        dst[i + 28] = (src[i + 28] - min) * diff;
        dst[i + 29] = (src[i + 29] - min) * diff;
        dst[i + 30] = (src[i + 30] - min) * diff;
        dst[i + 31] = (src[i + 31] - min) * diff;
    }
}




 /*
 * normalize - Your current working version of normalization
 * IMPORTANT: This is the version you will be graded on
 */
 char normalize_descr[] = "Normalize: Current working version";
 void normalize(int dim, float *src, float *dst)
 {
     opt1_normalize(dim,src,dst);
 }

 /*********************************************************************
 * register_normalize_functions - Register all of your different versions
 *     of the normalization functions  with the driver by calling the
 *     add_normalize_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

 void register_normalize_functions() {
     add_normalize_function(&naive_normalize, naive_normalize_descr);
     add_normalize_function(&opt1_normalize, opt1_normalize_descr);
     add_normalize_function(&normalize, normalize_descr);

     /* ... Register additional test functions here */
 }


/************************
 * KRONECKER PRODUCT KERNEL
 ************************/

/********************************************************************
 * Your different versions of the kronecker product functions go here
 *******************************************************************/

/*
 * naive_kronecker_product - The naive baseline version of k-hop neighbours
 */
char naive_kronecker_product_descr[] = "Naive Kronecker Product: Naive baseline implementation";
void naive_kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod) {
    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim1; j++) {
            for (int k = 0; k < dim2; k++) {
                for (int l = 0; l < dim2; l++) {
                    prod[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)] = mat1[RIDX(i, j, dim1)] * mat2[RIDX(k, l, dim2)];
                }
            }
        }
    }
}



//prod[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)] = mat1[RIDX(i, j, dim1)] * mat2[RIDX(k, l, dim2)];
char opt3_kronecker_product_descr[] = "opt3 Kronecker Product: Naive baseline implementation";
void opt3_kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod) {
	int res1 = dim1*dim2;
    int mat1indx = 0;
    int resRow = 0;
    int dim2_4 = 4 * dim2;
    int b,temp,mat2indx,temp5,i,j,k,l;
    float mat1el;
    int res1_4 = 4 * res1; 
    
    if(dim2 == 4){
    	for(i = 0; i < dim1; i++){
            b = 0;
      		for(j = 0; j < dim1; j++) {
    			mat1el = mat1[mat1indx];
                temp = resRow + b;
                prod[temp     ] = mat1el * mat2[0];
                prod[temp + 1 ] = mat1el * mat2[1];
                prod[temp + 2 ] = mat1el * mat2[2];
                prod[temp + 3 ] = mat1el * mat2[3];
                temp += res1;
                prod[temp    ] = mat1el * mat2[4];
                prod[temp + 1] = mat1el * mat2[5];
                prod[temp + 2] = mat1el * mat2[6];
                prod[temp + 3] = mat1el * mat2[7];
                temp += res1;
                b+=4;
                prod[temp    ] = mat1el * mat2[8];
                prod[temp + 1] = mat1el * mat2[9];
                prod[temp + 2] = mat1el * mat2[10];
                prod[temp + 3] = mat1el * mat2[11];
                temp += res1;
                prod[temp    ] = mat1el * mat2[12];
                prod[temp + 1] = mat1el * mat2[13];
                prod[temp + 2] = mat1el * mat2[14];
                prod[temp + 3] = mat1el * mat2[15];
    			mat1indx++;
    		
    		}
            resRow += res1_4;
    	}
    }
    else if (dim2 == 8){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 8];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 9];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 10];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 11];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 16];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 17];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 18];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 19];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 24];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 25];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 26];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 27];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
    else if (dim2 == 16){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 16];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 17];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 18];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 19];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 32];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 33];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 34];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 35];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 48];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 49];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 50];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 51];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
    else if (dim2 == 32){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 32];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 33];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 34];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 35];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 64];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 65];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 66];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 67];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 96];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 97];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 98];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 99];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
    else if (dim2 == 64){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 64];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 65];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 66];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 67];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 128];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 129];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 130];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 131];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 192];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 193];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 194];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 195];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
    else if (dim2 == 128){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 128];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 129];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 130];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 131];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 256];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 257];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 258];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 259];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 384];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 385];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 386];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 387];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
    else if (dim2 == 256){
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 256];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 257];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 258];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 259];
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[mat2indx + 512];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 513];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 514];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 515];
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[mat2indx + 768];
                        prod[temp + 1] = mat1el * mat2[mat2indx + 769];
                        prod[temp + 2] = mat1el * mat2[mat2indx + 770];
                        prod[temp + 3] = mat1el * mat2[mat2indx + 771];
                        mat2indx+=4;
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }


    else{
        for (i = 0; i < dim1; i++) {
            mat2indx = 0;
            for (k = 0; k < dim2; k+=4) {
                b = 0;
                for (j = 0; j < dim1; j++) {
                    mat1el = mat1[mat1indx];
                    for (l = 0; l < dim2; l+=4) {
                        temp = resRow + b;
                        temp5 = mat2indx + dim2;
                        prod[temp     ] = mat1el * mat2[mat2indx  ];
                        prod[temp + 1 ] = mat1el * mat2[mat2indx+1];
                        prod[temp + 2 ] = mat1el * mat2[mat2indx+2];
                        prod[temp + 3 ] = mat1el * mat2[mat2indx+3];
                        temp += res1;
                        mat2indx+=4;
                        prod[temp    ] = mat1el * mat2[temp5   ];
                        prod[temp + 1] = mat1el * mat2[temp5 +1];
                        prod[temp + 2] = mat1el * mat2[temp5 +2];
                        prod[temp + 3] = mat1el * mat2[temp5 +3];
                        temp5 += dim2;
                        temp += res1;
                        b+=4;
                        prod[temp    ] = mat1el * mat2[temp5   ];
                        prod[temp + 1] = mat1el * mat2[temp5 +1];
                        prod[temp + 2] = mat1el * mat2[temp5 +2];
                        prod[temp + 3] = mat1el * mat2[temp5 +3];
                        temp5 += dim2;
                        temp += res1;
                        prod[temp    ] = mat1el * mat2[temp5   ];
                        prod[temp + 1] = mat1el * mat2[temp5 +1];
                        prod[temp + 2] = mat1el * mat2[temp5 +2];
                        prod[temp + 3] = mat1el * mat2[temp5 +3];
                    }
                    mat1indx++;
                    mat2indx -= dim2;
                }
                resRow += res1_4;
                mat1indx -=dim1;
                mat2indx += dim2_4;
            }
            mat1indx += dim1;
        }
    }
}


/*
* kronecker_product - Your current working version of kronecker_product
* IMPORTANT: This is the version you will be graded on
*/
char kronecker_product_descr[] = "Kronecker Product: Current working version";
void kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod)
{
    opt3_kronecker_product(dim1,dim2,mat1,mat2,prod);
}

/******************************************************************************
* register_kronecker_product_functions - Register all of your different versions
*     of the kronecker_product with the driver by calling the
*     add_kronecker_product_function() for each test function. When you run the
*     driver program, it will test and report the performance of each
*     registered test function.
******************************************************************************/

void register_kronecker_product_functions() {
    add_kronecker_product_function(&naive_kronecker_product, naive_kronecker_product_descr);
    add_kronecker_product_function(&opt3_kronecker_product, opt3_kronecker_product_descr);
    add_kronecker_product_function(&kronecker_product, kronecker_product_descr);
    /* ... Register additional test functions here */
}

