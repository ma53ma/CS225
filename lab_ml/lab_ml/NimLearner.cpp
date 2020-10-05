/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "NimLearner.h"
#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    /* Your code goes here! */
    std::stringstream ss1;
    int tokens = (int) startingTokens;
    startingVertex_ = "p1-" + std::to_string(tokens);
    //cout << "putting start as " << startingVertex_ << endl;
    while (tokens >= 0) {
      ss1 << "p1-" << tokens;
      Vertex vertex1 = ss1.str();
      g_.insertVertex(vertex1);
      ss1.str("");
      ss1 << "p2-" << tokens;

      Vertex vertex2 = ss1.str();
      g_.insertVertex(vertex2);
      ss1.str("");
      tokens--;
    }

    tokens = (int) startingTokens;
    while (tokens >= 1) {
      int oneStep = tokens - 1;
      int twoStep = tokens - 2;
      ss1 << "p1-" << tokens;
      Vertex v1_1 = ss1.str();
      ss1.str("");
      ss1 << "p2-" << tokens;
      Vertex v2_1 = ss1.str();
      ss1.str("");
      ss1 << "p1-" << oneStep;
      Vertex v1_2 = ss1.str();
      ss1.str("");
      ss1 << "p1-" << twoStep;
      Vertex v1_3 = ss1.str();
      ss1.str("");
      ss1 << "p2-" << oneStep;
      Vertex v2_2 = ss1.str();
      ss1.str("");
      ss1 << "p2-" << twoStep;
      Vertex v2_3 = ss1.str();
      ss1.str("");
      if (oneStep >= 0){
        g_.insertEdge(v1_1, v2_2);
        g_.setEdgeWeight(v1_1, v2_2, 0);
        g_.insertEdge(v2_1, v1_2);
        g_.setEdgeWeight(v2_1, v1_2, 0);
      }
      if (twoStep >= 0) {
        g_.insertEdge(v1_1, v2_3);
        g_.setEdgeWeight(v1_1, v2_3, 0);
        g_.insertEdge(v2_1, v1_3);
        g_.setEdgeWeight(v2_1, v1_3, 0);

      }
      tokens--;
    }
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
  Vertex start = startingVertex_;
  vector<Vertex> adj = g_.getAdjacent(start);
  //cout << start << endl;
  while (adj.size() != 0) {
    int step = rand() % adj.size();
    path.push_back(g_.getEdge(start, adj[step]));
    start = adj[step];
    adj = g_.getAdjacent(start);
  }
 /* Your code goes here! */
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
  int index = path.size() - 1;
  int add = 1;
  while (index >= 0) {
    Edge edge = path[index];
    g_.setEdgeWeight(edge.source, edge.dest, edge.getWeight() + add);
    add = add * -1;
    index--;
  }
 /* Your code goes here! */
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
