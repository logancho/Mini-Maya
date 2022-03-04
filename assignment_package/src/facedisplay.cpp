#include "facedisplay.h"
#include "vertex.h"
#include <iostream>
//Constructor

FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr)
{}

FaceDisplay::~FaceDisplay(){};


GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

void FaceDisplay::updateFace(Face *f) {
    representedFace = f;
}

//create

void FaceDisplay::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> cols;
    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;

    //1. Find bounding vertices

    Face *curFace = representedFace;
    HalfEdge *curE = curFace->hEdge;

    do {
        //1. Add the vertex curE points to pos
        Vertex *curV = curE->vertex;

        pos.push_back(glm::vec4(curV->pos, 1.f));

        //2. Move to the next halfEdge
        curE = curE->next;

        //3. Add color to cols for each vertex
        glm::vec3 oppositeColor = glm::vec3(1.f) - curFace->color;
        cols.push_back(glm::vec4(oppositeColor, 1.f));

    } while(curE->ID != (curFace)->hEdge->ID);

    for (int i = 0; i < pos.size(); i++) {
        idx.push_back(i % pos.size());
        idx.push_back((i + 1) % pos.size());
    }


    count = pos.size() * 2;



    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(glm::vec4), cols.data(), GL_STATIC_DRAW);
}


