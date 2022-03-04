#pragma once
#include "face.h"
#include <iostream>

int Face::lastID = 0;

//Constructor:
Face::Face(glm::vec3 c)
    : hEdge(nullptr), color(c), ID(Face::lastID++)
{
    QListWidgetItem::setText(QString::number(this->ID));
}

Face::Face()
    : hEdge(nullptr), color(glm::vec3(0.3, 0.3, 0.3)), ID(Face::lastID++)
{
    QListWidgetItem::setText(QString::number(this->ID));
}

Face::Face(const Face &f)
    : hEdge(f.hEdge), color(f.color), ID(f.ID)
{
    QListWidgetItem::setText(QString::number(this->ID));
}

//Destructor:
Face::~Face(){};
