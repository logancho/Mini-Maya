#include "vertexdisplay.h"

//Constructor

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), representedVertex(nullptr)
{}

//Destructor
VertexDisplay::~VertexDisplay(){};

//drawMode

GLenum VertexDisplay::drawMode() {
    return GL_POINTS;
}

//create
void VertexDisplay::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> cols;
//    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;

    pos.push_back(glm::vec4(representedVertex->pos, 1.f));
    cols.push_back(glm::vec4(1.f, 1.f, 1.f, 1.f));
    idx.push_back(0);
    count = 1;

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
//

void VertexDisplay::updateVertex(Vertex *v) {
    this->representedVertex = v;
}
