// Author: Bishal Sarang
// N Queens Problem Visualisation using OpenGL


/* TODO:
  2. Provide command line arguments to control animation speed and value of N.
  3. Fix while refreshing the window.
*/

#include <iostream>
#include<ctime>
#include<algorithm>
#include <vector>
#include <windows.h>
#include <GL/glut.h>

// Debug Macro
#define what(x) cerr << #x << " => " << x << endl;

using namespace std;

// Constants
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;
const int OFFSET_X = 100;
const int OFFSET_Y = 100;
const int SIDE_LENGTH = 70;
const int SLEEP_TIME = 500;


enum Mode{ NOSTEPS,
           STEPS,
};

// Datatype to represent Black and White Color
enum Color{ BLACK,
            WHITE,
          };

// Datatype to represent box states: ocuupied or empty
enum State{ EMPTY,
            OCCUPIED,
          };

// Datatype to represent safe and unsafe states
enum Safe{ UNSAFE,
           SAFE,

};

// Datatype to store information of each boxes
struct box{ int x1, y1, x2, y2;
            Color color;
};

struct textColor{ float red, green, blue, alpha;
};

// Size of board
int N = 4;
Mode mode = STEPS;
int total_solutions = 0;

// Display text
string text = "Solving for N = " + to_string(N);
string display_text = "Displaying solution: ";
// Vector to store info{end coordinates of box and color} of N * N boxes
vector<box> boxes;

// Board of size N * N
vector<vector<int>> grid(N, vector<int>(N, EMPTY));
// 2D array of box_infos to map with grid of N * N
vector<vector<box>> box_coordinates(N, vector<box>(N));


// Function Prototypes
void draw_box(Color c, int x1, int y1, int x2, int y2);
void put_queen(int x1, int y1);
void put_text(int x1, int y1, textColor color,  char c);
bool is_safe(vector<vector<int>>grid, int r, int c);
bool solve(vector<vector<int>> grid, int row);
void flip_color(Color &color);
void draw_board();
void display_texts(int x, int y, int width, textColor color, string s);

void init(){
  // Set background color to blue
  glClearColor (0.0, 0.0, 1.0, 1.0) ;
  glClear(GL_COLOR_BUFFER_BIT) ;
  glPointSize(4.0);
  glMatrixMode(GL_PROJECTION);
  // Bottom left corner is (0, 0) and upper right corner is (WINDOW_WIDTH, WINDOW_HEIGHT)
  gluOrtho2D(0, WINDOW_WIDTH,0,WINDOW_HEIGHT);
}

// Function to draw queen inside box
void put_queen(int x1, int y1){
  glColor3f(0,255,0);
  int mid = SIDE_LENGTH / 2;

  glBegin(GL_POINTS);
    glVertex2i(x1 + mid + 12, y1 + SIDE_LENGTH - 16);
    glVertex2i(x1 + mid - 12, y1 + SIDE_LENGTH - 16);
    glVertex2i(x1 + mid + 24, y1 + SIDE_LENGTH - 24);
    glVertex2i(x1 + mid - 24, y1 + SIDE_LENGTH - 24);
  glEnd();


  glBegin(GL_LINES);
   glVertex2i(x1 + mid - 24, y1 + SIDE_LENGTH - 24);
   glVertex2i(x1 + mid - 16, y1 + 16);

   glVertex2i(x1 + mid - 16, y1 + 16);
   glVertex2i(x1 + mid + 16, y1 + 16);

   glVertex2i(x1 + mid + 16, y1 + 16);
   glVertex2i(x1 + mid + 24, y1 + SIDE_LENGTH - 24);

   glVertex2i(x1 + mid + 24, y1 + SIDE_LENGTH - 24);
   glVertex2i(x1 + mid + 12, y1 + SIDE_LENGTH - 32);

   glVertex2i(x1 + mid + 12, y1 + SIDE_LENGTH - 32);
   glVertex2i(x1 + mid + 12, y1 + SIDE_LENGTH - 16);

   glVertex2i(x1 + mid + 12, y1 + SIDE_LENGTH - 16);
   glVertex2i(x1 + mid, y1 + SIDE_LENGTH - 32);

   glVertex2i(x1 + mid, y1 + SIDE_LENGTH - 32);
   glVertex2i(x1 + mid - 12, y1 + SIDE_LENGTH - 16);

   glVertex2i(x1 + mid - 12, y1 + SIDE_LENGTH - 16);
   glVertex2i(x1 + mid - 12, y1 + SIDE_LENGTH - 32);

   glVertex2i(x1 + mid - 12, y1 + SIDE_LENGTH - 32);
   glVertex2i(x1 + mid - 24, y1 + SIDE_LENGTH - 24);

  glEnd();
  glFlush();
}

// Function to draw character on screen
void put_text(int x1, int y1, textColor text_color, char c){
  glColor3f(text_color.red, text_color.green, text_color.blue);
  glRasterPos2i (x1 + 27, y1 + 27);
  glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, c);
  glFlush();
}

