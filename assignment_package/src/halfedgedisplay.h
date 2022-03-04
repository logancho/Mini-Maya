#pragma once
#include "drawable.h"
#include "halfedge.h"

class HalfEdge;
class HalfEdgeDisplay : public Drawable {
public:
    HalfEdge* representedHalfEdge;

    HalfEdgeDisplay(OpenGLContext* context);
    ~HalfEdgeDisplay();

    // Creates VBO data to make a visual
    // representation of the currently selected HalfEdge
    void create() override;

    //GL_POINTS
    GLenum drawMode() override;

    // Change which Vertex representedVertex points to
    void updateHalfEdge(HalfEdge*);
};
