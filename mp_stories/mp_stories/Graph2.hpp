#include <queue>
#include <algorithm>
#include <string>
#include <list>
#include <limits>
#include <vector>

inline size_t leftChild(size_t currentIdx) {
    return 2 * currentIdx;
}

inline size_t rightChild(size_t currentIdx) {
    return (2 * currentIdx) + 1;
}

inline size_t parent(size_t currentIdx) {
    return currentIdx / 2;
}

inline size_t maxPriorityChild(std::vector<std::pair<std::string, unsigned int>> & _elems, size_t currentIdx) {
  size_t left = leftChild(currentIdx) ; size_t right = rightChild(currentIdx);
  if (right >= _elems.size()) {
    return left;
  }

  return (_elems[left].second <= _elems[right].second) ? left : right;
}


inline bool hasAChild(std::vector<std::pair<std::string, unsigned int>> & _elems, size_t currentIdx) {
    return (leftChild(currentIdx) < _elems.size());
}

inline void heapifyUp(std::vector<std::pair<std::string, unsigned int>> & _elems, size_t currentIdx) {
  if (currentIdx != 1) {
    size_t parentIdx = parent(currentIdx);
    if (_elems[currentIdx].second <= _elems[parentIdx].second) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(_elems, parentIdx);
    }
    //cout <<"heaping up" << endl;
  }
}


inline void heapifyDown(std::vector<std::pair<std::string, unsigned int>> & _elems, size_t currentIdx) {
  if (hasAChild(_elems, currentIdx)) {
    size_t minChildIndex = maxPriorityChild(_elems, currentIdx);
    if (_elems[minChildIndex].second <= _elems[currentIdx].second) {
      std::swap(_elems[minChildIndex], _elems[currentIdx]);
      heapifyDown(_elems, minChildIndex);
    }
  }
}

inline void updateElem(std::vector<std::pair<std::string, unsigned int>> & _elems, size_t idx, unsigned int value) {
  _elems[idx].second = value;
  //cout << "updating " << _elems[idx].first << " with " << _elems[idx].second << endl;
  heapifyUp(_elems, idx);
  heapifyDown(_elems, idx);
  //cout << "reached " << endl;
  return;
}

inline bool empty(std::vector<std::pair<std::string, unsigned int>> & _elems) {
  return _elems.empty();
}

inline std::string pop(std::vector<std::pair<std::string, unsigned int>> & _elems) {
  if(empty(_elems)) {
    return "";
  }
  std::string minVal = _elems[1].first;
  _elems[1] = _elems[_elems.size() - 1];
  _elems.pop_back();
  heapifyDown(_elems, 1);
  return minVal;

}


/**
 * Returns an std::list of vertex keys that creates any shortest path between `start` and `end`.
 *
 * This list MUST include the key of the `start` vertex as the first vertex in the list, the key of
 * the `end` vertex as the last element in the list, and an ordered list of all vertices that must
 * be traveled along the shortest path.
 *
 * For example, the path a -> c -> e returns a list with three elements: "a", "c", "e".
 *
 * You should use undirected edges. Hint: There are no edge weights in the Graph.
 *
 * @param start The key for the starting vertex.
 * @param end   The key for the ending vertex.
 */


template <class V, class E>
std::list<std::string> Graph<V,E>::shortestPath(const std::string start, const std::string end) {
  // TODO: Part 3
  std::unordered_map<std::string, unsigned int> weight;
  std::unordered_map<std::string, std::string> pred;
  std::unordered_map<std::string, bool> visited;
  std::list<std::string> path;
  string curr = start;
  string source;
  string dest;
  std::vector<std::pair<std::string, unsigned int>> _elems;
  //std::vector<std::pair<std::string, unsigned int>> _elems;
  _elems.push_back(std::pair<std::string, unsigned int>("sentinel", std::numeric_limits<int>::max()));

  //std::vector<std::pair<std::string, unsigned int>> elem;



  //cout << "start is " << start << endl;
  //cout << "end is " << end << endl;

  for (auto it = vertexMap.begin(); it != vertexMap.end(); ++it) {
    if (!it->first.compare(start)) {
      weight[it->first] = 0;
      //cout << "weight of " << it->first << " is " << 0 << endl;

    } else {
      weight[it->first] = std::numeric_limits<int>::max();
      //cout << "weight of " << it->first << " is " << std::numeric_limits<int>::max() << endl;
    }
    pred[it->first] = "";
    visited[it->first] = false;
    _elems.push_back(std::pair<std::string, unsigned int>(it->first, std::numeric_limits<int>::max()));
  }

  // cout << weight.size() << endl;
  // cout << vertexMap.size() << endl;

  for (size_t i = 0; i < vertexMap.size(); i++) {
    for (edgeListIter e : adjList[curr]) {
      if ((*e).get().source().key() == curr) {
        source = (*e).get().source().key();
        dest = (*e).get().dest().key();
        //cout << curr << " with " << source << " and " << dest << endl;
      } else {
        source = (*e).get().dest().key();
        dest = (*e).get().source().key();
      }

      //cout << " source and dest are " << source << " and " << dest << endl;

      if (weight[dest] > weight[source] + 1) {
        //cout << "comp weights: " << weight[dest] << " and " << weight[source] + 1 << endl;
        weight[dest] = weight[source] + 1; // update weight and elems??
        for (size_t i = 1; i < _elems.size(); i++) {
          //cout << "checking strings of" << _elems[i].first << " and " << dest << endl;
          if (!_elems[i].first.compare(dest)) {
            //cout << "passing strings of" << _elems[i].first << " and " << dest << endl;

            updateElem(_elems, i, weight[source] + 1);
          }
        }
        auto it = pred.find(dest);
        //cout << "asdas" << endl;
        if (it != pred.end()) {
          it->second = source;
        }
      }
    }

    visited[curr] = true;

    if (!curr.compare(end)) {
      break;
    }
    while (visited.at(curr)) {
      curr = pop(_elems);
    }
    //cout << "new curr is" << curr << endl;
  }

  //cout << "predecessor of " << curr <<  " is " << pred.at(curr) << endl;
  while (curr != start) {
    path.emplace_front(curr);
    //cout << "adding " << curr << endl;
    curr = pred.at(curr);
  }
  path.emplace_front(start);
  return path;
}
