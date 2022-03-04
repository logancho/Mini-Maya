#pragma once
#include "glm/glm.hpp"
#include "halfedge.h"
#include <QListWidgetItem>
#include "joint.h"

class HalfEdge;
class Vertex : public QListWidgetItem
{
public:
    //Fields
    glm::vec3 pos;
    HalfEdge* hEdge;
    int ID;
    static int lastID;

    //Joint Weights
//    std::vector<Joint*> joints;
    std::vector<int> joints;
    std::vector<float> jointWeights;

    //Constructor
    Vertex(glm::vec3 pVec, HalfEdge* hE);
    Vertex(glm::vec3 pVec);
    Vertex(const Vertex &v);
    Vertex();
    //Destructor
    ~Vertex();
    //Methods:
};

