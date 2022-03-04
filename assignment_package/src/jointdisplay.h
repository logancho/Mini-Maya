#pragma once
#include "drawable.h"
#include "joint.h"

class Joint;

class JointDisplay : public Drawable {
public:
    Joint* representedJoint;
    bool selected;

    JointDisplay(OpenGLContext* context);
    ~JointDisplay();

    void create() override;

    GLenum drawMode() override;

    void updateJoint(Joint*);
};
