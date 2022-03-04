/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    MyGL *mygl;
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *vertPosXSpinBox;
    QDoubleSpinBox *vertPosYSpinBox;
    QDoubleSpinBox *vertPosZSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceBlueSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *faceGreenSpinBox;
    QDoubleSpinBox *faceRedSpinBox;
    QLabel *label_11;
    QPushButton *splitEdgePushButton;
    QPushButton *triangulatePushButton;
    QPushButton *subdividePushButton;
    QPushButton *importOBJButton;
    QTreeWidget *jointTreeWidget;
    QPushButton *loadJSONPushButton;
    QPushButton *bindPushButton;
    QPushButton *jointX1PushButton;
    QPushButton *jointX2PushButton;
    QPushButton *jointY1PushButton;
    QPushButton *jointY2PushButton;
    QPushButton *jointZ1PushButton;
    QPushButton *jointZ2PushButton;
    QDoubleSpinBox *jointPosYSpinBox;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QDoubleSpinBox *jointPosXSpinBox;
    QDoubleSpinBox *jointPosZSpinBox;
    QLabel *label_16;
    QLabel *label_17;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1371, 578);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QString::fromUtf8("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QString::fromUtf8("mygl"));
        mygl->setGeometry(QRect(11, 11, 618, 432));
        vertsListWidget = new QListWidget(centralWidget);
        vertsListWidget->setObjectName(QString::fromUtf8("vertsListWidget"));
        vertsListWidget->setGeometry(QRect(640, 10, 111, 261));
        halfEdgesListWidget = new QListWidget(centralWidget);
        halfEdgesListWidget->setObjectName(QString::fromUtf8("halfEdgesListWidget"));
        halfEdgesListWidget->setGeometry(QRect(770, 10, 111, 261));
        facesListWidget = new QListWidget(centralWidget);
        facesListWidget->setObjectName(QString::fromUtf8("facesListWidget"));
        facesListWidget->setGeometry(QRect(900, 10, 111, 261));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(640, 280, 111, 16));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(770, 280, 111, 16));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(900, 280, 111, 16));
        label_3->setAlignment(Qt::AlignCenter);
        vertPosXSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosXSpinBox->setObjectName(QString::fromUtf8("vertPosXSpinBox"));
        vertPosXSpinBox->setGeometry(QRect(720, 320, 62, 22));
        vertPosXSpinBox->setMinimum(-99.989999999999995);
        vertPosXSpinBox->setSingleStep(0.010000000000000);
        vertPosYSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosYSpinBox->setObjectName(QString::fromUtf8("vertPosYSpinBox"));
        vertPosYSpinBox->setGeometry(QRect(800, 320, 62, 22));
        vertPosYSpinBox->setMinimum(-99.989999999999995);
        vertPosYSpinBox->setSingleStep(0.010000000000000);
        vertPosZSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosZSpinBox->setObjectName(QString::fromUtf8("vertPosZSpinBox"));
        vertPosZSpinBox->setGeometry(QRect(880, 320, 62, 22));
        vertPosZSpinBox->setMinimum(-99.989999999999995);
        vertPosZSpinBox->setSingleStep(0.010000000000000);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(640, 320, 71, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(720, 340, 61, 16));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(800, 340, 61, 16));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(880, 340, 61, 16));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(880, 390, 61, 16));
        label_8->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(720, 390, 61, 16));
        label_9->setAlignment(Qt::AlignCenter);
        faceBlueSpinBox = new QDoubleSpinBox(centralWidget);
        faceBlueSpinBox->setObjectName(QString::fromUtf8("faceBlueSpinBox"));
        faceBlueSpinBox->setGeometry(QRect(880, 370, 62, 22));
        faceBlueSpinBox->setMaximum(1.000000000000000);
        faceBlueSpinBox->setSingleStep(0.050000000000000);
        faceBlueSpinBox->setValue(1.000000000000000);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(640, 370, 71, 21));
        faceGreenSpinBox = new QDoubleSpinBox(centralWidget);
        faceGreenSpinBox->setObjectName(QString::fromUtf8("faceGreenSpinBox"));
        faceGreenSpinBox->setGeometry(QRect(800, 370, 62, 22));
        faceGreenSpinBox->setMaximum(1.000000000000000);
        faceGreenSpinBox->setSingleStep(0.050000000000000);
        faceGreenSpinBox->setValue(1.000000000000000);
        faceRedSpinBox = new QDoubleSpinBox(centralWidget);
        faceRedSpinBox->setObjectName(QString::fromUtf8("faceRedSpinBox"));
        faceRedSpinBox->setGeometry(QRect(720, 370, 62, 22));
        faceRedSpinBox->setMaximum(1.000000000000000);
        faceRedSpinBox->setSingleStep(0.050000000000000);
        faceRedSpinBox->setValue(1.000000000000000);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(800, 390, 61, 16));
        label_11->setAlignment(Qt::AlignCenter);
        splitEdgePushButton = new QPushButton(centralWidget);
        splitEdgePushButton->setObjectName(QString::fromUtf8("splitEdgePushButton"));
        splitEdgePushButton->setGeometry(QRect(650, 420, 113, 32));
        triangulatePushButton = new QPushButton(centralWidget);
        triangulatePushButton->setObjectName(QString::fromUtf8("triangulatePushButton"));
        triangulatePushButton->setGeometry(QRect(770, 420, 113, 32));
        subdividePushButton = new QPushButton(centralWidget);
        subdividePushButton->setObjectName(QString::fromUtf8("subdividePushButton"));
        subdividePushButton->setGeometry(QRect(650, 460, 113, 32));
        importOBJButton = new QPushButton(centralWidget);
        importOBJButton->setObjectName(QString::fromUtf8("importOBJButton"));
        importOBJButton->setGeometry(QRect(770, 460, 113, 32));
        jointTreeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        jointTreeWidget->setHeaderItem(__qtreewidgetitem);
        jointTreeWidget->setObjectName(QString::fromUtf8("jointTreeWidget"));
        jointTreeWidget->setGeometry(QRect(1090, 10, 161, 261));
        loadJSONPushButton = new QPushButton(centralWidget);
        loadJSONPushButton->setObjectName(QString::fromUtf8("loadJSONPushButton"));
        loadJSONPushButton->setGeometry(QRect(890, 460, 113, 32));
        bindPushButton = new QPushButton(centralWidget);
        bindPushButton->setObjectName(QString::fromUtf8("bindPushButton"));
        bindPushButton->setGeometry(QRect(890, 420, 111, 32));
        jointX1PushButton = new QPushButton(centralWidget);
        jointX1PushButton->setObjectName(QString::fromUtf8("jointX1PushButton"));
        jointX1PushButton->setGeometry(QRect(1020, 330, 101, 31));
        jointX2PushButton = new QPushButton(centralWidget);
        jointX2PushButton->setObjectName(QString::fromUtf8("jointX2PushButton"));
        jointX2PushButton->setGeometry(QRect(1020, 360, 101, 31));
        jointY1PushButton = new QPushButton(centralWidget);
        jointY1PushButton->setObjectName(QString::fromUtf8("jointY1PushButton"));
        jointY1PushButton->setGeometry(QRect(1120, 330, 113, 32));
        jointY2PushButton = new QPushButton(centralWidget);
        jointY2PushButton->setObjectName(QString::fromUtf8("jointY2PushButton"));
        jointY2PushButton->setGeometry(QRect(1120, 360, 113, 32));
        jointZ1PushButton = new QPushButton(centralWidget);
        jointZ1PushButton->setObjectName(QString::fromUtf8("jointZ1PushButton"));
        jointZ1PushButton->setGeometry(QRect(1230, 330, 113, 32));
        jointZ2PushButton = new QPushButton(centralWidget);
        jointZ2PushButton->setObjectName(QString::fromUtf8("jointZ2PushButton"));
        jointZ2PushButton->setGeometry(QRect(1230, 360, 113, 32));
        jointPosYSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosYSpinBox->setObjectName(QString::fromUtf8("jointPosYSpinBox"));
        jointPosYSpinBox->setGeometry(QRect(1150, 420, 62, 22));
        jointPosYSpinBox->setMinimum(-99.989999999999995);
        jointPosYSpinBox->setSingleStep(0.050000000000000);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(1070, 440, 61, 16));
        label_12->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(1150, 440, 61, 16));
        label_13->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(1110, 400, 141, 21));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(1230, 440, 61, 16));
        label_15->setAlignment(Qt::AlignCenter);
        jointPosXSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosXSpinBox->setObjectName(QString::fromUtf8("jointPosXSpinBox"));
        jointPosXSpinBox->setGeometry(QRect(1070, 420, 62, 22));
        jointPosXSpinBox->setMinimum(-99.989999999999995);
        jointPosXSpinBox->setSingleStep(0.050000000000000);
        jointPosZSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosZSpinBox->setObjectName(QString::fromUtf8("jointPosZSpinBox"));
        jointPosZSpinBox->setGeometry(QRect(1230, 420, 62, 22));
        jointPosZSpinBox->setMinimum(-99.989999999999995);
        jointPosZSpinBox->setSingleStep(0.050000000000000);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(1150, 280, 41, 21));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(1130, 310, 91, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1371, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "OpenGLDemo", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCamera_Controls->setText(QCoreApplication::translate("MainWindow", "Camera Controls", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Vertices", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Half-Edges", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Faces", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Vertex Position", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Blue", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Red", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Face Color", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Green", nullptr));
        splitEdgePushButton->setText(QCoreApplication::translate("MainWindow", "Split Edge", nullptr));
        triangulatePushButton->setText(QCoreApplication::translate("MainWindow", "Triangulate", nullptr));
        subdividePushButton->setText(QCoreApplication::translate("MainWindow", "Subdivide", nullptr));
        importOBJButton->setText(QCoreApplication::translate("MainWindow", "Import OBJ", nullptr));
        loadJSONPushButton->setText(QCoreApplication::translate("MainWindow", "Load JSON", nullptr));
        bindPushButton->setText(QCoreApplication::translate("MainWindow", "Bind skeleton", nullptr));
        jointX1PushButton->setText(QCoreApplication::translate("MainWindow", "Joint X +", nullptr));
        jointX2PushButton->setText(QCoreApplication::translate("MainWindow", "Joint X -", nullptr));
        jointY1PushButton->setText(QCoreApplication::translate("MainWindow", "Joint Y +", nullptr));
        jointY2PushButton->setText(QCoreApplication::translate("MainWindow", "Joint Y -", nullptr));
        jointZ1PushButton->setText(QCoreApplication::translate("MainWindow", "Joint Z +", nullptr));
        jointZ2PushButton->setText(QCoreApplication::translate("MainWindow", "Joint Z -", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Joint Relative Position", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Joints", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Joint Rotation", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
