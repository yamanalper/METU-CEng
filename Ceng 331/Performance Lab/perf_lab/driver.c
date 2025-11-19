/*******************************************************************
 * 
 * driver.c - Driver program for CS:APP Performance Lab
 * 
 * In kernels.c, students generate an arbitrary number of normalize and
 * kronecker_product test functions, which they then register with the driver
 * program using the add_normalize_function() and
 * add_kronecker_product_function() 
 * 
 * The driver program runs and measures the registered test functions
 * and reports their performance.
 * 
 * Copyright (c) 2002, R. Bryant and D. O'Hallaron, All rights
 * reserved.  May not be used, modified, or copied without permission.
 *
 ********************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "fcyc.h"
#include "defs.h"
#include "config.h"

/* Team structure that identifies the students */
extern team_t team;

/* Keep track of a number of different test functions */
#define MAX_BENCHMARKS 100
#define DIM_CNT 6

/* Misc constants */
#define BSIZE 32     /* cache block size in bytes */
#define MAX_DIM 1280 /* 1024 + 256 */
#define ODD_DIM 96   /* not a power of 2 */

/* This struct characterizes the results for one benchmark test */
typedef struct {
    int func_type;
    union func {
        lab_test_norm tfunctnorm;
        lab_test_krpro tfunctkrpro;
    } tfunct; /* The test function */
    double cpes[DIM_CNT]; /* One CPE result for each dimension */
    char *description;    /* ASCII description of the test function */
    unsigned short valid; /* The function is tested if this is non zero */
} bench_t;

/* The range of matrix dimensions that we will be testing */
static int test_dim_krpro[] = {32, 64, 128, 256, 512, 1024};
static int test_dim_norm[] = {32, 64, 128, 256, 512, 1024};
static int test_dim1_krpro[] = {8, 4, 32, 16, 64, 4};
static int test_dim2_krpro[] = {4, 16, 4, 16, 8, 256};

/* Baseline CPEs (see config.h) */
static double krpro_baseline_cpes[] = {K32, K64, K128, K256, K512, K1024};
static double norm_baseline_cpes[] = {N32, N64, N128, N256, N512, N1024};

/* These hold the results for all benchmarks */
static bench_t benchmarks_krpro[MAX_BENCHMARKS];
static bench_t benchmarks_norm[MAX_BENCHMARKS];

/* These give the sizes of the above lists */
static int krpro_benchmark_count = 0;
static int norm_benchmark_count = 0;

/*
 * An matrix is a dimxdim matrix of ints stored in a 1D array.  The
 * data array holds three matrixs (the input original, a copy of the original,
 * and the output result array. There is also an additional BSIZE bytes
 * of padding for alignment to cache block boundaries.
 */
static float data[(3*MAX_DIM * MAX_DIM) + (BSIZE/sizeof(float))];
static float data2[(3*MAX_DIM * MAX_DIM) + (BSIZE/sizeof(float))];
/* Various matrix pointers */
static float *orig = NULL;         /* original matrix */
static float *orig2 = NULL;        /* second original */
static float *copy_of_orig = NULL; /* copy of original for checking result */
static float *copy_of_orig2 = NULL;/* copy of second original for checking result*/
static float *result1 = NULL;       /* result matrix for normalize*/
static float *result2 = NULL;       /* result matrix for kronecker_product*/

/* Keep track of the best transpose and kronecker_product score for grading */
double krpro_maxmean = 0.0;
char *krpro_maxmean_desc = NULL;

double norm_maxmean = 0.0;
char *norm_maxmean_desc = NULL;


/******************** Functions begin *************************/
void add_kronecker_product_function(lab_test_krpro f, char *description) {
    benchmarks_krpro[krpro_benchmark_count].func_type = 5;
    benchmarks_krpro[krpro_benchmark_count].tfunct.tfunctkrpro = f;
    benchmarks_krpro[krpro_benchmark_count].description = description;
    benchmarks_krpro[krpro_benchmark_count].valid = 0;
    krpro_benchmark_count++;
}


