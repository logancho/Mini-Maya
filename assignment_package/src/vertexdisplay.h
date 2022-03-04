#pragma once
#include "drawable.h"
#include "vertex.h"

class Vertex;
class VertexDisplay : public Drawable {
public:
    Vertex *representedVertex;

    VertexDisplay(OpenGLContext* context);
    ~VertexDisplay();

    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;

    //GL_POINTS
    GLenum drawMode() override;

    // Change which Vertex representedVertex points to
    void updateVertex(Vertex*);
};
