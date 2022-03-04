#pragma once
#include "drawable.h"
#include "face.h"

class Face;

class FaceDisplay : public Drawable {

public:
    Face* representedFace;

    FaceDisplay(OpenGLContext* context);
    ~FaceDisplay();

    // Creates VBO data to make a visual
    // representation of the currently selected HalfEdge
    void create() override;

    //GL_LINES
    GLenum drawMode() override;

    // Change which Vertex representedVertex points to
    void updateFace(Face*);
};


