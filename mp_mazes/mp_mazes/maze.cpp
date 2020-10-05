#include<vector>
#include "maze.h"
#include "cs225/PNG.h"
#include "dsets.h"
#include <cmath>
#include<map>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<chrono>
#include<random>
#include<algorithm>
#include<queue>
using namespace std;
using namespace cs225;

using std::cout;
using std::endl;

/*
No-parameter constructor.

Creates an empty maze.
*/
SquareMaze::SquareMaze() {
  width_ = 0;
  height_ = 0;
  wallOrder.clear();
  walls.clear();
  tiles.clear();
  wallsRemoved.clear();
}

PNG * SquareMaze::drawCreativeMaze() const {
  PNG * maze = new PNG(width_*30 + 1, height_*30 + 1);
  HSLAPixel black(0,0,0);
  for (int x = 0; x < width_*30 + 1; x++) {
    if (x >= (width_*30 + 1)/4 && x <= 3*(width_*30 + 1)/4) {
      maze->getPixel(x,(height_*30 + 1)/4) = black;
      maze->getPixel(x,3*(height_*30 + 1)/4) = black;
      if (x >= (width_*30 + 1)/6 && x <= 3*(width_*30 + 1)/8) {
        maze->getPixel(x,3*(height_*30 + 1)/8) = black;
        maze->getPixel(x,5*(height_*30 + 1)/8) = black;
      }
      if (x >= 5*(width_*30 + 1)/8 && x <= 3*(width_*30 + 1)/4) {
        maze->getPixel(x,3*(height_*30 + 1)/8) = black;
        maze->getPixel(x,5*(height_*30 + 1)/8) = black;
      }
    }
  }

  for (int y =0; y < width_*30 + 1; y++)      {
    if (y >= 5*(height_*30 + 1)/8 && y < 3*(height_*30 + 1)/4) {
        maze->getPixel((width_*30 + 1)/4, y) = black;
        maze->getPixel(3*(width_*30 + 1)/4, y) = black;
    }
    if (y >= (height_*30 + 1)/4 && y < 3*(height_*30 + 1)/8) {
        maze->getPixel((width_*30 + 1)/4, y) = black;
        maze->getPixel(3*(width_*30 + 1)/4, y) = black;
    }
    if (y >= 3*(height_*30 + 1)/8 && y < 5*(height_*30 + 1)/8) {
      maze->getPixel(3*(width_*30 + 1)/8, y) = black;
      maze->getPixel(5*(width_*30 + 1)/8, y) = black;

    }
  }

  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      if (!canTravel(x,y, 0)) {
        if (x >= (width_/4 + 1) && x <= (3*width_/4 - 1) && (y >= (height_/4 + 1) && y <= (3*height_/4 - 1)))     {
          if (x >= (width_/4 + 1) && x <= (3*width_/8) && y >= 3*width_/8 && y <= (5*width_/8)) {
            continue;
          }
          if (x >= (5*width_/8) && x <= (3*width_/4) && y >= 3*width_/8 && y <= (5*width_/8)) {
            continue;
          }
          for (int k = 0; k <= 10; k++) {
            maze->getPixel((x + 1) * 30, y*30 + k) = black;
          }
        }
      }
      if (!canTravel(x, y, 1)) {
        if (x >= (width_/4 + 1) && x <= (3*width_/4 - 1) && (y >= (height_/4 + 1) && y <= (3*height_/4 - 1)))      {
          if (x >= (width_/4 + 1) && x <= (3*width_/8) && y >= 3*width_/8 && y <= (5*width_/8)) {
            continue;
          }
          if (x >= (5*width_/8) && x <= (3*width_/4) && y >= 3*width_/8 && y <= (5*width_/8)) {
            continue;
          }
          for (int k =0; k <= 10; k++) {
            maze->getPixel(x*30 +k, (y + 1)*30) = black;
          }
        }
      }
    }
  }

  return maze;
}

