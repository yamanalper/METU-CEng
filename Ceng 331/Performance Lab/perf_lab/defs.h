/*
 * driver.h - Various definitions for the Performance Lab.
 * 
 * DO NOT MODIFY ANYTHING IN THIS FILE
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define RIDX(i,j,n) ((i)*(n)+(j))

typedef struct {
  //char *team_name;
    char *ID1, *name1;
    char *ID2, *name2;
} team_t;


typedef void (*lab_test_norm) (int, float*, float*);

typedef void (*lab_test_krpro) (int, int, float*, float*, float*);


void normalize(int, float *, float *);

void kronecker_product(int, int, float *, float *, float*);


void register_normalize_functions(void);

void register_kronecker_product_functions(void);


void add_normalize_function(lab_test_norm, char*);

void add_kronecker_product_function(lab_test_krpro, char*);

#endif /* _DEFS_H_ */

