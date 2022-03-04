#pragma once
#include <QString>
#include "smartpointerhelp.h"
#include "glm/glm.hpp"
#include <la.h>
#include <glm/gtc/quaternion.hpp>
#include <QTreeWidgetItem>


class Joint : public QTreeWidgetItem {
public:
    //fields
    //Name
    QString name;
    //Parent
    Joint* parent;
    //Children
    std::vector<uPtr<Joint>> children;
    //Position vec4
    glm::vec4 relativePos;
    //Rotation
    glm::quat rotation;
    //Bind Matrix
    glm::mat4 bindMatrix;

    //constructors
    //Default
    Joint();
    //With name
    Joint(std::string n);

    Joint(uPtr<Joint> &p);


    //methods
    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();

    Joint& addChild(uPtr<Joint> c);
    void setName(QString newName);

    glm::mat4 setBindMatrix();
};
