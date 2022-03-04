#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QVariantMap>
#include <map>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this), m_mesh(this),
      m_progLambert(this), m_progFlat(this), m_progSkeleton(this),
      m_glCamera(),
      mp_selectedVertex(nullptr), mp_selectedFace(nullptr),
      mp_selectedHalfEdge(nullptr), mp_selectedJoint(nullptr),
      v_highlight(this), he_highlight(this), f_highlight(this), rootJoint{},
      j_highlight(this),
      bMCount(0),
      jTCount(0)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
//    m_geomSquare.destroy();
    m_mesh.destroy();
    v_highlight.destroy();
    he_highlight.destroy();
    f_highlight.destroy();
    j_highlight.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
//    m_geomSquare.create();

    //MESH
    m_mesh.createCube();

//    ////Split Edge Debugging:
//    HalfEdge *he1 = m_mesh.halfEdges[0].get();
//    if (he1 != nullptr) {
//        m_mesh.splitEdge(he1);
//    }
//    ////

//    ////Triangulate Debugging:
//    Face *face1 = m_mesh.faces[0].get();
//    if (face1 != nullptr) {
//        m_mesh.triangulate(face1);
//    }
//    ////

    //Subdivide debugging
    //Centroid - Done
    //

//    m_mesh.subdivide();
//    m_mesh.subdivide();
//    m_mesh.subdivide();
//    m_mesh.subdivide();

    m_mesh.create();



    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    m_progSkeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);


    //Emit all signals to add each mesh component to QListWidgetItem
    sendMeshComponentSignals();


    //Skeleton Debug
//    //Set root joint as some random rootjoint
//    rootJoint = mkU<Joint> ();
//    //Emit root node signal
//    emit(sig_sendRootJoint(rootJoint.get()));

    //Works!!

    //Skeleton Debug:

    //Modify root position for fun

//    rootJoint->relativePos = glm::vec4(1, 2, 0, 1);
//    //Adding a child to root
//    uPtr<Joint> child1 = mkU<Joint> ();
//    Joint* child1Pointer = child1.get();
//    child1Pointer->relativePos = glm::vec4(5, 0, 0, 1);
//    rootJoint->addChild(std::move(child1));

    j_highlight.selected = true;

//    glm::mat4 bindMatrices;
//    int bMCount;
//    glm::mat4 jointTrans;
//    bindMatrices[0] = glm::mat4();
//    std::cout << bindMatrices[0][0][0] << std::endl;;
}




void MyGL::sendMeshComponentSignals() {
    for (int i = 0; i < m_mesh.vertices.size(); i++) {
        emit(sig_sendVertex(m_mesh.vertices[i].get()));
    }
    //Face:

    for (int i = 0; i < m_mesh.faces.size(); i++) {
        emit(sig_sendFace(m_mesh.faces[i].get()));
    }

    //HalfEdge:

    for (int i = 0; i < m_mesh.halfEdges.size(); i++) {
        emit(sig_sendHalfEdge(m_mesh.halfEdges[i].get()));
    }
}



void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSkeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
//    std::cout << "still working6\n \n";
//    std::cout << "calling paintGL\n";
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(glm::vec3(m_glCamera.eye));


    //m_progSkeleton
    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSkeleton.setCamPos(glm::vec3(m_glCamera.eye));

    //bM complete
    if (bMCount > 0) {
        m_progSkeleton.setBindMatrices(bindMatrices, bMCount);
    }
    //jT
    if (rootJoint) {
        jTCount = 0;
        jointTransformRecLoop(rootJoint.get());
        if (jTCount > 0) {
            m_progSkeleton.setJointTrans(jointTrans, jTCount);
        }
    }

    //Recurse through all of the joints, and for each, add


//    void ShaderProgram::setBindMatrices(const glm::mat4 bindMatrices [100], int bMCount) {
//    void ShaderProgram::setJointTrans(const glm::mat4 jointTrans [100], int jTCount) {


    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    //Send the geometry's transformation matrix to the shader
//    m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
//    m_progLambert.draw(m_geomSquare);
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,7)) * glm::rotate(glm::mat4(), 0.15f * 3.14159f, glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.f);
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,7));

