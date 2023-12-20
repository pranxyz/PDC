// Program: Open MP "Hello World" with output ordered by rank.
// Used Program 5.1 from the textbook as a starting point, and modified it to achieve the desired objective.

/* File:     omp_hello.c
 *
 * Purpose:  A parallel hello, world program that uses OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_hello omp_hello.c
 * Run:      ./omp_hello <number of threads>
 *
 * Input:    none
 * Output:   A message from each thread
 *
 * IPP:      Section 5.1 (pp. 211 and ff.)
 */

// headers
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    // declare thread_count which will be used in omp parallel
    int thread_count = strtol(argv[1], NULL, 10);
    // print the number of threads
    // creating all the threads
    // start of the parallel zone
    #  pragma omp parallel num_threads(thread_count)
    // #  pragma omp single // I tried to implement using single with mutilple for loops event single no wait but I was not able to get the desired output.
    // I also kept getting the common error "work-sharing region may not be closely nested inside of work-sharing, critical, ordered, master or explicit task region".
    // This happened when a different omp was nested in the single omp.
    #  pragma omp for ordered schedule(static, 1)

    // for loop for number of threads
    for(int i = 0; i < omp_get_num_threads(); i++)
    {
        // now the output will be in order always
        #pragma omp ordered
        {
            Hello();
        }
        //  end of the parallel zone
    }
        #pragma omp single
    printf("Number of threads = %d\n", thread_count);
return 0;
}  /* main */

// Hello function has not been modified. I used the same function provided by the textbook.
/*-------------------------------------------------------------------
 * Function:    Hello
 * Purpose:     Thread function that prints message
 */
void Hello(void) {
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();
   printf("Hello from thread %d of %d\n", my_rank, thread_count);

}  /* Hello */