CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.
Kyle:
    Wrote Code

Bella:
    Wrote Report


REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.


The table below shows the total elapsed (real) time taken by the program for different threshold values:

|    Threshold     |  Real Time  |
|------------------|-------------|
|     2,097,152    |    0.372    |
|     1,048,576    |    0.213    |
|       524,288    |    0.158    |
|       262,144    |    0.141    |
|       131,072    |    0.112    |
|        65,536    |    0.111    |
|        32,768    |    0.116    |
|        16,384    |    0.124    |

We see that as the threshold decreases, the total real time increases. This is because the threshold controls when the parallel sort algorithm switches from parallel execution to sequential sorting.

High threshold (2097152, 1048576):
- sorts the slowest compared to lower thresholds
- most of the sorting is done sequentially, since the threshold is so large that few or no parallel sort calls are made
- overhead from creating and managing multiple processes is very small, if any
- doesn't benefit from paralleism, doesn't sort multiple chunks of the data at the same time

Moderate thresholds (524288 to 65536):
- these values seem to provide the best performance
- the sort algorithm begins to divide the work into sub-tasks that can be executed in parallel
- the overhead from managing parallel processes remains relatively low, while benefiting from parallel execution

Low threshold (32768, 16384):
- the array is divided into many small pieces
- each small piece may be sorted in its own process, causing a lot of overhead probably due to
    - many process creations and also taking them down
    - memory duplication overhead
    - more competition for cpu resources
- while these times are still better than the high threshold where there was little to no paralleism, the overhead of managing these processes starts to outweight the gains, since the runtimes start increasing

In terms of runtime, threshold = 65,536 proved to be the best. This threshold is in the middle of the different threshold we tested, towards the bottom. 
This behavior shows that more paralleism is not always better, and that its costs might exceed the computational savings. However, a moderate amount of paralleism should still be used to take fully advantage of the multiple cpu cores.

