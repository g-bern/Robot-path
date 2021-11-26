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
    
    struct vertex{
        int x;
        int y;
        vertex* neighbours[8] = {nullptr};
    };

    vector<vertex*> G;

public:
    Graph() {}
    
    void addVertex(int valueX, int valueY);

    void addVertexByParent(int parentX, int parentY, int valueX, int valueY, int position);

    const vertex* find(int valueX, int valueY);
};
#endif // GRAPH_H