void add_normalize_function(lab_test_norm f, char *description) {
    benchmarks_norm[norm_benchmark_count].func_type = 3;
    benchmarks_norm[norm_benchmark_count].tfunct.tfunctnorm = f;
    benchmarks_norm[norm_benchmark_count].description = description;
    benchmarks_norm[norm_benchmark_count].valid = 0;
    norm_benchmark_count++;
}


/*
 * random_in_interval - Returns random integer in interval [low, high)
 */
static float random_in_interval(float low, float high) {
    int size = high - low;
    return (float)(rand() % size) + low;
}

/*
 * create - creates a dimxdim matrix aligned to a BSIZE byte boundary
 */
static void create(int dim) {
    int i, j;

    /* Align the matrixs to BSIZE byte boundaries */
    orig = data;
    orig2 = data2;
    while ((unsigned) orig % BSIZE)
        orig = (float *) (((char *) orig) + 1);
    result1 = orig + dim * dim;
    copy_of_orig = result1 + dim * dim;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            /* Original image initialized to random numbers */
            orig[RIDX(i, j, dim)] = random_in_interval(-25, 50);

            /* Copy of original image for checking result */
            copy_of_orig[RIDX(i, j, dim)] = orig[RIDX(i, j, dim)];

            /* Result image initialized to all black */
            result1[RIDX(i, j, dim)] = 0;
        }
    }

    while ((unsigned) orig2 % BSIZE)
        orig2 = (float *) (((char *) orig2) + 1);
    result2 = (float*) (orig2 + dim * dim);
    copy_of_orig2 = (float *) (result2 + dim * dim);

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            /* Original2 matrix initialized to random numbers */
            orig2[RIDX(i, j, dim)] = random_in_interval(0, 15);

            /* Copy of original2 matrix for checking result */
            copy_of_orig2[RIDX(i, j, dim)] = orig2[RIDX(i, j, dim)];

            /* Result matrix initialized to 0 */
            result2[RIDX(i, j, dim)] = 0;
        }
    }
}




static int compare_ints(unsigned p1, unsigned p2) {
    return (p1 != p2);
}

static int compare_floats(float f1, float f2){
	float eps = 0.001;
	return ((f1 < (f2-eps)) || (f1 > (f2+eps)));
}

/* Make sure the orig array is unchanged */
static int check_orig(int dim) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            if (compare_floats(orig[RIDX(i, j, dim)], copy_of_orig[RIDX(i, j, dim)])) {
                printf("\n");
                printf("Error: Original matrix has been changed!\n");
                return 1;
            }

    return 0;
}

/* Make sure the orig2 array is unchanged */
static int check_orig2(int dim) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            if (compare_floats(orig2[RIDX(i, j, dim)], copy_of_orig2[RIDX(i, j, dim)])) {
                printf("\n");
                printf("Error: Second Original matrix has been changed!\n");
                return 1;
            }

    return 0;
}


/*
 * check_krpro - Make sure the kronecker product actually works.
 * The orig array and orig2 arrays should not  have been tampered with!
 */
static int check_krpro(int dim1, int dim2) {
    int err = 0;
    int i, j, k, l;
    int right, wrong, mult;
    int badi, badj;
    int dim = dim1*dim2;

    /* return 1 if the original matrix has been  changed */
    if (check_orig(dim))
        return 1;
    /* return 1 if the second original matrix has been  changed */
    if (check_orig2(dim))
        return 1;


    for ( i = 0; i < dim1; i++) {
        for ( j = 0; j < dim1; j++) {
            for ( k = 0; k < dim2; k++) {
                for ( l = 0; l < dim2; l++) {
                    mult = orig[RIDX(i, j, dim1)] * orig2[RIDX(k, l, dim2)];
                    
                    if (compare_floats(mult, result2[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)])) {
			badi = RIDX(i, k, dim2);
			badj = RIDX(j, l, dim2);
			wrong = result2[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)];
			right = mult;
			err++;
		    }
                }
            }
        }
    }


    if (err) {
        printf("\n");
        printf("ERROR: Dimension=%d, %d errors\n", dim, err);
        printf("You have dst[%d][%d] = %d\n",
               badi, badj, wrong);
        printf("It should be dst[%d][%d] = %d\n",
               badi, badj, right);


    }
    return err;
}


