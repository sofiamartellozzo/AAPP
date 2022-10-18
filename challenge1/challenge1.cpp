
#include <iostream> 
#include <list>
#include <random>
using namespace std;


/* cell definition */
struct Cell {
  int x;                // column
  int y;                // row

  struct Cell *father;  // pointer to its father

  bool visited;         // marker for DFS 
  list<Cell> adj_cells; // adjacent list of cells (for DFS)

  bool Dx_wall;         // for representation
  bool Dw_wall; 
};

/* walls definition */
struct Walls{
  struct Cell *cell1;   // one side cell
  struct Cell *cell2;   // other side cell
  bool orientation;     // 0 = vertical 1 = orizontal
};

list<Cell> sol;
list<Cell> adj;


/* method to generate the grid with all the cells at the beguinning */
Cell** GenerateMaze(int num_rows,int num_cols){
  
  Cell **maze = new Cell *[num_rows];

  for(int i=0; i<num_rows; i++){

    maze[i] = new Cell[num_cols];
    for(int j=0; j<num_cols; j++){

      maze[i][j].x = i;
      maze[i][j].y = j;
      maze[i][j].father = &maze[i][j]; //at first each set is comosed of only the cell itself
    }
  }
  return maze;
}

/* method to initialize all the walls in the grid at the beguinning */
list<Walls> GenerateWalls(int num_rows, int num_cols, Cell** maze){

  list<Walls> walls;

  for(int i=0; i<num_rows; i++){
    for(int j=0; j<num_cols; j++){
      if(j<num_cols-1) {
        
        Walls wall1;
        wall1.cell1 = &maze[i][j];
        wall1.cell2 = &maze[i][j+1]; // dx
        wall1.orientation = 0;       // orizontal
        walls.push_back(wall1);
        
      };
      if(i<num_rows-1) {
        
        Walls wall2;
        wall2.cell1 = &maze[i][j];
        wall2.cell2 = &maze[i+1][j]; // down
        wall2.orientation = 1;       // vertical
        walls.push_back(wall2);
      
      };
    }
  }
  return walls;
}

/* recursive method to find the representative of the cell given in input */
Cell* Find_Set(Cell *c){

    if ((c->x == c->father->x)&&(c->y == c->father->y)){
        //we found the representative
        return c;
    }
    else{
        //go up on the tree
        Cell * aux_pointer;
        aux_pointer = Find_Set(c->father);
        return aux_pointer;
    }
}

/* method to verify if the cells given as input belongs to the same set (= have the same representative) */
bool Find(Cell *c1, Cell *c2){

  Cell *father1;
  Cell *father2;

  father1 = Find_Set(c1);
  father2 = Find_Set(c2);

  if ((father1->x == father2->x)&&(father1->y == father2->y)){
    return true;
  }
  else{
    return false;
  }
}

/* method to join to cell in the same set
  if them already belongs to two different sets, join these two */
void Union (Cell *c1, Cell *c2){

  Cell *aux_father1;
  Cell *aux_father2;

  aux_father1 = Find_Set(c1);
  aux_father2 = Find_Set(c2);

  aux_father1->father = aux_father2;

  c1->adj_cells.push_back(*c2);
  c2->adj_cells.push_back(*c1);
}

/* Depth First Search Algorithm to find the path from s to g */
list<Cell> DFS(Cell  *n, Cell  *goal, Cell **maze){

  cout << "LALA" << endl;

  cout << "sol: " << sol.size() << endl;
  cout << "adj: " << adj.size() << endl;

  Cell * node;
  node = &maze[n->x][n->y];

  sol.push_back(*node);
  node->visited = true;
  if(adj.size() > 0){
    
    adj.remove(*node);        //remove the first element (that is node)
  }
  cout << "sol: " << sol.size() << endl;
  cout << "adj: " << adj.size() << endl;
  
  if((node->x == goal->x)&&(node->y == goal->y)){

    return sol;
  }
  else{

    // debug
    cout << "(" << node->x << "," << node->y << ") -> " << endl;

    list<Cell> node_adj;
    node_adj = node->adj_cells; 
    list<Cell>::iterator it = node_adj.begin();

    for(int i = 0; i < node_adj.size(); i++){

      advance(it, i);
      if(it->visited == false){

        adj.push_front(*it);
      }
    }
    cout << "sol: " << sol.size() << endl;
    cout << "adj: " << adj.size() << endl;
    Cell * new_node;
    new_node = &adj.front();
    cout << "sol: " << sol.size() << endl;
    cout << "adj: " << adj.size() << endl;
    sol = DFS(new_node, goal, maze);
    return sol;
  }
}



int main(){

  int num_cols = 8;
  int num_rows = 6;

  // generate the grid of all cells
  Cell **maze;
  maze = GenerateMaze(num_rows, num_cols);

  // generate the list of walls
  list<Walls> walls;
  walls = GenerateWalls(num_rows, num_cols, maze);
  
  Cell  *start = &maze[0][0];
  Cell  *goal = &maze[num_rows-1][num_cols-1];

  // repeat until s and g are in the same set
  while(!Find(start, goal)){

    // debug
    int walls_lenght = walls.size();
    cout << "size of the list is:" << walls_lenght << endl;

    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1,walls_lenght); // Guaranteed unbiased
    auto random_integer = uni(rng);

    // debug
    cout << "random number: " << random_integer << endl;

    // initialize iterator to list
    list<Walls>::iterator it = walls.begin();
    //move the iterator by random_integer -1 elements, -1 because the list starts from 0
    advance(it, random_integer - 1);

    // debug
    cout << it->cell1->x << it->cell1->y << endl;
    cout << it->cell2->x << it->cell2->y << endl;

    // get the two cell faced by the wall selected
    Cell *neighbor1 = it->cell1;
    Cell *neighbor2 = it->cell2;

    // check if belongs to the same set
    if(!Find(neighbor1, neighbor2)){

      walls.erase(it);  // remove the wall

      // debug
      int walls_lenght = walls.size();
      cout << "size of the list is:" << walls_lenght << endl;

      // join the two cell in the same set
      Union(neighbor1, neighbor2);
    }
  }

  list<Cell> path ;  // the path from s to g
  //list<Cell> adj;       // where to store the adjacent cells for DFS
  
  //path = DFS(start, goal, adj, path);
  path = DFS(start, goal, maze);

  /*
  bool solution = DFS(maze, start, goal);
  if (solution){
    cout << "Path found!" << endl;
  }
  else{
    cout << "Path not found" << endl;
  }

  //now print the solution
  //first set for each cell wich wall is still up
  list<Walls>::iterator iter = walls.begin();
  for(int w = 0; w < walls.size(); w++ ){
    advance(iter, w);
    if(iter->orientation == 0){
      //orizontal wall
      iter->cell1->Dx_wall = true;
    }
    else{
      //vertical wall
      iter->cell1->Dw_wall = true;
    }
  }

  //to print it I will generate n_rows string
  list<string> rows;
  string roof;
  for (int i = 0; i < num_cols; i++){
    string one_roof = "-";
    roof += one_roof;
  }

  for(int r = 0; r < num_rows; r++){
    string row;
    for(int c = 0; c < num_cols; c++){
      string cell_str;
      if (r == 0){
        //first row
        if (c == 0){
          if ((maze[r][c].Dx_wall)&&(maze[r][c].Dw_wall)){
            cell_str = "";
          }
        }
      }
    }*/

  return 0;
  
}