/*
Makes a new SquareMaze of the given height and width.

If this object already represents a maze it will clear all the existing data
before doing so. You will start with a square grid (like graph paper)
with the specified height and width. You will select random walls to delete
without creating a cycle, until there are no more walls that could be deleted
without creating a cycle. Do not delete walls on the perimeter of the grid.

Hints: You only need to store 2 bits per square: the "down" and "right" walls.
The finished maze is always a tree of corridors.)

Parameters
  width	- The width of the SquareMaze (number of cells)
  height - The height of the SquareMaze (number of cells)
*/
void SquareMaze::makeMaze(int width, int height) {

  wallOrder.clear();
  walls.clear();
  tiles.clear();
  wallsRemoved.clear();
  width_ = width;
  height_ = height;
  DisjointSets dset = DisjointSets();
  dset.addelements(width * height);
  walls.resize(width*height);

  copyMaze = true;


  for (int i = 0; i < (width * height) - 1 ; i++) { //can we get away with cutting last one off? has no walls
    tiles.push_back(i); // adding all tiles to vector

    int offRightCheck = (i % width) + 1; //past right side of maze

    if (offRightCheck < width) {

      walls[i].push_back(i + 1); // adding potential right wall
    }

    int offDownCheck = i + width; // off bottom of maze
    if (offDownCheck < (height * width)) {

      walls[i].push_back(i + width); // adding potential down wall
    }
  }

  //list of all potential walls

  int index = (std::chrono::system_clock::now().time_since_epoch().count() % tiles.size());
  if (index <= 1) {
    index = 2;
  }
  /*
  while (dset.size() != width_ * height_) {
    int index = (std::chrono::system_clock::now().time_since_epoch().count() % walls.size());

  }
  */


  for(int i = index; i != index - 1; i=  (i + 1) % tiles.size()) {
    for (int j : walls[i]) {
      if (dset.find(i) != dset.find(j)) { // if not in same tree
        //cout << "erasing " << i << " and " << j << endl;
        dset.setunion(i, j); // combine
        map<int, vector<int>>::iterator tileIt = wallsRemoved.find(i);
        if (tileIt != wallsRemoved.end()) {
          (tileIt->second).push_back(j);
        } else {
          wallsRemoved[i] = {j};
        }
      }
    }
  }
}

/*
This uses your representation of the maze to determine whether it is possible
to travel in the given direction from the square at coordinates (x,y).

For example, after makeMaze(2,2), the possible input coordinates will
be (0,0), (0,1), (1,0), and (1,1).

dir = 0 represents a rightward step (+1 to the x coordinate)
dir = 1 represents a downward step (+1 to the y coordinate)
dir = 2 represents a leftward step (-1 to the x coordinate)
dir = 3 represents an upward step (-1 to the y coordinate)
You can not step off of the maze or through a wall.

This function will be very helpful in solving the maze.
It will also be used by the grading program to verify
that your maze is a tree that occupies the whole grid,
and to verify your maze solution. So make sure that this function works!

Parameters
  x	- The x coordinate of the current cell
  y	- The y coordinate of the current cell
  dir -	The desired direction to move from the current cell

Returns
  whether you can travel in the specified direction
*/
bool SquareMaze::canTravel(int x, int y, int dir) const {
  int curr = x + y*width_;
  int next;
  if (dir == 0) {
    if (x + 1 >= width_) {
      return false;
    }
    next = curr + 1;
  } else if (dir == 1) {
    if (y + 1 >= height_) {
      return false;
    }
    next = curr + width_;
  } else if (dir == 2) {
    if (x - 1  < 0) {
      return false;
    }
    next = curr - 1;
  } else {
    if (y - 1 < 0) {
      return false;
    }
    next = curr - width_;
  }
  int small = min(curr, next);
  int big = max(curr,next);
  if (wallsRemoved.find(small) == wallsRemoved.end()) {
    return false;
  }
  for (int path : wallsRemoved.at(small)) {
    if (path == big) {
      return true;
    }
  }
  return false;
}