/*
 * check_norm - Make sure the normalization actually works.
 * The orig array should not have been tampered with!
 */
static int check_norm(int dim) {
    int err = 0;
    float min, max;
    int i, j;
    float right, wrong, norm;
    int badi, badj;

    /* return 1 if the original matrix has been  changed */
    if (check_orig(dim))
        return 1;
 
    min = orig[0];
    max = orig[0];

    for ( i = 0; i < dim; i++) {
        for ( j = 0; j < dim; j++) {
            float value = orig[RIDX(i, j, dim)];
            if (value < min) {
                min = value;
            }
            if (value > max) {
                max = value;
            }
        }
    }
    
    for ( i = 0; i < dim; i++) {
        for ( j = 0; j < dim; j++) {
            norm = (orig[RIDX(i, j, dim)] - min) / (max - min);
            
            if (compare_floats(norm, result1[RIDX(i, j, dim)])) {
                badi = i;
                badj = j;
                wrong = result1[RIDX(i, j, dim)];
                right = norm;
                err++;
            }
        }
    }

    if (err) {
        printf("\n");
        printf("ERROR: Dimension=%d, %d errors\n", dim, err);
        printf("You have dst[%d][%d] = %f\n",
               badi, badj, wrong);
        printf("It should be dst[%d][%d] = %f\n",
               badi, badj, right);
    }

    return err;
}


void func_wrapper(void *arglist[]) {
    float *src, *mat1, *mat2, *dst2;
    float *dst1;
    int mydim, dim2;
    lab_test_norm f3;
    lab_test_krpro f5;
    int type;
    type = *((int *) arglist[0]);
    if (type == 3) {
        f3 = (lab_test_norm) arglist[1];
        mydim = *((int *) arglist[2]);
        src = (float *) arglist[3];
        dst1 = (float *) arglist[4];
        (*f3)(mydim, src, dst1);

    } else if (type == 5) {
        f5 = (lab_test_krpro) arglist[1];
        mydim = *((int *) arglist[2]);
        dim2 = *((int *) arglist[3]);
        mat1 = (float *) arglist[4];
        mat2 = (float *) arglist[5];
        dst2 = (float *) arglist[6];
        (*f5)(mydim, dim2, mat1, mat2, dst2);

    }
}

/*krpro benchmark*/
void run_krpro_benchmark(int idx, int dim1, int dim2) {
    benchmarks_krpro[idx].tfunct.tfunctkrpro(dim1, dim2, orig, orig2, result2);
}

void test_krpro(int bench_index) {

    int i;
    int test_num;
    char *description = benchmarks_krpro[bench_index].description;
    for (test_num = 0; test_num < DIM_CNT; test_num++) {
        int dim, dim1, dim2;
        /* Check for odd dimension */
        create(ODD_DIM);
        run_krpro_benchmark(bench_index, 8, 12); /*ODD_DIM = 96*/
        if (check_krpro(8,12)) {
            printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
                   benchmarks_krpro[bench_index].description, ODD_DIM);
            return;
        }

        /* Create a test matrix of the required dimension */
        dim = test_dim_krpro[test_num];
        dim1 = test_dim1_krpro[test_num];
        dim2 = test_dim2_krpro[test_num];
        create(dim);
#ifdef DEBUG
        printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_krpro[bench_index].description);