//    m_progLambert.setModelMatrix(model);
//    m_progLambert.draw(m_mesh);
    m_progSkeleton.setModelMatrix(model);
    m_progSkeleton.draw(m_mesh);

//    m_progFlat.setModelMatrix(model);
//    m_progFlat.draw(m_mesh);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    m_progLambert.setModelMatrix(model);
//    m_progLambert.draw(m_geomSquare);

    //Highlight drawing:
    //1. Vertex
    if (v_highlight.representedVertex != nullptr) {
        v_highlight.destroy();
        v_highlight.create();
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(v_highlight);
        glEnable(GL_DEPTH_TEST);
    }
    //2. HalfEdge
    if (he_highlight.representedHalfEdge != nullptr) {
        he_highlight.destroy();
        he_highlight.create();
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(he_highlight);
        glEnable(GL_DEPTH_TEST);
    }
    //3. Face
    if (f_highlight.representedFace != nullptr) {
        f_highlight.destroy();
        f_highlight.create();
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(f_highlight);
        glEnable(GL_DEPTH_TEST);
    }


    //Wireframe Drawing

    //Debugging:

    //QTreeWidgetItem * QTreeWidget::topLevelItem(int index) const
    //Access the top level item in column 0 and use this to do a BFS traversal where a new JointDisplay is created for each of the
    //joints every frame/ every time frame is called
    if (rootJoint) {
        JointDisplay rootDisplay = JointDisplay(this);
        rootDisplay.representedJoint = rootJoint.get();

        /////////rootJoint->relativePos = glm::vec4(-1, 0, 0, 1);
//        std::cout << "root pos: (" << rootJoint.get()->relativePos.x << ", " << rootJoint.get()->relativePos.y << ", " << rootJoint.get()->relativePos.z << ")" << std::endl;

        rootDisplay.destroy();
        rootDisplay.create();
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(rootDisplay);
        glEnable(GL_DEPTH_TEST);

        //BFS for drawing all joints in the tree
        jointRecLoop(rootJoint.get());
    }

    //Joint highlight
    if (mp_selectedJoint != nullptr) {
        j_highlight.destroy();
        j_highlight.create();
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(j_highlight);
        glEnable(GL_DEPTH_TEST);
    }
//    j_highlight.destroy();
//    j_highlight.create();
//    glDisable(GL_DEPTH_TEST);
//    m_progFlat.draw(j_highlight);
//    glEnable(GL_DEPTH_TEST);
}


void MyGL::jointTransformRecLoop(Joint* curJoint) {
    jointTrans[jTCount] = curJoint->getOverallTransformation();
    for (int i = 0; i < curJoint->children.size(); i++) {
        jointTransformRecLoop(curJoint->children[i].get());
    }
}

void MyGL::jointRecLoop(Joint* curJoint) {
//    std::cout << "running the jointRecLoop\n";
    if (!curJoint->children.empty()) {
//        std::cout << "children vector not empty" << std::endl;
        //For each child
        for (int i = 0; i < curJoint->children.size(); i++) {
//            std::cout << "Creating new child jointDisplay" << std::endl;
            Joint* curChild = curJoint->children[i].get();
            /////////curChild->relativePos = glm::vec4(2, 0, 0, 1);
//            std::cout << "root pos: (" << curChild->relativePos.x << ", " << curChild->relativePos.y << ", " << curChild->relativePos.z << ")" << std::endl;

            JointDisplay childDisplay = JointDisplay(this);
            childDisplay.representedJoint = curChild;
            childDisplay.create();

            glDisable(GL_DEPTH_TEST);
            m_progFlat.draw(childDisplay);
            glEnable(GL_DEPTH_TEST);

            //Call on child
            jointRecLoop(curChild);
        }
    }
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    }


    //debugging:
