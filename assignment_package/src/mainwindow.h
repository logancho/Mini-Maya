#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    //My Slots:

    //Adding mesh components to tree

    void slot_addVertexToListWidget(QListWidgetItem*);
    void slot_addFaceToListWidget(QListWidgetItem*);
    void slot_addHalfEdgeToListWidget(QListWidgetItem*);

    //Vertex and Face select (update spinBox)

    void slot_setSelectedVertex2(QListWidgetItem*);
    void slot_setSelectedFace2(QListWidgetItem*);

//    //import OBJ
//    void slot_importOBJ(bool);

    //Add joint root node
    void slot_addRootToJointTreeWidget(QTreeWidgetItem*);

    //Joint select (update spinBox)
    void slot_setSelectedJoint2(QTreeWidgetItem*);


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
