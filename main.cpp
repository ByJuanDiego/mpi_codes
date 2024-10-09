#include <iostream>
#include <mpi.h>
using namespace std;

/*
 *
 *
 * ---------------------------------------- Send types ----------------------------------------
 *
 * I) Blocking Send
 *  When doing a blocking send, MPI guarantees that the buffer
 *  passed can be safely reused once the send returns.
 *
 *    1) MPI_Send (Standard Send)
 *          Behind the scenes, it will issue a buffered send MPI_Bsend or a synchronous send MPI_Ssend.
 *          This decision will be based on whether the buffer attached for buffered sends contains enough
 *           free space for the message to send.
 *          If there is enough space, the buffered send MPI_Bsend will be issued,
 *           otherwise it will revert to a synchronous send MPI_Ssend.
 *
 *    2) MPI_Bsend (Buffered Send)
 *          It will block until a copy of the buffer passed is made.
 *          MPI will effectively send the buffer copied at a later point,
 *           but the user cannot know when.
 *          The MPI buffer in which the copy will be made must be allocated
 *           by the user and explicitly attached to MPI using MPI_Buffer_attach.
 *
 *    3) MPI_Ssend (Synchronous Send)
 *          1. The sender sends a request-to-send message.
 *          2. The receiver stores this request.
 *          3. When a matching receive is posted,
 *              the receiver sends back a permission-to-send message,
 *              and the sender now sends the message.
 *
 *    4) MPI_Rsend (Ready Send)
 *          This routine requires the recipient to have issued the corresponding receive
 *           routine (MPI_Recv or MPI_Irecv) before MPI_Rsend is invoked.
 *          This particularity may improve performance by saving the time normally spent
 *           hand-shaking with the receiving MPI process.
 *          It will decide whether to use the synchronous or asynchronous mode using the
 *           same decision flow that MPI_Send.
 *
 * II) Non-blocking Send
 *  When doing a non-blocking send, the user must not attempt to reuse the buffer after
 *  the send returns without explicitly checking for send completion (using MPI_Wait or MPI_Test).
 *
 *    1) MPI_Isend (Immediate Return Standard Send)
 *      The word standard indicates that this routine is not explicitly told whether to send the message in a
 *       synchronous mode or asynchronous mode. Instead, MPI_Isend will make that decision itself
 *    2) MPI_Ibsend (Immediate Return Buffered Send)
 *      Unlike its blocking counterpart MPI_Bsend, MPI_Ibsend will not block until that copy is made,
 *       instead it will return immediately.
 *      This implies that when MPI_Ibsend returns, the buffer provided may not have
 *       been copied yet so reusing this buffer is unsafe.
 *    3) MPI_Issend (Immediate Return Synchronous Send)
 *      Unlike its blocking counterpart MPI_Ssend, MPI_Issend will not block until the recipient has
 *       received the message. In other words, when MPI_Issend returns,
 *       the buffer passed may not have been sent yet, and it must be considered unsafe to reuse the buffer passed.
 *      The user must therefore check for completion with MPI_Wait or MPI_Test before safely reusing the buffer passed.
 *       Note that MPI_Issend may be implicitly invoked by the standard non-blocking send (MPI_Isend)
 *    4) MPI_Irsend (Immediate Return Ready Send)
 *      It will decide whether to use the synchronous or asynchronous mode using the same decision flow that MPI_Isend.
 *      As a non-blocking send, MPI_Irsend will immediately return and the buffer passed
 *       must not be seen as safe to reuse until the completion of MPI_Irsend has been checked.
 *
 */

int main(int argc, char *argv[]) {
    int rank, size, data = 0;
    MPI_Status status;
    const int err = MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (err != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    if (rank == 2) {
        data = 20;

        MPI_Send(&data, 1, MPI_INT, 0, 777, MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_INT, 1, 777, MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_INT, 3, 777, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&data, 1, MPI_INT, 2, 777, MPI_COMM_WORLD, &status);
    }

    printf("Peekaboo! from %d/%d with data=%d\n", rank, size - 1, data);

    MPI_Finalize();
}
