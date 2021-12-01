#include "graph.h"
#include <limits>
#include <math.h>


void Graph::addVertex(int valueX, int valueY){

    vertex* newVertex = new vertex;
    newVertex->x = valueX;
    newVertex->y = valueY;
    G.emplace_back(newVertex);

}

int opposite(int a){
    switch(a){
        case 0: return 4;
        case 1: return 5;
        case 2: return 6;
        case 3: return 7;
        case 4: return 0;
        case 5: return 1;
        case 6: return 2;
        case 7: return 3;
    }
}

void Graph::addVertexByParent(int parentX, int parentY, int valueX, int valueY, int position){
    for(auto &i : G){
        if(i->x == parentX && i->y == parentY){
            vertex* newVertex = new vertex;
            newVertex->x = valueX;
            newVertex->y = valueY;
            newVertex->neighbours[opposite(position)] = i;
            i->neighbours[position] = newVertex;
            G.emplace_back(newVertex);
            break;
        }
    }
}

void Graph::addEdge(int valueX1, int valueY1, int valueX2, int valueY2, int positionAtoB){
    vertex& a = find(valueX1, valueY1);
    vertex& b = find(valueX2, valueY2);
    a.neighbours[positionAtoB] = &b;
    b.neighbours[opposite(positionAtoB)] = &a;
}

bool Graph::isEmpty(){
    if(G.empty()){
        return true;
    } else {
        return false;
    }
}

int Graph::getSize(){
    return G.size();
}

int Graph::isNeighbours(int valueX, int valueY){
    auto v = find(valueX, valueY);
    int result{0};
    for(int i = 0; i < 8; i++){
        if(v.neighbours[i] != nullptr){
            result++;
        }
    }
    return result;
}

float Graph::pathLength(connect value){
    return (value.little + (value.big * sqrt(2)));
}

vector<Graph::vertex> Graph::deikstrasAlgo(int startX, int startY, int endX, int endY){

    vector<connect> distances;
    vector<bool> visited;
    vector<vertex> result = {};
    vertex currentVertex1, currentVertex2;
    int minIndex;
    float delta;
    connect minDistance;
    if (this->getSize() <= 1 ||  (startX == endX && startY == endY)
    || (this->isNeighbours(startX, startY) == 0)
    || (this->isNeighbours(endX, endY) == 0)) {

        return result;

    }

    for (int i = 0; i < getSize(); i++) {
        distances.push_back({numeric_limits<int>::max(), numeric_limits<int>::max()});

        visited.push_back(false);
    }
    int testD = posit(startX, startY);
    distances[testD] = {0, 0};
    int s;
    do {
        minIndex = INT_MAX;
        minDistance = {numeric_limits<int>::max(), numeric_limits<int>::max()};

        for (int i = 0; i < getSize(); i++) {
            if (!visited[i] && pathLength(distances[i]) < pathLength(minDistance)) {
                minDistance = distances[i];
                minIndex = i;
                s = i;
            }
        }

        if (minIndex < INT_MAX) {
            for (int i = 0; i < 8; i++) {
                if(G[s]->neighbours[i] == nullptr) continue;
                currentVertex1.x = G[s]->neighbours[i]->x;
                currentVertex1.y = G[s]->neighbours[i]->y;
                delta = pathLength(minDistance);
                if(i % 2 != 0){
                    delta += 1;
                } else {
                    delta += sqrt(2);
                }

                if (delta < pathLength(distances[posit(currentVertex1.x, currentVertex1.y)])) {
                    distances[posit(currentVertex1.x, currentVertex1.y)] = minDistance;
                    if(i % 2 != 0){
                        distances[posit(currentVertex1.x, currentVertex1.y)].little += 1;
                    } else {
                        distances[posit(currentVertex1.x, currentVertex1.y)].big += 1;
                    }
                }
            }
            visited[minIndex] = true;
        }
    } while (minIndex < INT_MAX);

    testD = posit(endX, endY);

    if (distances[posit(endX, endY)].little < INT_MAX || distances[posit(endX, endY)].big < INT_MAX) {

        currentVertex1.x = endX;
        currentVertex1.y = endY;
        result.push_back(currentVertex1);

        for (int i = 0; i < 8; i++) {

            if(currentVertex1.x == startX && currentVertex1.y == startY) break;
            if(find(currentVertex1.x, currentVertex1.y).neighbours[i] == nullptr) continue;

            currentVertex2.x = find(currentVertex1.x, currentVertex1.y).neighbours[i]->x;
            currentVertex2.y = find(currentVertex1.x, currentVertex1.y).neighbours[i]->y;

            delta = pathLength(distances[posit(currentVertex1.x, currentVertex1.y)]);
            if(i % 2 != 0){
                delta -= 1;
            } else {
                delta -= sqrt(2);
            }
            //auto testt =  pathLength(distances[posit(currentVertex2.x, currentVertex2.y)]);
            if (abs(delta - pathLength(distances[posit(currentVertex2.x, currentVertex2.y)])) <= 0.01) {
                result.emplace(result.begin(), currentVertex2);
                currentVertex1 = currentVertex2;
                i = -1;
            }
        }
    }
    return result;
}

