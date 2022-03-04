#include "jointdisplay.h"
#include <iostream>
//Constructor

JointDisplay::JointDisplay(OpenGLContext* context)
    : Drawable(context), representedJoint(nullptr), selected(false)
{}

JointDisplay::~JointDisplay(){};

GLenum JointDisplay::drawMode() {
    return GL_LINES;
}

void JointDisplay::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> cols;
    std::vector<GLuint> idx;

    const int n = 15;
    float angle = 2 * M_PI / n;
    //n sides! - each circle will be n sided, and also, bear in mind, we must draw 3 different circles
    //let's initially begin with squares

    //For each of these points, what we are going to do is transform them by the rotation quaternion, and then add them to pos,
    //one circle at a time

    //y
    std::vector<glm::vec4> circle1Pos;
    //z
    std::vector<glm::vec4> circle2Pos;
    //x
    std::vector<glm::vec4> circle3Pos;

    for (int i = 0; i < n; i++) {
        float curAngle = angle * i;
        glm::vec3 curPos1 = 0.5f * glm::vec3(glm::cos(curAngle), 0, glm::sin(curAngle));
        circle1Pos.push_back(glm::vec4(curPos1, 1));

        glm::vec3 curPos2 = 0.5f * glm::vec3(glm::cos(curAngle), glm::sin(curAngle), 0);
        circle2Pos.push_back(glm::vec4(curPos2, 1));

        glm::vec3 curPos3 = 0.5f * glm::vec3(0, glm::cos(curAngle), glm::sin(curAngle));
        circle3Pos.push_back(glm::vec4(curPos3, 1));
    }

    //Step 2: Modify by the rotation quaternion of our represented Joint
    //Obtain rotation mat
//    glm::mat4 rotateMat = glm::toMat4(representedJoint->rotation);

    //
    glm::mat4 totalTransformation = representedJoint->getOverallTransformation();
    //Debugging tools
//    glm::quat exampleQuat = glm::quat(glm::vec3(90, 45, 0));
//    glm::quat exampleQuat = glm::angleAxis(glm::radians(90.f), glm::vec3(1, 0, 0));
//    glm::mat4 rotateMat = glm::toMat4(exampleQuat);

    //Step 3: Add all vertices of circle1Pos to pos (and set red color)

    for (int i = 0; i < circle1Pos.size(); i++) {
        glm::vec4 initialPos = circle1Pos[i];
        pos.push_back(totalTransformation * initialPos);
        if (selected) {
            cols.push_back(glm::vec4(0.9f, 1.f, 0.9f, 1.f));

        } else {
            cols.push_back(glm::vec4(0.f, 1.f, 0.f, 1.f));
        }
    }

    for (int i = 0; i < circle2Pos.size(); i++) {
        glm::vec4 initialPos = circle2Pos[i];
        pos.push_back(totalTransformation * initialPos);
        if (selected) {
            cols.push_back(glm::vec4(0.9f, 0.9f, 1.f, 1.f));
        } else {
            cols.push_back(glm::vec4(0.f, 0.f, 1.f, 1.f));
        }
    }

    for (int i = 0; i < circle3Pos.size(); i++) {
        glm::vec4 initialPos = circle3Pos[i];
        pos.push_back(totalTransformation * initialPos);
        if (selected) {
            cols.push_back(glm::vec4(1.f, 0.9f, 0.9f, 1.f));

        } else {
            cols.push_back(glm::vec4(1.f, 0.f, 0.f, 1.f));
        }
    }

    //Step 4: create index list
    int seenSoFar = 0;

    for (int i = 0; i < circle1Pos.size(); i++) {
        idx.push_back(seenSoFar + i % circle1Pos.size());
        idx.push_back(seenSoFar + (i + 1) % circle1Pos.size());
    }

    seenSoFar += circle1Pos.size();

    for (int i = 0; i < circle2Pos.size(); i++) {
        idx.push_back(seenSoFar + i % circle2Pos.size());
        idx.push_back(seenSoFar + (i + 1) % circle2Pos.size());
    }

    seenSoFar += circle2Pos.size();

    for (int i = 0; i < circle3Pos.size(); i++) {
        idx.push_back(seenSoFar + i % circle3Pos.size());
        idx.push_back(seenSoFar + (i + 1) % circle3Pos.size());
    }

    seenSoFar += circle3Pos.size();



    //Final part, drawing a line to all of the joints children! (purple to yellow)
    //seenSoFar, SeenSoFar + 1, SeenSoFar, SeenSoFar + 2 etc.

    //Add pos vector
    pos.push_back(totalTransformation * glm::vec4(0.f, 0.f, 0.f, 1.f));
    cols.push_back(glm::vec4(1.f, 0.f, 1.f, 1.f));

    //Add child pos vectors
    for (int i = 0; i < representedJoint->children.size(); i++) {
        Joint* curChild = representedJoint->children[i].get();
        pos.push_back(totalTransformation * curChild->relativePos);
        cols.push_back(glm::vec4(1.f, 1.f, 0.f, 1.f));
    }

    for (int i = seenSoFar + 1; i < pos.size(); i++) {
        idx.push_back(seenSoFar);
        idx.push_back(i);
    }

    //
    count = idx.size();

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
