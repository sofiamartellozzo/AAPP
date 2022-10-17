
#include <iostream> 
#include <list>
#include <random>
using namespace std;


//cell definition
/* Each cell is composed of 
    x: column
    y: row
 */
struct Cell {
  int x;
  int y;
  struct Cell *father;
  bool Dx_wall; 
  bool Dw_wall; 
};

struct Walls{
  struct Cell *cell1;
  struct Cell *cell2;
  bool orientation; //0 = vertical 1 = orizontal
};

Cell** GenerateMaze(int num_rows,int num_cols){
  //at first generate the maze
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

list<Walls> GenerateWalls(int num_rows, int num_cols, Cell** maze){

  list<Walls> walls;
  for(int i=0; i<num_rows; i++){
    for(int j=0; j<num_cols; j++){
      if(j<num_cols-1) {
        
        Walls wall1;
        wall1.cell1 = &maze[i][j];
        wall1.cell2 = &maze[i][j+1]; //dx
        wall1.orientation = 0; //orizontal
        walls.push_back(wall1);
        
        };
      if(i<num_rows-1) {
        
        Walls wall2;
        wall2.cell1 = &maze[i][j];
        wall2.cell2 = &maze[i+1][j]; //down
        wall2.orientation = 1; //vertical
        walls.push_back(wall2);
      
        };
    }
  }
  return walls;
}

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

void Union (Cell *c1, Cell *c2){
  Cell *aux_father1;
  Cell *aux_father2;

  aux_father1 = Find_Set(c1);
  aux_father2 = Find_Set(c2);

  aux_father1->father = aux_father2;
}

bool DFS(Cell **maze, Cell  *start, Cell  *goal){
  Cell *sol;
  sol = Find_Set(start);
  if((sol->x == goal->x)&&(sol->y == goal->y)){
    return true;
  }
  else{
    return false;
  }
}



int main(){
  // cout << "Hello" << endl;

  int num_cols = 8;
  int num_rows = 6;

  Cell **maze;
  maze = GenerateMaze(num_rows, num_cols);

  //generate the list of walls
  list<Walls> walls;
  walls = GenerateWalls(num_rows, num_cols, maze);
  
  
  Cell  *start = &maze[0][0];
  Cell  *goal = &maze[num_rows-1][num_cols-1];

  while(!Find(start, goal)){

    int walls_lenght = walls.size();
    cout << "size of the list is:" << walls_lenght << endl;

    std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1,walls_lenght); // Guaranteed unbiased

    auto random_integer = uni(rng);
    cout << "random number: " << random_integer << endl;

    //initialize iterator to list
    list<Walls>::iterator it = walls.begin();
    //move the iterator by random_integer -1 elements, -1 because the list starts from 0
    advance(it, random_integer-1);

    cout << it->cell1->x << it->cell1->y << endl;
    cout << it->cell2->x << it->cell2->y << endl;

    Cell *neighbor1 = it->cell1;
    Cell *neighbor2 = it->cell2;
    if(!Find(neighbor1, neighbor2)){
      walls.erase(it);
      int walls_lenght = walls.size();
      cout << "size of the list is:" << walls_lenght << endl;
      Union(neighbor1, neighbor2);
    }
  }

  //now test the solution
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
    }

  return 0;
  
}

}