void mark(int r, int c, Safe st){
  auto Point = box_coordinates[r][c];
  int x1 = Point.x1, y1 = Point.y1, x2 = Point.x2, y2 = Point.y2;
  if(st == UNSAFE){
    glColor3f(255, 0, 0);
    glRectf(x1, y1, x2, y2);
  }
  else{
    draw_box(Point.color, x1, y1, x2, y2);
    put_queen(x1, y1);
    put_text(x1, y1, {0, 255, 0, 0}, 'Q');
  }
  glFlush();
}

// Checks if it is safe to put queen at cell (r, c)
bool is_safe(vector<vector<int>>grid, int r, int c){
  // For every other rows of column c
  for(int i = r - 1; i >= 0; i--){
      if(grid[i][c] == OCCUPIED){
          mark(i, c, UNSAFE);
          mark(r, c, UNSAFE);
          Sleep(SLEEP_TIME);
          mark(i, c, SAFE);
          mark(r, c, SAFE);
          return false;
      }
  }

  // Check Diagonally
  int dx[] = {-1, -1};
  int dy[] = {1, -1};

  for(int i = 0; i < 2; i++){
      int x = r;
      int y = c;
      while(x + dx[i] >= 0 and x + dx[i] < grid.size()
            and y + dy[i] >= 0 and y + dy[i] < grid.size()){
          x += dx[i];
          y += dy[i];
          if(grid[x][y] == OCCUPIED){
            mark(x, y, UNSAFE);
            mark(r, c, UNSAFE);
            Sleep(SLEEP_TIME);
            mark(x, y, SAFE);
            mark(r, c, SAFE);
            return false;
          }
      }
  }
  return true;
}

// Draws black or white boxes
void draw_box(Color c, int x1, int y1, int x2, int y2){
     if(c == BLACK){
        glColor3f(0,0,0);
     }
     else{
       glColor3f(255,255,255);
     }
     boxes.push_back({x1, y1, x2, y2, c});
     glRectf(x1, y1, x2, y2);
     glFlush();
}

// Solves N Queen Problem
bool solve(vector<vector<int>> grid, int row){
  // If N queens are placed in every rows
  // we have the solution
  if(row == grid.size()){
    total_solutions++;
    display_texts(10, 20, 13, {0, 255, 0, 0}, display_text + to_string(total_solutions));
    cout << "Displaying solution " << total_solutions << endl;
    Sleep(15 * SLEEP_TIME);
    display_texts(10, 20, 13, {0, 0, 255, 0}, display_text + to_string(total_solutions));
    return false;
  }

  for(int col = 0; col < grid.size(); col++){
    auto point = box_coordinates[row][col];
    int x1 = point.x1, y1 = point.y1, x2 = point.x2, y2 = point.y2;
    Color color = point.color;

    put_text(x1, y1 - 5,{0, 255, 0, 0}, 'Q');
    put_queen(point.x1, point.y1);
    if(mode == STEPS){
      Sleep(SLEEP_TIME);
    }

    if(is_safe(grid, row, col)){
      grid[row][col] = OCCUPIED;
      Sleep(2 * SLEEP_TIME);
      if(solve(grid, row + 1)){
        return true;
      }
      if(mode == STEPS){
        Sleep(SLEEP_TIME);
      }
      grid[row][col] = EMPTY;
      draw_box(color, x1, y1, x2, y2);
    }
    else{
      if(mode == STEPS){
       Sleep(SLEEP_TIME);
      }
      draw_box(color, x1, y1, x2, y2);
    }
  }
  return false;
}


void flip_color(Color &color){
  if(color == BLACK){
      color = WHITE;
  }
  else{
      color = BLACK;
  }
}


void display_texts(int x, int y, int width, textColor text_color, string s){
  for(auto c: s){
    put_text(x, y, text_color, c);
    x += width;
  }
}


void draw_board(){
  boxes.clear();
  Color color = WHITE;

  int x = 10, y = 0;
  display_texts(x, y, 10, {0, 255, 0, 0},  text);
  x = 100, y = 100;
  for(int row = 0; row < N; row++){
    int x1 = x;
    int y1 = y;
    int x2 = x1 + SIDE_LENGTH;
    int y2 = y1 + SIDE_LENGTH;
    // Increment row
    y = y2;
    flip_color(color);
    if(N % 2){
      flip_color(color);
    }
    for(int col = 0; col < N; col++){
      // Draw box
      draw_box(color, x1, y1, x2, y2);
      // Flip color
      flip_color(color);
      x1 += SIDE_LENGTH;
      x2 += SIDE_LENGTH;
    }
  }
  // Sort by row coordinates
  // Then col coordinates
  sort(boxes.begin(), boxes.end(), [](box l, box r){
       if(l.y1 == r.y1){
        return l.x1 < r.x1;
       }
       return l.y1 > r.y1;
  });

  int k = 0;
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      box_coordinates[i][j] = boxes[k++];
    }
  }
 solve(grid, 0);
}


int main(int argc, char** argv){
   //  cin >> N;
    if(argc >= 2){
        //to_string(argv[1]);
        if(true){
            cout << "Here";
            N = stoi(argv[2]);
        }

    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    // Set Window size , position and title
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(800, 100);
    glutCreateWindow("N-QUEENS");

    // Set cootdinate system to match window size
    init();
    glutDisplayFunc(draw_board);
    // Wait for events
    glutMainLoop();
    return 0;
}
