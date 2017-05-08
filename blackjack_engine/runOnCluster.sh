#!/bin/bash
#
#PBS -N testJob
#PBS -e testJob.err
#PBS -m abe
#PBS -M acourt@ball.com
#PBS -l nodes=320:ppn=1

cd $PBS_O_WORKDIR
mpiexec -machinefile $PBS_NODEFILE BlackjackEngine
