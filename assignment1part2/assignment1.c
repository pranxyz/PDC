// Simple circular ring program that will ask for how many shifts and shifts into chronological form.

#include <stdio.h>
#include <mpi.h> // For MPI functions, etc

int main(int argc, char *argv[]) {
  // Initialising; rank = process rank, size = number of process
  MPI_Status status;
  int a, rank, size, tag, to, from;

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  tag = 1; // Randomly choosen.
  // Messages are sent with an accompanying user-defined integer tag,
  // to assist the receiving process in identifying the message.
  to = (rank + 1) % size; // Rank of the next process in the ring. Used the modulus "%".
  from = (rank + size - 1) % size; // Operator so that the last process "wraps around" to rank.

// Asking user to specify how many times we want to go around the ring.
  if (rank == 0) {
    printf("Enter the number of times around the ring: ");
    scanf("%d", &a);
    --a;
// Printing the first send
    printf("Process %d sending %d to %d (right)\n", rank, a, to);
    MPI_Send(&a, 1, MPI_INT, to, tag, MPI_COMM_WORLD);
  }

  // While loop for circular ring.
  // logic: When each processes receives the 0 message, it passes it on
  // to the next process and then quits. By passing the 0 first,
  // every process gets the 0 message in order to quit.

  while (1) {

    MPI_Recv(&a, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
    printf("Process %d received %d (left)\n", rank, a);

    if (rank == 0) {
      a--;
    }

    printf("Process %d sending %d to %d (right)\n", rank, a, to);
    MPI_Send(&a, 1, MPI_INT, to, tag, MPI_COMM_WORLD);

    if (a == 0) {
      break;
    }
  }

  // The last process does one extra send to process 0, which needs
  // to be received before the program can exit.
  if (rank == 0)
    MPI_Recv(&a, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);

  // Quit.
  MPI_Finalize();
  return 0;
}