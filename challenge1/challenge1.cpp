
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
  struct Cell *dx_cell;
  struct Cell *dw_cell;
  bool Dx_wall; //= true;
  bool Dw_wall; //= true;
};

struct Walls{
  struct Cell *cell1;
  struct Cell *cell2;
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
  /*
  //then connect all near by cell
  for(int i=0; i<num_rows+1; i++){
    for(int j=0; j<num_cols+1; j++){
      if(j<num_cols) {maze[i][j].dx_cell = &maze[i][j+1];};
      if(i<num_rows) {maze[i][j].dw_cell = &maze[i+1][j];};
    }
  }*/
  return maze;
}

Cell* Find_Set(Cell *c){
  cout << "search the father" << endl;
    if ((c->x == c->father->x)&&(c->y == c->father->y)){
        //we found the representative
        return c;
    }
    else{
        //go up on the tree
        cout << "call recursion" << endl;
        Cell * aux_pointer;
        aux_pointer = Find_Set(c->father);
        return aux_pointer;
    }
}

bool Find(Cell *c1, Cell *c2){
  cout << "enter??" << endl;
  Cell *father1;
  Cell *father2;
  father1 = Find_Set(c1);
  cout << "CAXXX" << endl;
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

  aux_father2->father = aux_father1;
}



int main(){
  // cout << "Hello" << endl;

  int num_cols = 8;
  int num_rows = 6;

  Cell **maze;
  maze = GenerateMaze(num_rows, num_cols);

//   cout << "second" << endl;
//   cout << maze[5][0].y << endl;
//   cout << "second" << endl;
//   cout << maze[0][0].x << endl;

  //generate the list of walls
  int num_walls = (num_rows-1)*(num_cols-1);
  list<Walls> walls;
  for(int i=0; i<num_rows; i++){
    for(int j=0; j<num_cols; j++){
      if(j<num_cols-1) {
        // cout << "wall1: " << i << j << endl;
        Walls wall1;
        wall1.cell1 = &maze[i][j];
        wall1.cell2 = &maze[i][j+1]; //dx
        walls.push_back(wall1);
        // cout << wall1.cell1->x << wall1.cell1->y << endl;
        // cout << wall1.cell2->x << wall1.cell2->y << endl;
        };
      if(i<num_rows-1) {
        // cout << "wall2: " << i << j << endl;
        Walls wall2;
        wall2.cell1 = &maze[i][j];
        wall2.cell2 = &maze[i+1][j]; //down
        walls.push_back(wall2);
        // cout << wall2.cell1->x << wall2.cell1->y << endl;
        // cout << wall2.cell2->x << wall2.cell2->y << endl;
        };
    }
  }
  
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


  cout << maze[3][0].x << endl;
  return 0;
  
}



