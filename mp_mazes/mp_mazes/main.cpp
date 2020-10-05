#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "cs225/PNG.h"

using namespace std;

int main()
{
  SquareMaze ma;
  ma.makeMaze(15,15);

  cs225::PNG* creative = ma.drawCreativeMaze();
  creative->writeToFile("creative.png");

  //std::vector<int> soln = ma.solveMaze();
}
