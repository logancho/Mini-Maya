#include "halfedge.h"

int HalfEdge::lastID = 0;

//Constructor

HalfEdge::HalfEdge(HalfEdge* n, HalfEdge* s, Face* f, Vertex* v)
    : next(n), sym(s), face(f), vertex(v), ID(++HalfEdge::lastID)
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

HalfEdge::HalfEdge()
    : next(nullptr), sym(nullptr), face(nullptr), vertex(nullptr), ID(HalfEdge::lastID++)
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

HalfEdge::HalfEdge(const HalfEdge &hE)
    : next(hE.next), sym(hE.sym), face(hE.face), vertex(hE.vertex), ID(hE.ID)
{
//    QTreeWidgetItem::setText(0, QString(QString::number(lastID - 1)));
    QListWidgetItem::setText(QString::number(this->ID));
}

//Destructor

HalfEdge::~HalfEdge(){};