//use updateFace, updateVertex, updateHalfEdge
    else if (e->key() == Qt::Key_N) {
        if (he_highlight.representedHalfEdge != nullptr) {
            he_highlight.updateHalfEdge(he_highlight.representedHalfEdge->next);
        }
    }

    else if (e->key() == Qt::Key_M) {
        if (he_highlight.representedHalfEdge != nullptr) {
            he_highlight.updateHalfEdge(he_highlight.representedHalfEdge->sym);
        }
    }

    else if (e->key() == Qt::Key_F) {
        if (he_highlight.representedHalfEdge != nullptr) {
            f_highlight.updateFace(he_highlight.representedHalfEdge->face);
            he_highlight.updateHalfEdge(nullptr);
        }
    }

    else if (e->key() == Qt::Key_V) {
        if (he_highlight.representedHalfEdge != nullptr) {
            v_highlight.updateVertex(he_highlight.representedHalfEdge->vertex);
            he_highlight.updateHalfEdge(nullptr);
        }
    }

    else if (e->key() == Qt::Key_H && QApplication::keyboardModifiers() && Qt::ShiftModifier) {
        if (f_highlight.representedFace != nullptr) {
            he_highlight.updateHalfEdge(f_highlight.representedFace->hEdge);
            f_highlight.updateFace(nullptr);
        }
    }

    else if (e->key() == Qt::Key_H) {
        if (v_highlight.representedVertex != nullptr) {
            he_highlight.updateHalfEdge(v_highlight.representedVertex->hEdge);
            v_highlight.updateVertex(nullptr);
        }
    }

    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

//Slots:

//select slots:

void MyGL::slot_setSelectedVertex(QListWidgetItem *i) {
//    std::cout << "Selected Vertex" << std::endl;
    mp_selectedVertex = static_cast<Vertex*>(i);
    mp_selectedFace = nullptr;
    mp_selectedHalfEdge = nullptr;

    v_highlight.updateVertex(mp_selectedVertex);
    he_highlight.updateHalfEdge(nullptr);
    f_highlight.updateFace(nullptr);
    update();
}

void MyGL::slot_setSelectedFace(QListWidgetItem *i) {
//    std::cout << "Selected Face" << std::endl;
    mp_selectedFace = static_cast<Face*>(i);
    mp_selectedHalfEdge = nullptr;
    mp_selectedVertex = nullptr;

    v_highlight.updateVertex(nullptr);
    he_highlight.updateHalfEdge(nullptr);
    f_highlight.updateFace(mp_selectedFace);
    update();
}

void MyGL::slot_setSelectedHalfEdge(QListWidgetItem *i) {
//    std::cout << "Selected HalfEdge" << std::endl;
    mp_selectedHalfEdge = static_cast<HalfEdge*>(i);
    mp_selectedFace = nullptr;
    mp_selectedVertex = nullptr;

    v_highlight.updateVertex(nullptr);
    he_highlight.updateHalfEdge(mp_selectedHalfEdge);
    f_highlight.updateFace(nullptr);
    update();
}

//Alter color

//Face color:

