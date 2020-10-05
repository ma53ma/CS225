/**
 * @file NetworkFlow.cpp
 * CS 225: Data Structures
 */

#include <vector>
#include <algorithm>
#include <set>

#include "graph.h"
#include "edge.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  std::vector<Vertex> vertices = g_.getVertices();
    for (unsigned long i = 0; i < vertices.size(); i++) {
        residual_.insertVertex(vertices[i]);
        flow_.insertVertex(vertices[i]);
    }
    for (unsigned long i = 0; i < vertices.size() - 1; i++) {
        for (unsigned long j = i ; j < vertices.size(); j++) {
            if (g_.edgeExists(vertices[i], vertices[j])) {
                flow_.insertEdge(vertices[i], vertices[j]);
                flow_.setEdgeWeight(vertices[i], vertices[j], 0);

                residual_.insertEdge(vertices[i], vertices[j]);
                int w = g_.getEdgeWeight(vertices[i], vertices[j]);
                residual_.setEdgeWeight(vertices[i], vertices[j], w);

                residual_.insertEdge(vertices[j], vertices[i]);
                residual_.setEdgeWeight(vertices[j], vertices[i], 0);
            } else if (g_.edgeExists(vertices[j], vertices[i])) {
              flow_.insertEdge(vertices[j], vertices[i]);
              flow_.setEdgeWeight(vertices[j], vertices[i], 0);

              residual_.insertEdge(vertices[j], vertices[i]);
              int w = g_.getEdgeWeight(vertices[j], vertices[i]);
              residual_.setEdgeWeight(vertices[j], vertices[i], w);

              residual_.insertEdge(vertices[i], vertices[j]);
              residual_.setEdgeWeight(vertices[i], vertices[j], 0);
            }
        }
    }
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink,
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  int capacity = residual_.getEdgeWeight(path[0], path[1]);
  for (unsigned long i = 1; i < path.size() - 1; i++) {
      int curr = residual_.getEdgeWeight(path[i], path[i + 1]);
      if (curr < capacity) {
          capacity = curr;
      }
  }
  return capacity;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  std::vector<Vertex> path;
  while (findAugmentingPath(source_, sink_, path)) {
      int capacity = pathCapacity(path);
      for (unsigned long i = 0; i < path.size() - 1; i++) {
          Vertex s = path[i];
          Vertex d = path[i + 1];
          //update 1: FLOW GRAPH;
          //test if it flows forward
          if (flow_.edgeExists(s, d)) {
              int currweight = flow_.getEdgeWeight(s, d);
              flow_.setEdgeWeight(s, d, currweight + capacity);
          } else {
              //in this case, it should flow opposite
              //just to be safe, check if this edge exists
              if (!flow_.edgeExists(d, s)) {
                  //report error
                  cout<<"calculateFlow wrong"<<endl;
                  return flow_;
              }
              //otherwise, minue the capacity
              int currweight = flow_.getEdgeWeight(d, s);
              flow_.setEdgeWeight(d, s, currweight - capacity);
          }
          //update 2: residual graph: minus the capacity in the forward direction
          int currweight = residual_.getEdgeWeight(s, d);
          residual_.setEdgeWeight(s, d, currweight - capacity);
          //update 3: residual graph: add the capacity in the opposite direction
          int opp = residual_.getEdgeWeight(d, s);
          residual_.setEdgeWeight(d, s, opp + capacity);
      }
      path.clear();
  }
  //update maxFlow_
  std::vector<Vertex> v = flow_.getAdjacent(source_);
  // cout<<v.size()<<endl;
  maxFlow_ = 0;
  for (unsigned long i = 0; i <v.size(); i++) {
      maxFlow_ += flow_.getEdgeWeight(source_, v[i]);
  }
  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}
