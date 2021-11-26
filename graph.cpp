#include "graph.h"

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
            i->neighbours[position] = new vertex;
            i->neighbours[position]->x = valueX;
            i->neighbours[position]->y = valueY;
            i->neighbours[position]->neighbours[opposite(position)] = i;
            break;
        }
    }
}

void Graph::addEdge(int valueX1, int valueY1, int valueX2, int valueY2, int positionAtoB){
    auto a = find(valueX1, valueY1);
    auto b = find(valueX2, valueY2);
    a->neighbours[positionAtoB] = b;
    b->neighbours[opposite(positionAtoB)] = a;
}

bool Graph::isEmpty(){
    if(G.empty()){
        return true;
    } else {
        return false;
    }
}

Graph::vertex* Graph::find(int valueX, int valueY){
    for(auto &i : G){
        if(i->x == valueX && i->y == valueY){
            return i;
        }
    }
    return nullptr;
}