void MyGL::slot_alterFaceColorR(double d) {
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->color.r = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterFaceColorG(double d) {
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->color.g = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterFaceColorB(double d) {
    if (mp_selectedFace != nullptr) {
        mp_selectedFace->color.b = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterVertexPosX(double d) {
    if (mp_selectedVertex != nullptr) {
        mp_selectedVertex->pos.x = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterVertexPosY(double d) {
    if (mp_selectedVertex != nullptr) {
        mp_selectedVertex->pos.y = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterVertexPosZ(double d) {
    if (mp_selectedVertex != nullptr) {
        mp_selectedVertex->pos.z = d;
        //update vbo to glsl shader!
        m_mesh.destroy();
        m_mesh.create();
        //Call paintGl through update!
        update();
    }
}


//Split Edge:

void MyGL::slot_splitEdge(bool b) {
    if (mp_selectedHalfEdge != nullptr) {
//        std::cout << "splitEdge slot activated" << std::endl;
        m_mesh.splitEdge(mp_selectedHalfEdge);

        sendMeshComponentSignals();
        m_mesh.destroy();
        m_mesh.create();
        update();
    }
}

//Triangulate:

void MyGL::slot_triangulate(bool b) {
    if (mp_selectedFace != nullptr) {
//        std::cout << "triangulate slot activated" << std::endl;
        m_mesh.triangulate(mp_selectedFace);

        sendMeshComponentSignals();
        m_mesh.destroy();
        m_mesh.create();
        update();
    }
}

void MyGL::slot_subdivide(bool b) {
//    std::cout << "subdivide slot activated" << std::endl;
    m_mesh.subdivide();

    sendMeshComponentSignals();
    m_mesh.destroy();
    m_mesh.create();
    update();
}

void MyGL::slot_importOBJ(bool b) {
    //1. Obtain filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr("OBJs (*.obj)"));

    //2. Read file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    //Empty existing mesh:
    m_mesh.clearMesh();

    //Read obj file:
    while (!in.atEnd()) {
        QString line = in.readLine();
        //Process lines!
        QStringList list = line.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        std::vector<int> verticesInFace;

        //If non empty:
        if (list.size() > 0) {
            if (list.at(0) == 'v') {
                //Case 1: Vertex
//                vertices.push_back(glm::vec3(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat()));
                m_mesh.importVertexFromOBJ(glm::vec3(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat()));

            } else if (list.at(0) == 'f') {
                //Case 2: Face
                for (int i = 1; i < list.size(); i ++) {
//                    verticesInFace.push_back(std::stoi((list.at(i).mid(0, 1)).toStdString()) - 1);
                    //run split on list.at(i) to get rid of slashes
                    QStringList numberList = list.at(i).split(QLatin1Char('/'), Qt::SkipEmptyParts);
                    verticesInFace.push_back(std::stoi(numberList.at(0).toStdString()) - 1);

                }

                //verticesInFace is constructed now!
                m_mesh.constructFacesFromOBJ(verticesInFace);
                //Assumption 1: vertices is already constructed!
                //Assumption 2: 1-indexed, so make sure to minus 1
            }
        }
    }
    sendMeshComponentSignals();
    m_mesh.destroy();
    m_mesh.create();
    update();
}

//Joints
void MyGL::slot_setSelectedJoint(QTreeWidgetItem *i) {
    std::cout << "Selected Joint" << std::endl;
    mp_selectedJoint = static_cast<Joint*>(i);
    //Additionally, alter its colors!
    j_highlight.representedJoint = mp_selectedJoint;
    update();
}

void MyGL::slot_loadJSON(bool b) {

    //1. Obtain filename
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr("JSONs (*.json)"));

    //2. Read file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    QString jsonString = in.readAll();
    file.close();
//    std::cout << "Trying out json parser" << std::endl;

    QByteArray jsonBytes = jsonString.toLocal8Bit();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);

//    if (!jsonDoc.isNull()) {
//        std::cout << "JSON document is not null." << std::endl;
//    }

    QJsonObject jsonObj;

    if (jsonDoc.isObject()) {

        //Step 0: Update fields
        //0. Deselect joint
        mp_selectedJoint = nullptr;
        //1. Empty all joint influences + weights of all vertices
        for (int i = 0; i < m_mesh.vertices.size(); i++) {
            m_mesh.vertices[i].get()->joints.clear();
            m_mesh.vertices[i].get()->jointWeights.clear();
        }
        //2.
        bMCount = 0;
        jTCount = 0;

        jsonObj = jsonDoc.object();

        QVariantMap jsonMap = jsonObj.toVariantMap();
        //First, parse the root
        QMap jsonRootMap = jsonMap["root"].toMap();
        //Recursive joint building:
        //First child (which is our root)
        rootJoint = mkU<Joint> ();

        Joint* rJoint = rootJoint.get();
        //NAME
        rJoint->setName(jsonRootMap["name"].toString());

        //POS
        QVariantList posList = jsonRootMap["pos"].toList();
        rJoint->relativePos = glm::vec4(posList[0].toFloat(), posList[1].toFloat(), posList[2].toFloat(), 1.f);
//        std::cout << "root pos json reading: (" << posList[0].toFloat() << ", " << posList[1].toFloat() << ", " << posList[2].toFloat() << ")" << std::endl;

        //ROTATION
        //Compute quaternion
        QVariantList rotList = jsonRootMap["rot"].toList();
        rJoint->rotation = glm::angleAxis(glm::radians(rotList[0].toFloat()),
                glm::vec3(rotList[1].toFloat(), rotList[2].toFloat(), rotList[3].toFloat()));

        //CHILDREN
        QVariantList childrenList = jsonRootMap["children"].toList();

        for (int i = 0; i < childrenList.size(); i++) {
            QMap jsonChildMap = childrenList[i].toMap();
            //For this childVariant, we will:

            //1. Create a new joint
            uPtr<Joint> newChildHeap = mkU<Joint> ();
            Joint* newChild = newChildHeap.get();

            //2. Set its parent + add it to parent's children vector
            rJoint->addChild(std::move(newChildHeap));

            //3. Set its name
//            std::cout << "Setting name for: " << jsonChildMap["name"].toString().QString::toStdString() << std::endl;
            newChild->setName(jsonChildMap["name"].toString());

            //4. Set its pos
            QVariantList childPosList = jsonChildMap["pos"].toList();
            newChild->relativePos = glm::vec4(childPosList[0].toFloat(), childPosList[1].toFloat(), childPosList[2].toFloat(), 1.f);
//            std::cout << "root pos json reading: (" << childPosList[0].toFloat() << ", " << childPosList[1].toFloat() << ", " << childPosList[2].toFloat() << ")" << std::endl;

            //5. Set its rotation
            QVariantList childRotList = jsonChildMap["rot"].toList();
            newChild->rotation = glm::angleAxis(glm::radians(childRotList[0].toFloat()),
                    glm::vec3(childRotList[1].toFloat(), childRotList[2].toFloat(), childRotList[3].toFloat()));

            //6. Call the recursive function on its own children list
            jsonRecLoop(newChild, jsonChildMap["children"].toList());
        }

        //Finally, emit this root node to the mainwindow!
        emit(sig_sendRootJoint(rootJoint.get()));
        update();
    }
}

void MyGL::jsonRecLoop(Joint* parentJoint, QVariantList childrenList) {
    for (int i = 0; i < childrenList.size(); i++) {
        QMap jsonChildMap = childrenList[i].toMap();
        //For this childVariant, we will:

        //1. Create a new joint
        uPtr<Joint> newChildHeap = mkU<Joint> ();
        Joint* newChild = newChildHeap.get();

        //2. Set its parent + add it to parent's children vector
        parentJoint->addChild(std::move(newChildHeap));

        //3. Set its name
        newChild->setName(jsonChildMap["name"].toString());

        //4. Set its pos
        QVariantList childPosList = jsonChildMap["pos"].toList();
        newChild->relativePos = glm::vec4(childPosList[0].toFloat(), childPosList[1].toFloat(), childPosList[2].toFloat(), 1.f);
//            std::cout << "root pos json reading: (" << childPosList[0].toFloat() << ", " << childPosList[1].toFloat() << ", " << childPosList[2].toFloat() << ")" << std::endl;

        //5. Set its rotation
        QVariantList childRotList = jsonChildMap["rot"].toList();
        newChild->rotation = glm::angleAxis(glm::radians(childRotList[0].toFloat()),
                glm::vec3(childRotList[1].toFloat(), childRotList[2].toFloat(), childRotList[3].toFloat()));

        //6. Call the recursive function on its own children list
        jsonRecLoop(newChild, jsonChildMap["children"].toList());
    }
}

//If we're binding a joint for the first time
//run setBindMesh on the joint, and then add that joint's bindMatrix to bindmatrix!

void MyGL::slot_bindToMesh(bool b) {
    std::cout << "bind button clicked\n";

    //m_mesh.vertices
    //For every single vertex, iterate through every single joint
    //Keep track of the top n closest joints!

    if (rootJoint) {
        const int n = 2;
        int count;

        bool bindedJoints = false;
        //For every vertex:
        for (int i = 0; i < m_mesh.vertices.size(); i++) {
            Vertex* curV = m_mesh.vertices[i].get();
            std::map<std::pair<float, int>, Joint*> orderedJoints;

            //Recurse through every single joint

            Joint* curJoint = rootJoint.get();
            count = 0;

            bindRecLoop(glm::vec4(curV->pos, 1.f), curJoint, orderedJoints, count, bindedJoints);

            if (!bindedJoints) {
                bMCount = count;
                std::cout << "bMCount: " << bMCount << std::endl;
            }
            bindedJoints = true;
            //WEIGHTS:
            //Now, iterate through the top n of these <key, value> pairs, and of these, add them to the vertices
            //joints vector and also, using dist, compute a weight to add to weights

            std::map<std::pair<float, int>, Joint*>::iterator itr  = orderedJoints.begin();
            int j = 0;
            float totalDist = 0;

            while(itr != orderedJoints.end() && j < n) {
                totalDist += itr->first.first;

                itr++;
                j++;
            }

//            std::cout << "totalDist: " << totalDist << std::endl;
            if (totalDist != 0) {
                j = 0;
                itr = orderedJoints.begin();
                while(itr != orderedJoints.end() && j < n) {

                    float weight = 1 - (itr->first.first / totalDist);
//                    curV->joints.push_back(itr->second);
                    curV->joints.push_back(itr->first.second);
                    curV->jointWeights.push_back(weight);
//                    std::cout << "Added a joint + joint weight of: " << weight << " to curV's vectors\n";

                    itr++;
                    j++;
                }
            }
        }
    }
}

void MyGL::bindRecLoop(const glm::vec4 vPos, Joint* curJoint, std::map<std::pair<float, int>, Joint*> &orderedJoints, int &count, bool &bindedJoints) {

    //BindMat
    if (!bindedJoints) {
        bindMatrices[count] = curJoint->setBindMatrix();
//        std::cout << "binded this joint!!" << std::endl;
    }
    //Compute distance
    float dist = glm::length(vPos - curJoint->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
//    std::cout << "count: " << count << " and dist: " << dist << std::endl;
    //Add to map
    orderedJoints.insert({std::make_pair(dist, count), curJoint});
    count++;
    //Recurse on children
    for (int i = 0; i < curJoint->children.size(); i++) {
        bindRecLoop(vPos, curJoint->children[i].get(), orderedJoints, count, bindedJoints);
    }
}

//More slots
void MyGL::slot_rotateJointX1(bool) {
//    std::cout << "slot_rotateJointX1" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {

        glm::vec3 newAxis = glm::vec3(1, 0, 0);
        glm::vec3 axis = glm::vec3(newAxis.x,
                                   newAxis.y,
                                   newAxis.z);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

void MyGL::slot_rotateJointX2(bool) {
//    std::cout << "slot_rotateJointX2" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {
        glm::vec3 axis = glm::vec3(1, 0, 0);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(-5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

void MyGL::slot_rotateJointY1(bool) {
//    std::cout << "slot_rotateJointY1" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {

        glm::vec3 axis = glm::vec3(0, 1, 0);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

void MyGL::slot_rotateJointY2(bool) {
//    std::cout << "slot_rotateJointY2" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {
        glm::vec3 axis = glm::vec3(0, 1, 0);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(-5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

void MyGL::slot_rotateJointZ1(bool) {
//    std::cout << "slot_rotateJointY1" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {
        glm::vec3 axis = glm::vec3(0, 0, 1);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

void MyGL::slot_rotateJointZ2(bool) {
//    std::cout << "slot_rotateJointY1" << std::endl;

    //Create a new quat for the selected joint

    if (mp_selectedJoint != nullptr) {

        glm::vec3 axis = glm::vec3(0, 0, 1);
        mp_selectedJoint->rotation = glm::angleAxis(glm::radians(-5.f), axis) * mp_selectedJoint->rotation;
        update();
    }
}

//Alter translation

void MyGL::slot_alterJointPosX(double d) {
    if (mp_selectedJoint != nullptr) {
        mp_selectedJoint->relativePos.x = d;

        //Call paintGl through update!
        update();
    }
}

void MyGL::slot_alterJointPosY(double d) {
    if (mp_selectedJoint != nullptr) {
        mp_selectedJoint->relativePos.y = d;

        update();
    }
}

void MyGL::slot_alterJointPosZ(double d) {
    if (mp_selectedJoint != nullptr) {
        mp_selectedJoint->relativePos.z = d;

        update();
    }
}
