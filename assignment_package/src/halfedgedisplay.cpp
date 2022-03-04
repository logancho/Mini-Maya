#include "halfedgedisplay.h"
#include "vertex.h"

//Constructor

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext* context)
    : Drawable(context), representedHalfEdge(nullptr)
{}

//Destructor
HalfEdgeDisplay::~HalfEdgeDisplay(){};

//drawMode

GLenum HalfEdgeDisplay::drawMode() {
    return GL_LINES;
}

//update

void HalfEdgeDisplay::updateHalfEdge(HalfEdge *hE) {
    representedHalfEdge = hE;
}

//create

void HalfEdgeDisplay::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> cols;
//    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;

    //red
    cols.push_back(glm::vec4(1, 0, 0, 1));
    //yellow
    cols.push_back(glm::vec4(1, 1, 0, 1));

    //1. Find bounding vertices

    HalfEdge *curE = representedHalfEdge;
    Vertex *curV;

    do {
        curV = curE->vertex;
        curE = curE->next;
    } while(curE->ID != representedHalfEdge->ID);

    pos.push_back(glm::vec4(curV->pos, 1.f));
    pos.push_back(glm::vec4(representedHalfEdge->vertex->pos, 1.f));

    idx.push_back(0);
    idx.push_back(1);
    count = 2;


    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

//    generateNor();
//    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
//    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(glm::vec4), cols.data(), GL_STATIC_DRAW);
}
