#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <iostream>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();


    //Connectors:

    //AddVertex
    connect(ui->mygl,
            // Signal name
            SIGNAL(sig_sendVertex(QListWidgetItem*)),
            // Widget with the slot that receives the signal
            this,
            // Slot name
            SLOT(slot_addVertexToListWidget(QListWidgetItem*)));
    //AddFace
    connect(ui->mygl,
            // Signal name
            SIGNAL(sig_sendFace(QListWidgetItem*)),
            // Widget with the slot that receives the signal
            this,
            // Slot name
            SLOT(slot_addFaceToListWidget(QListWidgetItem*)));

    //AddHalfEdge

    connect(ui->mygl,
            // Signal name
            SIGNAL(sig_sendHalfEdge(QListWidgetItem*)),
            // Widget with the slot that receives the signal
            this,
            // Slot name
            SLOT(slot_addHalfEdgeToListWidget(QListWidgetItem*)));

    //Selection


    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_setSelectedVertex(QListWidgetItem*)));

    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_setSelectedFace(QListWidgetItem*)));

    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_setSelectedHalfEdge(QListWidgetItem*)));

    //Part2:
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slot_setSelectedVertex2(QListWidgetItem*)));

    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slot_setSelectedFace2(QListWidgetItem*)));

    //Spinboxes
    //Face color

    connect(ui->faceRedSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterFaceColorR(double)));

    connect(ui->faceGreenSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterFaceColorG(double)));

    connect(ui->faceBlueSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterFaceColorB(double)));

    //Vertex Pos

    connect(ui->vertPosXSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterVertexPosX(double)));

    connect(ui->vertPosYSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterVertexPosY(double)));

    connect(ui->vertPosZSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterVertexPosZ(double)));

    //Topology buttons:

    connect(ui->splitEdgePushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_splitEdge(bool)));

    connect(ui->triangulatePushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_triangulate(bool)));

    //subdivide
    connect(ui->subdividePushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_subdivide(bool)));

    //import
    connect(ui->importOBJButton,
            // Signal name
            SIGNAL(clicked(bool)),
            // Widget with the slot that receives the signal
            ui->mygl,
            // Slot name
            SLOT(slot_importOBJ(bool)));

    //joint/skeleton
    //Add root node
    connect(ui->mygl,
            // Signal name
            SIGNAL(sig_sendRootJoint(QTreeWidgetItem*)),
            // Widget with the slot that receives the signal
            this,
            // Slot name
            SLOT(slot_addRootToJointTreeWidget(QTreeWidgetItem*)));

    //joint selection
    connect(ui->jointTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            ui->mygl, SLOT(slot_setSelectedJoint(QTreeWidgetItem*)));

    //Load JSON push_button
    connect(ui->loadJSONPushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_loadJSON(bool)));

    connect(ui->bindPushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_bindToMesh(bool)));

    //Rotation

    connect(ui->jointX1PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointX1(bool)));

    connect(ui->jointX2PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointX2(bool)));

    connect(ui->jointY1PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointY1(bool)));

    connect(ui->jointY2PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointY2(bool)));

    connect(ui->jointZ1PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointZ1(bool)));

    connect(ui->jointZ2PushButton,
            SIGNAL(clicked(bool)),
            ui->mygl,
            SLOT(slot_rotateJointZ2(bool)));

    //Spinbox feedback
    connect(ui->jointTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(slot_setSelectedJoint2(QTreeWidgetItem*)));

    //Spinbox alter
    connect(ui->jointPosXSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterJointPosX(double)));

    connect(ui->jointPosYSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterJointPosY(double)));

    connect(ui->jointPosZSpinBox,
            SIGNAL(valueChanged(double)),
            ui->mygl,
            SLOT(slot_alterJointPosZ(double)));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_addVertexToListWidget(QListWidgetItem *i) {
    ui->vertsListWidget->addItem(i);
}

void MainWindow::slot_addFaceToListWidget(QListWidgetItem *i) {
    ui->facesListWidget->addItem(i);
}

void MainWindow::slot_addHalfEdgeToListWidget(QListWidgetItem *i) {
    ui->halfEdgesListWidget->addItem(i);
}

void MainWindow::slot_setSelectedVertex2(QListWidgetItem *i) {
    Vertex* v = static_cast<Vertex*>(i);
    ui->vertPosXSpinBox->setValue(v->pos.x);
    ui->vertPosYSpinBox->setValue(v->pos.y);
    ui->vertPosZSpinBox->setValue(v->pos.z);
}

void MainWindow::slot_setSelectedFace2(QListWidgetItem *i) {
    Face* f = static_cast<Face*>(i);
    ui->faceRedSpinBox->setValue(f->color.r);
    ui->faceGreenSpinBox->setValue(f->color.g);
    ui->faceBlueSpinBox->setValue(f->color.b);
}

void MainWindow::slot_addRootToJointTreeWidget(QTreeWidgetItem *i) {
//    void MainWindow::slot_addRootToTreeWidget(QTreeWidgetItem *i) {
//    ui->jointTreeWidget->addTopLevelItem(i);
    ui->jointTreeWidget->addTopLevelItem(i);
}

void MainWindow::slot_setSelectedJoint2(QTreeWidgetItem *i) {
    Joint* j = static_cast<Joint*>(i);
//    ui->jointPosXSpinBox->setValue((j->getOverallTransformation() * glm::vec4(0, 0, 0, 1)).x);
//    ui->jointPosYSpinBox->setValue((j->getOverallTransformation() * glm::vec4(0, 0, 0, 1)).y);
//    ui->jointPosZSpinBox->setValue((j->getOverallTransformation() * glm::vec4(0, 0, 0, 1)).z);
    ui->jointPosXSpinBox->setValue(j->relativePos.x);
    ui->jointPosYSpinBox->setValue(j->relativePos.y);
    ui->jointPosZSpinBox->setValue(j->relativePos.z);
}
