#pragma once

#ifndef GRAPH_H
#define GRAPH_H


#include <map>
#include <vector>
#include <tuple>
#include <initializer_list>

using namespace std;

class Graph {
private:
    

    struct connect{
        int little;
        int big;
    };



public:

    struct vertex{
        int x;
        int y;
        vertex* neighbours[8]{};
    };

    Graph() {}
    
    void addVertex(int valueX, int valueY);

    void addVertexByParent(int parentX, int parentY, int valueX, int valueY, int position);

    void addEdge(int valueX1, int valueY1, int valueX2, int valueY2, int positionAtoB);

    bool isEmpty();

    int getSize();

    int isNeighbours(int valueX, int valueY);

    vector<vertex> deikstrasAlgo(int startX, int startY, int endX, int endY);

    int posit(int valueX, int valueY);

    float pathLength(connect value);

    vertex& find(int valueX, int valueY);

private:
    vector<vertex*> G;
};
#endif // GRAPH_H
