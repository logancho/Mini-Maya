#pragma once
#include "glm/glm.hpp"
#include <QListWidgetItem>

class Face;
class Vertex;
class HalfEdge : public QListWidgetItem
{
public:
    //Fields
    HalfEdge* next;
    HalfEdge* sym;
    Face* face;
    Vertex* vertex;
    int ID;
    static int lastID;

    //Constructor:
    HalfEdge(HalfEdge* n, HalfEdge* s, Face* f, Vertex* v);
    HalfEdge(const HalfEdge &hE);
    HalfEdge();
    //Destructor:
    ~HalfEdge();

    //Methods
};