#endif
        /* Check that the code works */
        run_krpro_benchmark(bench_index, dim1, dim2);
        if (check_krpro(dim1, dim2)) {
            printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
                   benchmarks_krpro[bench_index].description, dim);
            return;
        }

        /* Measure CPE */
        {
            double num_cycles, cpe;
            int tmpdim1 = dim1;
            int tmpdim2 = dim2;
            void *arglist[7];
            double dimension = (double) dim;
            double work = dimension * dimension;
#ifdef DEBUG
            printf("DEBUG: dimension=%.1f\n",dimension);
            printf("DEBUG: work=%.1f\n",work);
#endif
            int tmp_type = 5;
            arglist[0] = &tmp_type;
            arglist[1] = (void *) benchmarks_krpro[bench_index].tfunct.tfunctkrpro;
            arglist[2] = (void *) &tmpdim1;
            arglist[3] = (void *) &tmpdim2;
            arglist[4] = (void *) orig;
            arglist[5] = (void *) orig2;
            arglist[6] = (void *) result2;

            create(dim);
            num_cycles = 0;
            num_cycles = fcyc_v((test_funct_v) &func_wrapper, arglist);
            cpe = num_cycles / work;
            benchmarks_krpro[bench_index].cpes[test_num] = cpe;
        }
    }


    /*
     * Print results as a table
     */
    printf("Kronecker Product: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
        printf("\t%d", test_dim_krpro[i]);
    printf("\tMean\n");

    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
        printf("\t%.1f", benchmarks_krpro[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
        printf("\t%.1f", krpro_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute Speedup */
    {
        double prod, ratio, mean;
        prod = 1.0; /* Geometric mean */
        printf("Speedup\t");
        for (i = 0; i < DIM_CNT; i++) {
            if (benchmarks_krpro[bench_index].cpes[i] > 0.0) {
                ratio = krpro_baseline_cpes[i] /
                        benchmarks_krpro[bench_index].cpes[i];
            } else {
                printf("Fatal Error: Non-positive CPE value...\n");
                exit(EXIT_FAILURE);
            }
            prod *= ratio;
            printf("\t%.1f", ratio);
        }

        /* Geometric mean */
        mean = pow(prod, 1.0 / (double) DIM_CNT);
        printf("\t%.1f", mean);
        printf("\n\n");
        if (mean > krpro_maxmean) {
            krpro_maxmean = mean;
            krpro_maxmean_desc = benchmarks_krpro[bench_index].description;
        }
    }


#ifdef DEBUG
    fflush(stdout);
#endif
    return;


}

/*krpro benchmark end*/


void run_norm_benchmark(int idx, int dim) {
    benchmarks_norm[idx].tfunct.tfunctnorm(dim, orig, result1);
}

void test_norm(int bench_index) {

    int i;
    int test_num;
    char *description = benchmarks_norm[bench_index].description;
    for (test_num = 0; test_num < DIM_CNT; test_num++) {
        int dim;
        /* Check for odd dimension */
        create(ODD_DIM);
        run_norm_benchmark(bench_index, ODD_DIM);
        if (check_norm(ODD_DIM)) {
            printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
                   benchmarks_norm[bench_index].description, ODD_DIM);
            return;
        }

        /* Create a test matrix of the required dimension */
        dim = test_dim_norm[test_num];
        create(dim);
#ifdef DEBUG
        printf("DEBUG: Running benchmark \"%s\"\n", benchmarks_multip[bench_index].description);
#endif
        /* Check that the code works */
        run_norm_benchmark(bench_index, dim);
        if (check_norm(dim)) {
            printf("Benchmark \"%s\" failed correctness check for dimension %d.\n",
                   benchmarks_norm[bench_index].description, dim);
            return;
        }

        /* Measure CPE */
        {
            double num_cycles, cpe;
            int tmpdim = dim;
            void *arglist[5];
            double dimension = (double) dim;
            double work = dimension * dimension;
#ifdef DEBUG
            printf("DEBUG: dimension=%.1f\n",dimension);
            printf("DEBUG: work=%.1f\n",work);
#endif
            int tmp_type = 3;
            arglist[0] = &tmp_type;
            arglist[1] = (void *) benchmarks_norm[bench_index].tfunct.tfunctnorm;
            arglist[2] = (void *) &tmpdim;
            arglist[3] = (void *) orig;
            arglist[4] = (void *) result1;

            create(dim);
            num_cycles = 0;
            num_cycles = fcyc_v((test_funct_v) &func_wrapper, arglist);
            cpe = num_cycles / work;
            benchmarks_norm[bench_index].cpes[test_num] = cpe;
        }
    }


    /*
     * Print results as a table
     */
    printf("Normalize: Version = %s:\n", description);
    printf("Dim\t");
    for (i = 0; i < DIM_CNT; i++)
        printf("\t%d", test_dim_norm[i]);
    printf("\tMean\n");

    printf("Your CPEs");
    for (i = 0; i < DIM_CNT; i++) {
        printf("\t%.1f", benchmarks_norm[bench_index].cpes[i]);
    }
    printf("\n");

    printf("Baseline CPEs");
    for (i = 0; i < DIM_CNT; i++) {
        printf("\t%.1f", norm_baseline_cpes[i]);
    }
    printf("\n");

    /* Compute Speedup */
    {
        double prod, ratio, mean;
        prod = 1.0; /* Geometric mean */
        printf("Speedup\t");
        for (i = 0; i < DIM_CNT; i++) {
            if (benchmarks_norm[bench_index].cpes[i] > 0.0) {
                ratio = norm_baseline_cpes[i] /
                        benchmarks_norm[bench_index].cpes[i];
            } else {
                printf("Fatal Error: Non-positive CPE value...\n");
                exit(EXIT_FAILURE);
            }
            prod *= ratio;
            printf("\t%.1f", ratio);
        }

        /* Geometric mean */
        mean = pow(prod, 1.0 / (double) DIM_CNT);
        printf("\t%.1f", mean);
        printf("\n\n");
        if (mean > norm_maxmean) {
            norm_maxmean = mean;
            norm_maxmean_desc = benchmarks_norm[bench_index].description;
        }
    }


#ifdef DEBUG
    fflush(stdout);
#endif
    return;


}


void usage(char *progname) {
    fprintf(stderr, "Usage: %s [-hqg] [-f <func_file>] [-d <dump_file>]\n", progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -h         Print this message\n");
    fprintf(stderr, "  -q         Quit after dumping (use with -d )\n");
    fprintf(stderr, "  -g         Autograder mode: checks only normalize() and kronecker_product()\n");
    fprintf(stderr, "  -f <file>  Get test function names from dump file <file>\n");
    fprintf(stderr, "  -d <file>  Emit a dump file <file> for later use with -f\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
    int i;
    int quit_after_dump = 0;
    int skip_teamname_check = 0;
    int autograder = 0;
    int seed = 1729;
    char c = '0';
    char *bench_func_file = NULL;
    char *func_dump_file = NULL;

    /* register all the defined functions */
    register_kronecker_product_functions();
    register_normalize_functions();

    /* parse command line args */
    while ((c = getopt(argc, argv, "tgqf:d:s:h")) != -1)
        switch (c) {

            case 't': /* skip ID name check (hidden flag) */
                skip_teamname_check = 1;
                break;

            case 's': /* seed for random number generator (hidden flag) */
                seed = atoi(optarg);
                break;

            case 'g': /* autograder mode (checks only normalize() and kronecker_product()) */
                autograder = 1;
                break;

            case 'q':
                quit_after_dump = 1;
                break;

            case 'f': /* get names of benchmark functions from this file */
                bench_func_file = strdup(optarg);
                break;

            case 'd': /* dump names of benchmark functions to this file */
                func_dump_file = strdup(optarg);
                {
                    int i;
                    FILE *fp = fopen(func_dump_file, "w");

                    if (fp == NULL) {
                        printf("Can't open file %s\n", func_dump_file);
                        exit(-5);
                    }
                    for (i = 0; i < norm_benchmark_count; i++) {
                        fprintf(fp, "R:%s\n", benchmarks_norm[i].description);
                    }
                    for (i = 0; i < krpro_benchmark_count; i++) {
                        fprintf(fp, "R:%s\n", benchmarks_krpro[i].description);
                    }
                    fclose(fp);
                }
                break;

            case 'h': /* print help message */
                usage(argv[0]);

            default: /* unrecognized argument */
                usage(argv[0]);
        }

    if (quit_after_dump)
        exit(EXIT_SUCCESS);

    /* Print ID info */
    if (!skip_teamname_check) {
        if (strcmp("eXXXXXX", team.ID1) == 0) {
            printf("%s: Please fill in your ID in kernels.c.\n", argv[0]);
            exit(1);
        }
        //printf("Team Name: %s\n", team.team_name);
        printf("ID: %s\n", team.ID1);
        printf("Name: %s\n", team.name1);
        

        printf("\n");
    }

    srand(seed);

    /*
     * If we are running in autograder mode, we will only test
     * the normalize() and kronecker_product() functions.
     */
    if (autograder) {
        krpro_benchmark_count = 1;
        norm_benchmark_count = 1;
        
        benchmarks_norm[0].tfunct.tfunctnorm = normalize;
        benchmarks_norm[0].description = "normalize() function";
        benchmarks_norm[0].valid = 1;

        benchmarks_krpro[0].tfunct.tfunctkrpro = kronecker_product;
        benchmarks_krpro[0].description = "kronecker_product() function";
        benchmarks_krpro[0].valid = 1;

    }

        /*
         * If the user specified a file name using -f, then use
         * the file to determine the versions of normalize and kronecker_product to test
         */
    else if (bench_func_file != NULL) {

        char flag;
        char func_line[256];
        FILE *fp = fopen(bench_func_file, "r");

        if (fp == NULL) {
            printf("Can't open file %s\n", bench_func_file);
            exit(-5);
        }

        while (func_line == fgets(func_line, 256, fp)) {
            char *func_name = func_line;
            char **strptr = &func_name;
            char *token = strsep(strptr, ":");
            flag = token[0];
            func_name = strsep(strptr, "\n");
#ifdef DEBUG
            printf("Function Description is %s\n",func_name);
#endif


            if (flag == 'N') {
                for (i = 0; i < norm_benchmark_count; i++) {
                    if (strcmp(benchmarks_norm[i].description, func_name) == 0)
                        benchmarks_norm[i].valid = 1;
                }
            }
            if (flag == 'K') {
                for (i = 0; i < krpro_benchmark_count; i++) {
                    if (strcmp(benchmarks_krpro[i].description, func_name) == 0)
                        benchmarks_krpro[i].valid = 1;
                }
            }


        }

        fclose(fp);
    }

        /*
         * If the user didn't specify a dump file using -f, then
         * test all of the functions
         */
    else { /* set all valid flags to 1 */
    
        for (i = 0; i < norm_benchmark_count; i++)
            benchmarks_norm[i].valid = 1;

        for (i = 0; i < krpro_benchmark_count; i++)
            benchmarks_krpro[i].valid = 1;


    }

    /* Set measurement (fcyc) parameters */
    set_fcyc_cache_size(1 << 14); /* 16 KB cache size */
    set_fcyc_clear_cache(1); /* clear the cache before each measurement */
    set_fcyc_compensate(1); /* try to compensate for timer overhead */
  
    for (i = 0; i < norm_benchmark_count; i++) {
        if (benchmarks_norm[i].valid)
            test_norm(i);
    }

    for (i = 0; i < krpro_benchmark_count; i++) {
        if (benchmarks_krpro[i].valid)
            test_krpro(i);
    }




    if (autograder) {
        printf("\nbestscores:%.1f:%.1f:\n", norm_maxmean, krpro_maxmean);
    } else {
        printf("Summary of Your Best Scores:\n");
        printf("  Normalization: %3.1f (%s)\n", norm_maxmean, norm_maxmean_desc);
        printf("  Kronecker Product: %3.1f (%s)\n", krpro_maxmean, krpro_maxmean_desc);
    }

    return 0;
}














