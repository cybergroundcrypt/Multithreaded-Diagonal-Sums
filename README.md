# Multithreaded-Diagonal-Sums
Finds all diagonal sums equal to an input s 


The program will find all of the diagonal sums equal to an input s, an unsigned long value, in an input text file consisting of a 2D grid of n-by-n digits (1 through 9), where n ≥ 1, using t threads, where 1 ≤ t ≤ 3. The program should write the output of all of the diagonal sums equal to s to an output file.

Examples of the commands with their output:



./proj4.out in1.txt out1.txt 10 1; diff out1.txt correctOut1.txt | wc -c;
Computing the diagonal sums equal to 10 in a 5-by-5 grid using 1 thread(s).
Elapsed time for computing the diagonal sums using 1 thread(s): 0.000002 seconds.
Writing the diagonal sums equal to 10 to the file out1.txt.
Program is complete. Goodbye!
0


./proj4.out in1.txt out1.txt 10 1; diff out1.txt correctOut1.txt | wc -c;
Computing the diagonal sums equal to 10 in a 5-by-5 grid using 1 thread(s).
Elapsed time for computing the diagonal sums using 1 thread(s): 0.000002 seconds.
Writing the diagonal sums equal to 10 to the file out1.txt.
Program is complete. Goodbye!
0

./proj4.out in5.txt out5.txt 1222 3; diff out5.txt correctOut5.txt | wc -c;
Computing the diagonal sums equal to 1222 in a 3567-by-3567 grid using 3 thread(s).
Elapsed time for computing the diagonal sums using 3 thread(s): 11.546067 seconds.
Writing the diagonal sums equal to 1222 to the file out5.txt.
Program is complete. Goodbye!
0
