#include <iostream>
#include<bits/stdc++.h>
#include <cmath>
#include <ctime>

#include <sys/resource.h>

using namespace std;

// node structure
struct node
{
    vector<int> boardState;
    int g, h, f;
    node *parent;

    node()
    {
        g = 0;
        h = 0;
        f = 0;
        parent = NULL;
    }
};

// operator overloading for priority queue
struct comp
{
    bool operator()(const node *node1, const node *node2) const
    {
        return node1->f > node2->f;
    }
};

int numElem; // number of elements
int NG = 0;  // Nodes Generated
int NE = -1; // Nodes Expanded
int D = 0;   // Depth of tree
int bStar = 0;
int heuChoice = 0;
int maxDepth = 25;
vector<int> goalBoard = {1,2,3,8,0,4,7,6,5};//goal state
node* initGoal = new node();

priority_queue<node *, vector<node *>, comp> OPEN; // priority queue
vector<node *> CLOSED;                             // to keep track of closed nodes
vector<node *> totalPath;                          // all the nodes in the path from the root to the goal

// print board
void printBoard(node *n)
{
    int dim = sqrt(numElem);
    int k = 0;
    for (int i = 0; i < dim; ++i){
        for (int j = 0; j < dim; ++j){
            cout << n->boardState[k++] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// print path from root to goal node
void getFinalPath(node *n)
{
    node *temp = n;

    while (temp != NULL){
        totalPath.push_back(temp);
        temp = temp->parent;
    }

    int totalSize = totalPath.size();

    // show the moves one by one
    cout << "Total Path" << endl;
    for (int i = totalSize - 1; i >= 0; --i){
        printBoard(totalPath[i]);
    }
}

// return true if goal nonde
bool goalReached(node *n)
{
    if (n->h == 0){
        return 1;
    }
    else{
        return 0;
    }
}



//customHeuristic
int customHeuristic(node* n) {

    int conflicts = 0;
    int inversions = 0;
    initGoal->boardState = goalBoard;
    int check = 0;
    for(int i = 0; i < 9; i++){
        if(n->boardState[i] == initGoal->boardState[i] ){
            check++;
        }
    }


    if(check == 9){
        return 0;
    }

    // Count the number of conflicts
    for (int i = 0; i < n->boardState.size() - 1; i++) {
        for (int j = i + 1; j < n->boardState.size(); j++) {
            if (n->boardState[i] > n->boardState[j]) {
                int row_i = i / 3;
                int col_i = i % 3;
                int row_j = j / 3;
                int col_j = j % 3;
                if (row_i == row_j || col_i == col_j) {
                    conflicts++;
                }
            }
        }
    }

    // Count the number of inversions
    for (int i = 0; i < n->boardState.size() - 1; i++) {
        for (int j = i + 1; j < n->boardState.size(); j++) {
            if (n->boardState[i] && n->boardState[j] && n->boardState[i] > n->boardState[j]) {
                inversions++;
            }
        }
    }

    return conflicts + inversions;
}


//calculate heuristic
int misplacedTileHeuristic(node* n){
    int count = 0;
    for(int i = 0; i < numElem; ++i){
        if(goalBoard[i] != n->boardState[i]){
            ++count;//tiles out of place
        }
    }
    return count;
}

// build successor node
node *getSuccessor(node *state, int pos1, int pos2)
{
    NG++; // increment nodes generated
    node *newState = new node();
    newState->boardState = state->boardState;//copy the board state
    swap(newState->boardState[pos1], newState->boardState[pos2]);
    switch(heuChoice) {//decide which heuristic to use
        case 1:
            newState->h = misplacedTileHeuristic(newState);
            break;
        case 2:
            newState->h = customHeuristic(newState);
            break;
        default:
            std::cout << "Invalid Choice\n";
            break;
    }
    newState->g = state->g + 1; // increment depth
    newState->f = newState->h + newState->g; // apply formula
    newState->parent = state;

    return newState;
}

// generate successors
vector<node *> getSuccessors(node *n)
{
    NE++;
    int pos;
    int row;
    int col;
    int dim;//dimension of the game
    for (int i = 0; i < numElem; ++i)
    {
        if (n->boardState[i] == 0)
        {
            pos = i;
            break;
        }
    }
    dim = sqrt(numElem);
    row = pos / dim;
    col = pos % dim;

    vector<node *> successors;

    if (col != 0){//move left
            successors.push_back(getSuccessor(n, pos, pos - 1));
    }
    if (col != dim - 1){//move right
        successors.push_back(getSuccessor(n, pos, pos + 1));
    }
    if (row != 0){//move up
        successors.push_back(getSuccessor(n, pos, pos - dim));
    }
    if (row != dim - 1){//mode down
        successors.push_back(getSuccessor(n, pos, pos + dim));
    }

    return successors;
}



// check if node has been previously generated
bool checkOLD(node *n)
{
    int totalSize = CLOSED.size(), j;
    for (int i = 0; i < totalSize; ++i){
        for (j = 0; j < numElem; ++j){
            //check if the node is inside CLOSED
            if (n->boardState[j] != CLOSED[i]->boardState[j])
                break;
        }
        if (j == numElem){//node was found
            return 1;
        }
    }
    return 0;//node was not found
}

void A_star(node *n)
{
    switch(heuChoice) {//decide which heuristic to use
        case 1:
            n->h = misplacedTileHeuristic(n);
            break;
        case 2:
            n->h = customHeuristic(n);
            break;
        default:
            std::cout << "Invalid Choice\n";
            break;
    }
    n->f = n->h;
    n->parent = NULL;
    OPEN.push(n);

    bool done;
    int totalGCost, totalSize, k;
    node *current, *temp;
    vector<node *> currentSuccessors;
    priority_queue<node *, vector<node *>, comp> Pqueue;

    while (!OPEN.empty()){//loop as long as there is boards in OPEN
        current = OPEN.top();//best f value board
        OPEN.pop();
        CLOSED.push_back(current);

        if (goalReached(current)){
            getFinalPath(current);
            return;
        }

        currentSuccessors.clear();
        currentSuccessors = getSuccessors(current);

        totalSize = currentSuccessors.size();
        for (int i = 0; i < totalSize; ++i){
            if (checkOLD(currentSuccessors[i])){
                continue;
            }

            totalGCost = current->g + 1;

            while (!Pqueue.empty()){
                Pqueue.pop();
            }
            while (!OPEN.empty()){
                temp = OPEN.top();
                OPEN.pop();
                done = 0;

                for (k = 0; k < numElem; ++k){//loop through elements
                    if (currentSuccessors[i]->boardState[k] != temp->boardState[k]){
                        break;
                    }
                }
                if (k == numElem){
                    done = 1;
                }

                if (done && totalGCost < temp->g){//if done with successors, and total cost of g < temp->g
                    temp->parent = current;
                    temp->g = totalGCost;
                    temp->f = temp->g + temp->h;// apply f = g + h
                }
                Pqueue.push(temp);
            }
            if (!done){
                Pqueue.push(currentSuccessors[i]);
            }
            OPEN = Pqueue;

            // update the depth
                if (currentSuccessors[i]->g > D) {
                    D = currentSuccessors[i]->g;
                }

                // check if the depth limit has been reached
                if (D == maxDepth) {
                    getFinalPath(currentSuccessors[i]);
                    cout << "GOAL NOT REACHED" << endl << endl;
                    return;
                }
        }
    }

    return;
}

int main()
{

    struct rusage usage;

    node *newNode = new node();
    numElem = 9;

    node* initNode1 = new node();
    node* initNode2 = new node();
    node* initGoal = new node();

    cout << "Goal State" << endl;
    initGoal->boardState = goalBoard;
    printBoard(initGoal);

    cout << "State 1" << endl;
    vector<int> puzzle1 = {2,8,3,1,6,4,0,7,5};//initial state 1
    initNode1->boardState = puzzle1;
    printBoard(initNode1);

    cout << "State 2" << endl;
    vector<int> puzzle2 = {2,1,6,4,0,8,7,5,3};//initial state 2
    initNode2->boardState = puzzle2;
    printBoard(initNode2);


    cout << "Which Initial State?: ";
    vector<int> puzzle = {0};
    int puzzleChoice = 0;
    cin >> puzzleChoice;
    cout << endl;

    cout << "Which Heuristic would you like to use?: " << endl;
    cout << "(1) Misplaced Tiles or (2) Custom Heuristic: ";
    cin >> heuChoice;
    cout << endl;

    if (puzzleChoice == 1){ // initial state 1 selected
        puzzle = puzzle1;
    }
    else if (puzzleChoice == 2){ // initial state 2 selected
        puzzle = puzzle2;
    }
    else{ // wrong initial state
        cout << "invalid choice" << endl;
        return 0;
    }
    cout << endl;

    //start time 1
    getrusage(RUSAGE_SELF, &usage);
    clock_t start_r = usage.ru_utime.tv_sec * 1000000 + usage.ru_utime.tv_usec;

    newNode->boardState = puzzle;
    cout << "Initial State" << endl;
    printBoard(newNode);

    A_star(newNode);
    D = totalPath.size() - 1;
    bStar = ((float)NG / (float)D);

    //end time
    getrusage(RUSAGE_SELF, &usage);
    clock_t end_r = usage.ru_utime.tv_sec * 1000000 + usage.ru_utime.tv_usec;
    double cpu_time_used_r = ((double) (end_r - start_r)) / 1000000;
    float ET = cpu_time_used_r;
    stringstream ss;
    ss << fixed << setprecision(6) << ET;
    string Exact_ET = ss.str();


    cout << "METRICS: Execution Time(ET), Nodes Generated (NG), " << endl
    << "Nodes Expanded (NE), Depth (D), and Branching Factor (b*)" << endl << endl;
    const int col_width = 6;
    cout << left << setw(col_width + 5) << " ET"
                 << setw(col_width) << "NG"
                 << setw(col_width) << "NE"
                 << setw(col_width) << "D"
                 << setw(col_width) << "b*" << endl;

    cout << left << setw(col_width + 5) << Exact_ET
                 << setw(col_width) << NG
                 << setw(col_width) << NE
                 << setw(col_width) << D
                 << setw(col_width) << bStar << endl;
}
