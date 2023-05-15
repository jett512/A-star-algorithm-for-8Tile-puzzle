# Contributors
The team for this project includes, 
* Jett Tipsword (jbt71@txstate.edu)
* Rayyan Khan (r_k150@txstate.edu)
* Rafael Hernandez (reh119@txstate.edu)

# Description
Project 2 tasks a team of students to implement the A* algorithm, specifically the A* FINAL algorithm, in C++ and apply it to the 8 tile puzzle game. The team needs to develop the A* algorithm discussed in class with its heuristic. Also, each team member should individually create their own custom heuristic function, that notably avoids using distance calculation like the provided heuristic. The A* algorithm is a search algorithm that uses the actual cost of getting from the start node to the current node and an estimate of the cost of getting from the current node to the goal node to determine which node to expand next. The 8-puzzle game is a sliding puzzle with a 3x3 grid and 9 numbered tiles that must be rearranged into numerical order. Here the 9th number is 0, but for this project 0 is treated as an empty space. The performance of the algorithm will be analyzed by generating tables with execution time, number of nodes generated, number of nodes expanded, depth of the tree, effective branching factor, and total path for two different initial states of the game. Each member should pick a unique heuristic function, test the program with all four functions, and then analyze the results of the program.

# Usage 
* This program was developed and edited on the Texas State University Linux server.
* Make sure that the Project2_jbt71.cpp file is in the correct directory before proceeding to the next steps.

* Once in the correct directory, compile the code using
g++ -o Proj2 -std=c++11 Project2_jbt71.cpp

* After compilation run the program with
./Proj2

* The user will be shown the goal state, and both initial states.
From here the user will decide which initial state they would like to proceed with.
Valid answers only include '1' or '2', then hit enter

* After the user will be asked which heuristic they would like to use.
The two heuristics are the misplaced tile heuristic and the custom heuristic that I developed
Valid answers only include '1' or '2', then hit enter

* To attempt other initial states or Heuristics, the user can run the program again './Proj2' and repeat the process
