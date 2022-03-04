#include <joint.h>


Joint::Joint()
    : name(QString("defaultName")), parent(nullptr), children{}, relativePos(glm::vec4(0.f, 0.f, 0.f, 1.f)), rotation(glm::quat()), bindMatrix(glm::mat4())
{
    QTreeWidgetItem::setText(0, name);
}

Joint::Joint(std::string n)
    : name(QString::fromStdString(n)), parent(nullptr), children{}, relativePos(glm::vec4(0.f, 0.f, 0.f, 1.f)), rotation(glm::quat()), bindMatrix(glm::mat4())
{
    QTreeWidgetItem::setText(0, name);
}

//methods

glm::mat4 Joint::getLocalTransformation() {
    //Translate * Rotate
    return glm::translate(glm::mat4(), glm::vec3(relativePos)) * glm::toMat4(rotation);
}

glm::mat4 Joint::getOverallTransformation() {
    glm::mat4 T = getLocalTransformation();
    Joint* curJoint = this;
    //while curJoint has a parent
    while (curJoint->parent) {
        T = curJoint->parent->getLocalTransformation() * T;
        curJoint = curJoint->parent;
    }
    //this transformation should be done FIRST, so parents on the left, children on the right
    return T;
}

Joint& Joint::addChild(uPtr<Joint> c) {
    Joint* child = c.get();
    QTreeWidgetItem::addChild(child);

    child->parent = this;

    this->children.push_back(std::move(c));
//    return *(this->children.back());
    return *child;
}

void Joint::setName(QString newName) {
    this->name = newName;
    QTreeWidgetItem::setText(0, name);
}

glm::mat4 Joint::setBindMatrix() {
    bindMatrix = glm::inverse(getOverallTransformation());
    return bindMatrix;
}
