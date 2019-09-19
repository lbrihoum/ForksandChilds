# Forks, child processes, and pipes
## Introductory program assignment for learning pipes, child processes, and the use case of fork

## Description
- A program written in C that will read the number of children and the elements of the array as the command-line argument and will output the result. 

- The program works for 1, 2, and 3 children

### Example program execution:
- /lina 2 2 3 5 7 1 3 
- The number of children is 2, and the elements of the array are {2, 3, 5, 7, 1, 3}

## How the Program was executed

- This programming assignment was coded in C where the program will read an array from the terminal where the first element will be the program name, the second element being how many children there are, and the rest of the array being the array that needs to be calculated. 

- In the program, the parent will use the function fork() to create child processes and send the array that the parent wants calculated. Depending on how many childs there are, the parent will send each child a portion of the array to be calculated. The child processes will compute the partial sum and send the data back to the parent. The parent will then add all partial sums if there is more than one process and then compute the final sum of the array and output the final sum.

- Every parent and child created using fork() will obtain a process identifier (PID) and will need to pass data to each other using pipes. My program will make a new pipe for every child, since there is always one child, there will always be at least one pipe. The pipe has a read and write function, since the child has the data from the parent when initilized, the parent does not need to write any data to the children, it only needs to read the partial sum being sent from the children. For every partial sum being received, the parent will have to wait for the result to come before computing the final sum.

- I started this program by first initializing a fork and if there is more than one child processes, the parent will create another fork for the second child, and another fork for the third child. There is always a conditional statement to see if the fork() is not equal to 0 to ensure we do the correct calculations in the child process. Every child process is the same when calculating the partial sum and sending it to the parent. However, depending on how many children there are will determine how much of the array the child will compute.

- Once the child has computed the sum, it will write the data to the parent using the pipe. The parent will then read pipe and calculate the sum from every child and then add all sums together to calculate the final sum.

## Outcome using the above example program execution
- I am the parent with pid: 2 sending the the array: 2, 3, 5, 7, 1, 3
- I am the child with pid: 3, adding the array 2, 3, 5 and sending partial sum 10
- I am the child with pid: 4, adding the array 7, 1, 3, and sending partial sum 11
- I am the parent of pid: 3, and pid: 4.  I have pid: 2 and got partial results 10, and 11
- Final result is 21!

### Thank you!

