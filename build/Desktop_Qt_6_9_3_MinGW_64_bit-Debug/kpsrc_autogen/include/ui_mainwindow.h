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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDelete;
    QAction *actionClear;
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionAddNode;
    QAction *actionAddConnection;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuNode;
    QMenu *menuAdd;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName("actionDelete");
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
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuNode->menuAction());
        menuNode->addAction(menuAdd->menuAction());
        menuNode->addAction(actionDelete);
        menuNode->addAction(actionClear);
        menuNode->addAction(actionSave);
        menuNode->addAction(actionOpen);
        menuNode->addAction(actionZoom_In);
        menuNode->addAction(actionZoom_Out);
        menuAdd->addAction(actionAddNode);
        menuAdd->addAction(actionAddConnection);
        toolBar->addAction(actionZoom_In);
        toolBar->addAction(actionZoom_Out);
        toolBar->addAction(actionAddNode);
        toolBar->addAction(actionAddConnection);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionDelete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
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
        menuNode->setTitle(QCoreApplication::translate("MainWindow", "Node", nullptr));
        menuAdd->setTitle(QCoreApplication::translate("MainWindow", "Add", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
