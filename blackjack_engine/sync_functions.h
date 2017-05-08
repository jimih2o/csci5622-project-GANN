#include "mpi.h"

template<typename T>
void SendToZero(T *buf, int count, int rank);


template<>
void SendToZero<float>(float *buf, int count, int rank)
{
    MPI_Send(buf, count, MPI_FLOAT, 0, rank, MPI_COMM_WORLD);
}

template<>
void SendToZero<double>(double *buf, int count, int rank)
{
    MPI_Send(buf, count, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
}

template<>
void SendToZero<int>(int *buf, int count, int rank)
{
    MPI_Send(buf, count, MPI_INT, 0, rank, MPI_COMM_WORLD);
}






template<typename T>
void RecvFromZero(T *buf, int count, int rank);

template<>
void RecvFromZero<float>(float *buf, int count, int rank)
{
    MPI_Status status;
    MPI_Recv(buf, count, MPI_FLOAT, 0, rank, MPI_COMM_WORLD, &status);
}

template<>
void RecvFromZero<double>(double *buf, int count, int rank)
{
    MPI_Status status;
    MPI_Recv(buf, count, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD, &status);
}

template<>
void RecvFromZero<int>(int *buf, int count, int rank)
{
    MPI_Status status;
    MPI_Recv(buf, count, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
}







template<typename T>
void ZeroSendTo(T *buf, int count, int rank_dest);

template<>
void ZeroSendTo<int>(int *buf, int count, int rank_dest) {
    MPI_Send(buf, count, MPI_INT, rank_dest, rank_dest, MPI_COMM_WORLD);
}

template<>
void ZeroSendTo<float>(float *buf, int count, int rank_dest) {
    MPI_Send(buf, count, MPI_FLOAT, rank_dest, rank_dest, MPI_COMM_WORLD);
}

template<>
void ZeroSendTo<double>(double *buf, int count, int rank_dest) {
    MPI_Send(buf, count, MPI_DOUBLE, rank_dest, rank_dest, MPI_COMM_WORLD);
}







template<typename T>
void ZeroRecv(T *buf, int count, int rank_src);

template<>
void ZeroRecv<int>(int *buf, int count, int rank_src) {
    MPI_Status status;
    MPI_Recv(buf, count, MPI_INT, rank_src, rank_src, MPI_COMM_WORLD, &status);
}

template<>
void ZeroRecv<float>(float *buf, int count, int rank_src) {
    MPI_Status status;
    MPI_Recv(buf, count, MPI_FLOAT, rank_src, rank_src, MPI_COMM_WORLD, &status);
}

template<>
void ZeroRecv<double>(double *buf, int count, int rank_src) {
    MPI_Status status;
    MPI_Recv(buf, count, MPI_DOUBLE, rank_src, rank_src, MPI_COMM_WORLD, &status);
}