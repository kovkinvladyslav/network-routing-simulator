/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionClear;
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionAddNode;
    QAction *actionAddConnection;
    QAction *actionDelete_Node;
    QAction *actionDelete_Node_2;
    QAction *actionDelete_Connection;
    QAction *actionShow_All;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuNode;
    QMenu *menuAdd;
    QMenu *menuDelete;
    QToolBar *toolBar;
    QDockWidget *dockWidget;
    QWidget *nodeInspectorDock;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QRadioButton *NodeON;
    QRadioButton *NodeOFF;
    QTableView *tableView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName("actionClear");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName("actionZoom_In");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
        actionZoom_In->setIcon(icon);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName("actionZoom_Out");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
        actionZoom_Out->setIcon(icon1);
        actionAddNode = new QAction(MainWindow);
        actionAddNode->setObjectName("actionAddNode");
        actionAddConnection = new QAction(MainWindow);
        actionAddConnection->setObjectName("actionAddConnection");
        actionDelete_Node = new QAction(MainWindow);
        actionDelete_Node->setObjectName("actionDelete_Node");
        actionDelete_Node_2 = new QAction(MainWindow);
        actionDelete_Node_2->setObjectName("actionDelete_Node_2");
        actionDelete_Connection = new QAction(MainWindow);
        actionDelete_Connection->setObjectName("actionDelete_Connection");
        actionShow_All = new QAction(MainWindow);
        actionShow_All->setObjectName("actionShow_All");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setMouseTracking(false);
        graphicsView->setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::TextAntialiasing);
        graphicsView->setDragMode(QGraphicsView::DragMode::RubberBandDrag);
        graphicsView->setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
        graphicsView->setResizeAnchor(QGraphicsView::ViewportAnchor::AnchorViewCenter);

        verticalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuNode = new QMenu(menubar);
        menuNode->setObjectName("menuNode");
        menuAdd = new QMenu(menuNode);
        menuAdd->setObjectName("menuAdd");
        menuDelete = new QMenu(menuNode);
        menuDelete->setObjectName("menuDelete");
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName("dockWidget");
        nodeInspectorDock = new QWidget();
        nodeInspectorDock->setObjectName("nodeInspectorDock");
        verticalLayout_2 = new QVBoxLayout(nodeInspectorDock);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(nodeInspectorDock);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        NodeON = new QRadioButton(nodeInspectorDock);
        NodeON->setObjectName("NodeON");

        horizontalLayout->addWidget(NodeON);

        NodeOFF = new QRadioButton(nodeInspectorDock);
        NodeOFF->setObjectName("NodeOFF");

        horizontalLayout->addWidget(NodeOFF);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout_3);

        tableView = new QTableView(nodeInspectorDock);
        tableView->setObjectName("tableView");

        verticalLayout_2->addWidget(tableView);

        dockWidget->setWidget(nodeInspectorDock);
        MainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dockWidget);

        menubar->addAction(menuNode->menuAction());
        menuNode->addAction(menuAdd->menuAction());
        menuNode->addAction(menuDelete->menuAction());
        menuNode->addAction(actionClear);
        menuNode->addAction(actionSave);
        menuNode->addAction(actionOpen);
        menuNode->addAction(actionZoom_In);
        menuNode->addAction(actionZoom_Out);
        menuNode->addAction(actionShow_All);
        menuAdd->addAction(actionAddNode);
        menuAdd->addAction(actionAddConnection);
        menuDelete->addAction(actionDelete_Connection);
        menuDelete->addAction(actionDelete_Node);
        toolBar->addAction(actionZoom_In);
        toolBar->addAction(actionZoom_Out);
        toolBar->addAction(actionAddNode);
        toolBar->addAction(actionAddConnection);
        toolBar->addAction(actionDelete_Connection);
        toolBar->addAction(actionDelete_Node);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionZoom_In->setText(QCoreApplication::translate("MainWindow", "Zoom In", nullptr));
        actionZoom_Out->setText(QCoreApplication::translate("MainWindow", "Zoom Out", nullptr));
        actionAddNode->setText(QCoreApplication::translate("MainWindow", "Add Node", nullptr));
#if QT_CONFIG(tooltip)
        actionAddNode->setToolTip(QCoreApplication::translate("MainWindow", "Node", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionAddNode->setShortcut(QCoreApplication::translate("MainWindow", "N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAddConnection->setText(QCoreApplication::translate("MainWindow", "Add Connection", nullptr));
#if QT_CONFIG(shortcut)
        actionAddConnection->setShortcut(QCoreApplication::translate("MainWindow", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete_Node->setText(QCoreApplication::translate("MainWindow", "Delete Node", nullptr));
        actionDelete_Node_2->setText(QCoreApplication::translate("MainWindow", "Delete Node", nullptr));
        actionDelete_Connection->setText(QCoreApplication::translate("MainWindow", "Delete Connection", nullptr));
#if QT_CONFIG(tooltip)
        actionDelete_Connection->setToolTip(QCoreApplication::translate("MainWindow", "Delete Connection", nullptr));
#endif // QT_CONFIG(tooltip)
        actionShow_All->setText(QCoreApplication::translate("MainWindow", "Show All Connections", nullptr));
        menuNode->setTitle(QCoreApplication::translate("MainWindow", "Node", nullptr));
        menuAdd->setTitle(QCoreApplication::translate("MainWindow", "Add", nullptr));
        menuDelete->setTitle(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Node ID", nullptr));
        NodeON->setText(QCoreApplication::translate("MainWindow", "ON", nullptr));
        NodeOFF->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
