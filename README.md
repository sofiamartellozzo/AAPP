# AAPP
In this repo there are the two assignment developed for the course *Advanced Algorithm and Parallel Programming* (AAPP).\
<br />
Course held @Politecnico di Milano.\
Acadamic year 2022 - 2023
## Challenge 1: Maze Generator with DisjointSet
The algorithm works as follows:
1. Generate the set of cells and walls
2. Use the disjoint-set data structure to compute the random maze
3. Test the solution with Depth First Search algorithm

Each Cell of the grid is represented as a *struct* of: two integers (x,y) representing its coordiantes and a pointer to another Cell representing the father.
On the other hand, the Wall is also a structure composed of two pointers to Cell struct and a boolean value to determine if is orizontal or vertical its orientation.

### First optimization
Updatin the new father of also a child of a node and the node itself (if its father is not itself)
### Second optimization
Path compression: add the update of the father (representative) in the Find_Set function 


## Challenge 2: MPI for grep parallelization
Grep is a function that given as input a text and a word, search row by row in which of them the word is present. With MPI is developed a way to distribute equally the search among different and parallel process. The number of parallel processes is also set as input when lounching the program.
