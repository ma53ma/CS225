#pragma once

#include<vector>
#include "cs225/PNG.h"
#include<map>
/* Your code here! */

using namespace std;
using namespace cs225;

class SquareMaze {
  public:
    SquareMaze();
    PNG * drawCreativeMaze() const;
    bool canTravel(int x, int y, int dir) const;
    PNG * drawMaze() const;
    PNG * drawMazeWithSolution();
    void makeMaze(int width, int height);
    void setWall(int x, int y, int dir, bool exists);
    vector<int> solveMaze();


  private:
    vector<vector<int>> wallOrder; //actual walls we removed
    vector<vector<int>> walls; // removes walls we removed and cycles
    vector<int> tiles;
    map<int, vector<int>> wallsRemoved; //actual ways you can go
    map<int, vector<int>> wallsKept;
    int width_;
    int height_;
    bool copyMaze;


};
