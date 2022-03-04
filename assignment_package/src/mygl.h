#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"
#include <QListWidgetItem>
#include "vertexdisplay.h"
#include "halfedgedisplay.h"
#include "facedisplay.h"
#include "joint.h"
#include "jointdisplay.h"
#include <glm/gtc/quaternion.hpp>
#include <QTreeWidgetItem>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

//class Mesh;
class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    Mesh m_mesh;
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.


    Camera m_glCamera;

    //Selections

    Vertex* mp_selectedVertex;
    Face* mp_selectedFace;
    HalfEdge* mp_selectedHalfEdge;
    Joint* mp_selectedJoint;

    //Highlights

    VertexDisplay v_highlight;
    HalfEdgeDisplay he_highlight;
    FaceDisplay f_highlight;

    //Root Joint
    uPtr<Joint> rootJoint;

    //JointDisplay Highlight
    JointDisplay j_highlight;

    //Shader
    glm::mat4 bindMatrices[100];
    int bMCount;
    glm::mat4 jointTrans[100];
    int jTCount;

public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void sendMeshComponentSignals();
    void jointRecLoop(Joint* curJ);
    void jsonRecLoop(Joint* parentJoint, QVariantList childrenList);

    void bindRecLoop(const glm::vec4 vPos, Joint* curJoint, std::map<std::pair<float, int>, Joint*> &closestNJoints, int &count, bool &bM);

    void jointTransformRecLoop(Joint* curJoint);

protected:
    void keyPressEvent(QKeyEvent *e);


signals:
        void sig_sendVertex(QListWidgetItem*);
        void sig_sendFace(QListWidgetItem*);
        void sig_sendHalfEdge(QListWidgetItem*);

        void sig_sendRootJoint(QTreeWidgetItem*);

public slots:
        //Select slots:
        void slot_setSelectedVertex(QListWidgetItem*);
        void slot_setSelectedFace(QListWidgetItem*);
        void slot_setSelectedHalfEdge(QListWidgetItem*);

        //Alter slots:

        //Color:
        void slot_alterFaceColorR(double d);
        void slot_alterFaceColorG(double d);
        void slot_alterFaceColorB(double d);

        //Vertex pos:
        void slot_alterVertexPosX(double d);
        void slot_alterVertexPosY(double d);
        void slot_alterVertexPosZ(double d);

        //Topology Edit

        void slot_splitEdge(bool);
        void slot_triangulate(bool);

        //Subdivide

        void slot_subdivide(bool);

        //import OBJ
        void slot_importOBJ(bool);

        //Joint
        void slot_setSelectedJoint(QTreeWidgetItem*);

        void slot_loadJSON(bool);

        void slot_bindToMesh(bool);

        //Rotation modifiers
        void slot_rotateJointX1(bool);
        void slot_rotateJointX2(bool);

        void slot_rotateJointY1(bool);
        void slot_rotateJointY2(bool);

        void slot_rotateJointZ1(bool);
        void slot_rotateJointZ2(bool);

        //Translation modifiers
        void slot_alterJointPosX(double d);
        void slot_alterJointPosY(double d);
        void slot_alterJointPosZ(double d);
};


#endif // MYGL_H
