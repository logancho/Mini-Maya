#include "vertex.h"

int Vertex::lastID = 0;

//Constructor
//std::vector<Joint*> joints;
//std::vector<float> jointWeights;

Vertex::Vertex(glm::vec3 pVec, HalfEdge* hE)
    : pos(pVec), hEdge(hE), ID(Vertex::lastID++), joints{}, jointWeights{}
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

Vertex::Vertex(glm::vec3 pVec)
    : pos(pVec), hEdge(nullptr), ID(Vertex::lastID++), joints{}, jointWeights{}
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

Vertex::Vertex()
    : pos(glm::vec3(0, 0, 0)), hEdge(nullptr), ID(Vertex::lastID++), joints{}, jointWeights{}
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

Vertex::Vertex(const Vertex &v)
    : pos(v.pos), hEdge(v.hEdge), ID(v.ID), joints{}, jointWeights{}
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

//Destructor
Vertex::~Vertex(){};

