//A straightforward program comprising two loops, one to initialise an array, the other to sum the numbers in the array.
//First we write the loops sequentially, to provide a reference value for the computed sum. Then parallelize the loops using OMP pragmas.
//Program will print two results, the sum computed sequentially, and then as computed in parallel.
//headers
#include <stdio.h>
#include <omp.h>

int main() {
    //use an array of size 1000.
    const int N=1000;
    int a[N];

    //Note: I did not set a default array such as int a[] = {2, 2, 4, 1000}; I just used the size for sum.

    //Initialised the array
    for (int i=0; i < N; i++)
    a[i] = i;// ith element contains i.

    //Initialise variables of sequentail and parallel sums
    int sequential_sum=0;
    int parallel_sum=0;

    //Write a sequential for loop to sum the elements of the array,
    for (int i=0; i< N; i++)//summation loop
    sequential_sum += a[i];
    //and print the sum.
    printf("sequential_sum=%d\n", sequential_sum);

    //Write a parallel for loop, as described in Section 5.5 of the textbook, to initialise the array.
    //Description from text book:
    // # pragma omp parallel for num_threads(thread_count) \ reduction(+: approx)
    //Parallelize the summation loop using a parallel directive to establish a team of threads to carry out the summation
    # pragma omp parallel for num_threads(2) \
    reduction(+: parallel_sum)//reduction clause
    for (int i = 0; i < N; i++)//summation loop
    parallel_sum += a[i];
    //compute the global sum.
    printf("parallel_sum=%d\n", parallel_sum);
}

/* notes:

textbook 5.5:
h = (b-a)/n;
approx = (f(a) + f(b))/2.0;
for (i = 1; i <= n-1; i++)
approx += f(a + i*h);
approx = h*approx;
by simply placing a directive immediately before the for loop:
h = (b-a)/n;
approx = (f(a) + f(b))/2.0;
# pragma omp parallel for num_threads(thread_count) \
reduction(+: approx)
for (i = 1; i <= n-1; i++)
approx += f(a + i*h);
approx = h*approx;

textbook 5.12 exercises:
float a[] = {2.0, 2.0, 4.0, 1000.0};
int i;
float sum = 0.0;
for (i = 0; i < 4; i++)
sum += a[i];
printf("sum = %4.1f\n", sum);
int i;
float sum = 0.0;
# pragma omp parallel for num_threads(2) \
reduction(+:sum)
for (i = 0; i < 4; i++)
sum += a[i];
printf("sum = %4.1f\n", sum);

from https://tildesites.bowdoin.edu/~ltoma/teaching/cs3225-GIS/fall17/Lectures/openmp.html
//example4.c: add all elements in an array in parallel
#include < stdio.h >
int main() {
  const int N=100;
  int a[N];
  //initialize
  for (int i=0; i < N; i++)
    a[i] = i;
  //compute sum
  int local_sum, sum;
#pragma omp parallel private(local_sum) shared(sum)
  {
    local_sum =0;
    //the array is distributde statically between threads
#pragma omp for schedule(static,1)
    for (int i=0; i< N; i++) {
      local_sum += a[i];
    }
    //each thread calculated its local_sum. ALl threads have to add to
    //the global sum. It is critical that this operation is atomic.
#pragma omp critical
    sum += local_sum;
  }
  printf("sum=%d should be %d\n", sum, N*(N-1)/2);
}

*/