/*
Sets whether or not the specified wall exists.

This function should be fast (constant time). You can assume that in grading
we will not make your maze a non-tree and then call one of the other member
functions. setWall should not prevent cycles from occurring, but should simply
set a wall to be present or not present. Our tests will call setWall to copy
a specific maze into your implementation.

Parameters
  x	- The x coordinate of the current cell
  y	- The y coordinate of the current cell
  dir	- Either 0 (right) or 1 (down), which specifies which wall to set
        (same as the encoding explained in canTravel). You only need to
        support setting the bottom and right walls of every square in the grid.
  exists - true if setting the wall to exist, false otherwise
*/
void SquareMaze::setWall(int x, int y, int dir, bool exists) {

  if (copyMaze) {
    wallsRemoved.clear();
    copyMaze = false;
  }



  // may be missing ones that would create cycle otherwise

  if (!exists) {

    //cout << "supposed not to be walls at" << endl;
    //cout << x << " and " << y << " with direction " << dir  << "with " << exists << endl;
    int curr = x + y*width_;
    int next;
    if (dir == 0) {
      next = curr + 1;
    } else {
      next = curr + width_;
    }
    map<int, vector<int>>::iterator tileIt = wallsRemoved.find(curr);
    if (tileIt != wallsRemoved.end()) {
      for (int i : tileIt->second) {
        if ( i == next) {
          return;
        }
      }
      (tileIt->second).push_back(next);
    } else {
      wallsRemoved[curr] = {next};
    }

  }
}

/*
Solves this SquareMaze.

For each square on the bottom row (maximum y coordinate), there is a distance
from the origin (i.e. the top-left cell), which is defined as the length
(measured as a number of steps) of the only path through the maze from the
origin to that square.

Select the square in the bottom row with the largest distance from the origin
as the destination of the maze. solveMaze() returns the winning path from the
origin to the destination as a vector of integers, where each integer
represents the direction of a step, using the same encoding as in canTravel().

If multiple paths of maximum length exist, use the one with the destination
cell that has the smallest x value.

Hint: this function should run in time linear in the number of cells in the maze.

Returns
  a vector of directions taken to solve the maze
*/
vector<int> SquareMaze::solveMaze() {

  vector<bool> visited;
  for (int i = 0; i < width_ * height_; i++) {
    visited.push_back(false);
  }
  std::map<int, int> prev;
  std::queue<int> q;
  vector<int> endpoints;
  cout << "what " << endl;

  q.push(0);
  cout << "what " << endl;

  visited[0] = true;
  cout << "what " << endl;

  while (!q.empty()) {

    int front = q.front();
    int x = front % width_;
    int y = front / width_;
    if (y == height_ - 1) {
      endpoints.push_back(front);
    }

    q.pop();
    if (canTravel(x, y, 0) && !visited[front + 1]) {
      q.push(front + 1);
      prev[front + 1] = front;
      visited[front + 1] = true;

    }
    if (canTravel(x, y, 1) && !visited[front + width_]) {
      q.push(front + width_);
      prev[front + width_] = front;
      visited[front + width_] = true;

    }
    if (canTravel(x, y, 2) && !visited[front - 1]) {
      q.push(front - 1);
      prev[front - 1] = front;
      visited[front - 1] = true;

    }
    if (canTravel(x, y, 3) && !visited[front - width_]) {
      q.push(front - width_);
      prev[front - width_] = front;
      visited[front - width_] = true;
    }
    cout << "here" << endl;

  }
  vector<int> path;
  int last = width_ - 1;
  while (endpoints[last] == endpoints[last - 1]) {
    last--;
  }
  int curr = endpoints[width_ - 1];
  while (curr != 0) {
    int pre = prev[curr];
    if (curr == pre + 1) {
      path.push_back(0);
    } else if (curr == pre + width_) {
      path.push_back(1);
    } else if (curr == pre - 1) {
      path.push_back(2);
    } else if (curr == pre - width_) {
      path.push_back(3);
    }
    curr = pre;
  }
  std::reverse(path.begin(), path.end());
  return path;
  /*
  int maxIndex = 0;
  int index = 0;
  int maxSize = 0;
  for (std::map<std::vector<int>, std::vector<int>> pre : prevVect) {
    if ((int) pre.size() > maxSize) {
      maxSize = pre.size();
      maxIndex = index;
    }
    index++;
  }
  //std::map<std::vector<int>, int> finalDir = dirVect[maxIndex];
  std::map<std::vector<int>, std::vector<int>> preFinal = prevVect[maxIndex];
  vector<int> curr = {xvals[maxIndex],height_ - 1};

  while ((curr[0] != 0) || (curr[1] != 0)) {
    vector<int> prevStep = preFinal[curr];
    //path.push_back(finalDir[curr]);
    curr = preFinal[curr];
  }


  for (int i : path) {
    cout << i << " ";

  }
  cout << "" << endl;
  */
}

