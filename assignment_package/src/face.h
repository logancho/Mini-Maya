#pragma once
#include "glm/glm.hpp"
#include "halfedge.h"
#include <QListWidgetItem>

class HalfEdge;
class Face : public QListWidgetItem {
public:
    //Fields:
    HalfEdge* hEdge;
    glm::vec3 color;
    int ID;
    static int lastID;

    //Constructor
    Face(glm::vec3 c);
    Face(const Face &f);
    Face();
    ~Face();

    //Methods:
};
