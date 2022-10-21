
#include <iostream> 
#include <list>
#include <random>
#include <algorithm>
using namespace std;


/* cell definition */
struct Cell {
  int x;                // column
  int y;                // row

  struct Cell *father;         // pointer to its father

  bool visited = false;        // marker for DFS 
  list<Cell* > adj_cells;      // adjacent list of cells (for DFS)

  bool Dx_wall = true;         // for representation
  bool Dw_wall = true; 
};

/* walls definition */
struct Walls{
  struct Cell *cell1;   // one side cell
  struct Cell *cell2;   // other side cell
  bool orientation;     // 0 = vertical 1 = orizontal
};

list<Cell* > sol;   // where DFS will store the path
list<Cell* > adj;   // where DFS will store the adjacent nodes


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
        wall1.orientation = 0;       // vertical
        walls.push_back(wall1);
        
      };
      if(i<num_rows-1) {
        
        Walls wall2;
        wall2.cell1 = &maze[i][j];
        wall2.cell2 = &maze[i+1][j]; // down
        wall2.orientation = 1;       // orizontal
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

  c1->adj_cells.push_back(c2);  // set the near by reference for the visualization
  c2->adj_cells.push_back(c1);

}

/* Depth First Search Algorithm to find the path from s to g */
list<Cell* > DFS(Cell  *node, Cell  *goal){

  sol.push_back(node);
  node->visited = true;
  if(adj.size() > 0){

    adj.remove(node);        //remove the first element 
  }
  
  if((node->x == goal->x)&&(node->y == goal->y)){

    cout << "(" << node->x << "," << node->y << ") " << endl;
    return sol;
  }
  else{

    // debug
    cout << "(" << node->x << "," << node->y << ") -> ";

    list<Cell* > node_adj;
    node_adj = node->adj_cells; 

    for(auto it = node_adj.begin(); it != node_adj.end(); ++it){
        
        Cell* c = (*it);
        if(c->visited == false){
            
            adj.push_front(*it);
        }
    }
    Cell * new_node;
    new_node = adj.front();
    sol = DFS(new_node, goal);

    return sol;
  }
}

void PrintMaze(int num_cols, int num_rows, Cell ** maze){
  string roof;
  roof += " ";
  string one_roof = "__";
  for (int i = 0; i < num_cols; i++){
    roof += one_roof;
  }

  cout << roof << endl;

  for(int r = 0; r < num_rows; r++){
    string row;
    row += "|";
    for(int c = 0; c < num_cols; c++){
      if (maze[r][c].Dx_wall){
        if(maze[r][c].Dw_wall){
          row += "_|";
        }
        else{
          row += " |";
        }
        
      }
      else if(maze[r][c].Dw_wall){
        row += "_ ";
      }
      else{
        row += "  ";
      }
    }
    cout << row << endl;
  }
}

bool contains(list<Cell* > l, Cell* c){
  return std::find(std::begin(l),std::end(l),c) != std::end(l);
}

void PrintPath(int num_cols, int num_rows, Cell ** maze, list<Cell* > path){
  string roof;
  roof += " ";
  string one_roof = "___";
  for (int i = 0; i < num_cols; i++){
    roof += one_roof;
  }

  cout << roof << endl;

  bool exists ;

  for(int r = 0; r < num_rows; r++){
    string row;
    row += "|";
    for(int c = 0; c < num_cols; c++){

      exists = contains(path, &maze[r][c]); //check if current cell is in the path

      if(!exists){
        if (maze[r][c].Dx_wall){
          if(maze[r][c].Dw_wall){
            row += "__|";
          }
          else{
            row += "  |";
          }
            
        }
        else if(maze[r][c].Dw_wall){
          row += "__ ";
        }
        else{
          row += "   ";
        }
      }
      else{
        if (maze[r][c].Dx_wall){
          if(maze[r][c].Dw_wall){
            row += "x_|";
          }
          else{
            row += "x |";
          }
            
        }
        else if(maze[r][c].Dw_wall){
          row += "x_ ";
        }
        else{
          row += "x  ";
        }

      }

    }
    cout << row << endl;
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

    int walls_lenght = walls.size();

    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1,walls_lenght); // Guaranteed unbiased
    auto random_integer = uni(rng);

    cout << "random number: " << random_integer << endl;

    // initialize iterator to list
    list<Walls>::iterator it = walls.begin();
    //move the iterator by random_integer -1 elements, -1 because the list starts from 0
    advance(it, random_integer - 1);

    cout << "(" << it->cell1->x << "," << it->cell1->y << ") ";
    cout << "(" << it->cell2->x << "," << it->cell2->y << ") : ";

    // get the two cell faced by the wall selected
    Cell *neighbor1 = it->cell1;
    Cell *neighbor2 = it->cell2;

    // check if belongs to the same set
    if(!Find(neighbor1, neighbor2)){

      cout << "remove wall" << endl;

      if(it->orientation == 0){
        //vertical wall removed
        neighbor1->Dx_wall = false;
      }
      else{
        //orizontal
        neighbor1->Dw_wall = false;
      }

      // join the two cell in the same set
      Union(neighbor1, neighbor2);

      PrintMaze(num_cols, num_rows, maze);

    }
    else{
      cout << "do not remove the wall" << endl;
      PrintMaze(num_cols, num_rows, maze);
    }
    walls.erase(it);  // remove the wall from the list
  }

  cout << "Path foud with DFS: " << endl;

  list<Cell* > path ;  // the path from s to g
  path = DFS(start, goal);

  //now print the final solution
  PrintMaze(num_cols, num_rows, maze);
  PrintPath(num_cols, num_rows, maze, path);

  return 0;
  
}