/*
Draws the maze without the solution.

First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1).
where height and width were the arguments to makeMaze.
Blacken the entire topmost row and leftmost column of pixels, except the entrance
(1,0) through (9,0). For each square in the maze, call its maze coordinates (x,y).
If the right wall exists, then blacken the pixels with coordinates ((x+1)*10,y*10+k)
for k from 0 to 10. If the bottom wall exists, then blacken the pixels with
coordinates (x*10+k, (y+1)*10) for k from 0 to 10.

The resulting PNG will look like the sample image, except there will be no
exit from the maze and the red line will be missing.

Returns
  a PNG of the unsolved SquareMaze
*/
PNG * SquareMaze::drawMaze() const {
  PNG * maze = new PNG(width_*10 + 1, height_*10 + 1);
  HSLAPixel black(0,0,0);
  for (int x = 0; x < width_*10 + 1; x++) {
    if (x < 1 || x > 9) {
      maze->getPixel(x,0) = black;
    }
  }

  for (int y = 0; y < height_*10 + 1; y++) {
    maze->getPixel(0,y) = black;
  }

  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      //int curr = x + y*height_;
      //vector<int> pathways = wallsRemoved.at(curr);
      //cout << "testing:" << x << " and " << y << endl;
      if (!canTravel(x,y, 0)) {
        for (int k = 0; k <= 10; k++) {
          maze->getPixel((x + 1) * 10, y*10 + k) = black;
        }
      }
      if (!canTravel(x, y, 1)) {
        for (int k =0; k <= 10; k++) {
          maze->getPixel(x*10 +k, (y + 1)*10) = black;
        }
      }
    }
  }
  return maze;
}

/*
This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze
to show the solution vector and the exit.

Start at pixel (5,5). Each direction in the solution vector corresponds to
a trail of 11 red pixels in the given direction. If the first step is downward,
color pixels (5,5) through (5,15) red. (Red is 0,1,0.5,1 in HSLA).
Then if the second step is right, color pixels (5,15) through (15,15) red.
Then if the third step is up, color pixels (15,15) through (15,5) red.
Continue in this manner until you get to the end of the solution vector,
so that your output looks analogous the above picture.

Make the exit by undoing the bottom wall of the destination square:
call the destination maze coordinates (x,y), and whiten the pixels
with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.

Returns
  a PNG of the solved SquareMaze
*/
PNG * SquareMaze::drawMazeWithSolution() {
  PNG* png = drawMaze();
  	std::vector<int> path = solveMaze();
  	HSLAPixel background(0, 1, 1);
  	HSLAPixel red(0, 1, 0.5);
  	int lastX = 5;
  	int lastY = 5;
  	int exitX = 0;
  	for (int dir : path) {
        int xOff = 0;
        int yOff = 0;
        if (dir == 0) {
            xOff = 1;
        }
        if (dir == 1) {
            yOff = 1;
        }
        if (dir == 2) {
            xOff = -1;
        }
        if (dir == 3) {
            yOff = -1;
        }
  		for (int step = 0; step <= 10; step++) {
  			png->getPixel(lastX + xOff * step, lastY + yOff * step) = red;
  		}
  		lastX += 10 * xOff;
  		lastY += 10 * yOff;
  		exitX += xOff;
  	}
  	for (int p = 1; p < 10; p++) png->getPixel(exitX * 10 + p, height_ * 10) = background;
  	return png;
}