int Graph::posit(int valueX, int valueY){
    for(auto i = 0; i < G.size(); i++){

        if(G[i]->x == valueX && G[i]->y == valueY){
            if(valueX == 4 && valueY == 4) {
                int ss = 0;
            }
            return i;
        }
    }
}

Graph::vertex& Graph::find(int valueX, int valueY){
    for(auto &i : G){
        if(i->x == valueX && i->y == valueY){
            return *i;
        }
    }
}




/*
vector<int> shortest_path(Graph &graph, int start_vertex, int end_vertex) {
    vector<float> distances;
    vector<bool> visited;
    vector<int> result = {};
    int currentVertex1, currentVertex2, minIndex;
    float minDistance, delta;

    if (graph.get_vertices().size() <= 1 || start_vertex == end_vertex
    || graph.get_adjacent_vertices(end_vertex).empty()
    || graph.get_adjacent_vertices(start_vertex).empty()) {

        return result;

    }

    for (int i = 0; i < graph.get_vertices().size(); i++) {
        distances.push_back(FLT_MAX);

        visited.push_back(false);
    }
    distances[start_vertex] = 0;
///
    do {
        minIndex = INT_MAX;
        minDistance = FLT_MAX;

        for (int i = 0; i < graph.get_vertices().size(); i++) {
            if (!visited[i] && distances[i] < minDistance) {
                minDistance = distances[i];
                minIndex = i;
            }
        }

        if (minIndex < INT_MAX) {
            for (int i = 0; i < graph.get_adjacent_vertices(minIndex).size(); i++) {
                currentVertex1 = graph.get_adjacent_vertices(minIndex)[i];
                delta = minDistance + graph.edge_weight(minIndex, currentVertex1);

                if (delta < distances[currentVertex1]) {
                    distances[currentVertex1] = delta;
                }
            }
            visited[minIndex] = true;
        }
    } while (minIndex < INT_MAX);

    if (distances[end_vertex] < FLT_MAX) {

        currentVertex1 = end_vertex;
        result.push_back(currentVertex1);

        for (int i = 0; i < graph.get_adjacent_vertices(currentVertex1).size(); i++) {

            if(currentVertex1 == start_vertex) break;

            currentVertex2 = graph.get_adjacent_vertices(currentVertex1)[i];
            delta = distances[currentVertex1] - graph.edge_weight(currentVertex1, currentVertex2);
            if (delta == distances[currentVertex2]) {
                result.emplace(result.begin(), currentVertex2);
                currentVertex1 = currentVertex2;
                i = -1;
            }
        }
    }
    return result;
}
*/
