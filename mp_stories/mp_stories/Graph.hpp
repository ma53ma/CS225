#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"
#include <vector>

#include <string>
#include <iostream>

using std::cout;
using std::endl;

/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V,E>::numVertices() const {
  // TODO: Part 2
  return vertexMap.size();
}


/**
* The degree of the vertex. For directed: Sum of in-degree and out-degree
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V,E>::degree(const V & v) const {
  //find in vertexMap
  //map that string to adjList and return size of list
  // TODO: Part 2

  return (adjList.find(v.key())->second).size();
}


/**
* Inserts a Vertex into the Graph.
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V & Graph<V,E>::insertVertex(std::string key) {
  // add to vertexMap and adjList
  // TODO: Part 2
  V & v = *(new V(key));
  vertexMap.insert(std::pair<std::string, V&> (key, v));
  adjList[key];

  return v;
}


/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V,E>::removeVertex(const std::string & key) {
  // TODO: Part 2
  //remove from vertexMap and adjList, and remove all edges within the edgeList
  std::vector<edgeListIter> vect;
  for (edgeListIter it : adjList.at(key)) {
    vect.push_back(it);
  }

  for (size_t i = 0; i < vect.size(); i++) {
    removeEdge(vect[i]);
  }
  vertexMap.erase(key);

}


/**
* Inserts an Edge into the Graph.
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E & Graph<V,E>::insertEdge(const V & v1, const V & v2) {
  // TODO: Part 2
  E & e = *(new E(v1, v2));
  edgeList.push_front(e);
  adjList.at(v1.key()).push_front(edgeList.begin());
  adjList.at(v2.key()).push_front(edgeList.begin()); //concerned about source not being the thing here

  //insert at front of edge list
  //look up both vertcies and at at front of their adj lists


  return e;
}


/**
* Removes an Edge from the Graph. Consider both the undirected and directed cases.
* min(degree(key1), degree(key2))
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const std::string key1, const std::string key2) {
  // TODO: Part 2
  //look both vertices up in adj list and erase correct edges in adj list and in edge list
  for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
    if (it->get().source().key() == key1 && it->get().dest().key() == key2) {
      removeEdge(it);
      break;
    }
  }
}


/**
* Removes an Edge from the Graph given by the location of the given iterator into the edge list.
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const edgeListIter & it) {
  // TODO: Part 2
  for (auto listIt = adjList.at(it->get().source().key()).begin(); listIt != adjList.at(it->get().source().key()).end(); listIt++) {
    if ((*listIt == it)) {
      adjList.at(it->get().source().key()).erase(listIt);
      break; // check if this is valid?
    }
  }

  for (auto listIt = adjList.at(it->get().dest().key()).begin(); listIt != adjList.at(it->get().dest().key()).end(); listIt++) {
    if ((*listIt == it)) {
      adjList.at(it->get().dest().key()).erase(listIt);
      break;
    }
  }


  edgeList.erase(it);
}


/**
* Return the list of incident edges from a given vertex.
* For the directed case, consider all edges that has the vertex as either a source or destination.
* @param key The key of the given vertex
* @return The list edges (by reference) that are adjacent to the given vertex
*/
template <class V, class E>
const std::list<std::reference_wrapper<E>> Graph<V,E>::incidentEdges(const std::string key) const {
  // TODO: Part 2
  std::list<std::reference_wrapper<E>> edges;
  // RUN TIME
  //walk adj list of key to get them all
  std::list<edgeListIter> list = adjList.at(key);
  for (auto it = list.begin(); it != list.end(); it++) {
    edges.push_back(**it);
  }
  return edges;


}


/**
* Return whether the two vertices are adjacent to one another. Consider both the undirected and directed cases.
* When the graph is directed, v1 and v2 are only adjacent if there is an edge from v1 to v2.
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V,E>::isAdjacent(const std::string key1, const std::string key2) const {
  // TODO: Part 2
  //walk the smaller of the two adj lists and look for the other one
  // RUN TIME
  int deg1 = degree(key1);
  int deg2 = degree(key2);

  string bigKey;
  string smallKey;

  Edge e = *edgeList.begin();
  if (e.directed() == true) { // check this because it is probably wrong
    if (deg1 > deg2) {
      bigKey = key1;
      smallKey = key2;
    } else {
      bigKey = key2;
      smallKey = key1;
    }
  } else {
    //cout << "directed" << endl;
    smallKey = key1;
    bigKey = key2;
  }

  std::list<edgeListIter> list = adjList.at(smallKey);
  for (auto it = list.begin(); it != list.end(); it++) {
    //cout << smallKey << " and " << (*it)->get().dest() << endl;
    if ((*it)->get().dest() == bigKey) {
      return true;
    }
  }

  return false;
}
