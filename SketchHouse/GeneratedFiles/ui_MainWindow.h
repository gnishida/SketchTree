/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Thu Jul 23 12:50:22 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionGreedyInverse;
    QAction *actionSaveImage;
    QAction *actionModeSketch;
    QAction *actionMode3DView;
    QAction *actionRandomGeneration_2;
    QAction *actionRandomGeneration;
    QAction *actionLoadSketch;
    QAction *actionSaveSketch;
    QAction *actionNewSketch;
    QAction *actionPenColorWall;
    QAction *actionPenColorDoor;
    QAction *actionPenWidth20;
    QAction *actionPenWidth10;
    QAction *actionPenWidth5;
    QAction *actionPenColorWindow;
    QAction *actionLoadCamera;
    QAction *actionSaveCamera;
    QAction *actionResetCamera;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuL_System;
    QMenu *menuMode;
    QMenu *menuPen;
    QMenu *menuColor;
    QMenu *menuWidth;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(600, 600);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionGreedyInverse = new QAction(MainWindowClass);
        actionGreedyInverse->setObjectName(QString::fromUtf8("actionGreedyInverse"));
        actionSaveImage = new QAction(MainWindowClass);
        actionSaveImage->setObjectName(QString::fromUtf8("actionSaveImage"));
        actionModeSketch = new QAction(MainWindowClass);
        actionModeSketch->setObjectName(QString::fromUtf8("actionModeSketch"));
        actionModeSketch->setCheckable(true);
        actionModeSketch->setChecked(true);
        actionMode3DView = new QAction(MainWindowClass);
        actionMode3DView->setObjectName(QString::fromUtf8("actionMode3DView"));
        actionMode3DView->setCheckable(true);
        actionRandomGeneration_2 = new QAction(MainWindowClass);
        actionRandomGeneration_2->setObjectName(QString::fromUtf8("actionRandomGeneration_2"));
        actionRandomGeneration = new QAction(MainWindowClass);
        actionRandomGeneration->setObjectName(QString::fromUtf8("actionRandomGeneration"));
        actionLoadSketch = new QAction(MainWindowClass);
        actionLoadSketch->setObjectName(QString::fromUtf8("actionLoadSketch"));
        actionSaveSketch = new QAction(MainWindowClass);
        actionSaveSketch->setObjectName(QString::fromUtf8("actionSaveSketch"));
        actionNewSketch = new QAction(MainWindowClass);
        actionNewSketch->setObjectName(QString::fromUtf8("actionNewSketch"));
        actionPenColorWall = new QAction(MainWindowClass);
        actionPenColorWall->setObjectName(QString::fromUtf8("actionPenColorWall"));
        actionPenColorWall->setCheckable(true);
        actionPenColorDoor = new QAction(MainWindowClass);
        actionPenColorDoor->setObjectName(QString::fromUtf8("actionPenColorDoor"));
        actionPenColorDoor->setCheckable(true);
        actionPenWidth20 = new QAction(MainWindowClass);
        actionPenWidth20->setObjectName(QString::fromUtf8("actionPenWidth20"));
        actionPenWidth20->setCheckable(true);
        actionPenWidth10 = new QAction(MainWindowClass);
        actionPenWidth10->setObjectName(QString::fromUtf8("actionPenWidth10"));
        actionPenWidth10->setCheckable(true);
        actionPenWidth5 = new QAction(MainWindowClass);
        actionPenWidth5->setObjectName(QString::fromUtf8("actionPenWidth5"));
        actionPenWidth5->setCheckable(true);
        actionPenColorWindow = new QAction(MainWindowClass);
        actionPenColorWindow->setObjectName(QString::fromUtf8("actionPenColorWindow"));
        actionLoadCamera = new QAction(MainWindowClass);
        actionLoadCamera->setObjectName(QString::fromUtf8("actionLoadCamera"));
        actionSaveCamera = new QAction(MainWindowClass);
        actionSaveCamera->setObjectName(QString::fromUtf8("actionSaveCamera"));
        actionResetCamera = new QAction(MainWindowClass);
        actionResetCamera->setObjectName(QString::fromUtf8("actionResetCamera"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuL_System = new QMenu(menuBar);
        menuL_System->setObjectName(QString::fromUtf8("menuL_System"));
        menuMode = new QMenu(menuBar);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        menuPen = new QMenu(menuBar);
        menuPen->setObjectName(QString::fromUtf8("menuPen"));
        menuColor = new QMenu(menuPen);
        menuColor->setObjectName(QString::fromUtf8("menuColor"));
        menuWidth = new QMenu(menuPen);
        menuWidth->setObjectName(QString::fromUtf8("menuWidth"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuL_System->menuAction());
        menuBar->addAction(menuMode->menuAction());
        menuBar->addAction(menuPen->menuAction());
        menuFile->addAction(actionNewSketch);
        menuFile->addAction(actionLoadSketch);
        menuFile->addAction(actionSaveSketch);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveImage);
        menuFile->addSeparator();
        menuFile->addAction(actionLoadCamera);
        menuFile->addAction(actionSaveCamera);
        menuFile->addAction(actionResetCamera);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuL_System->addAction(actionRandomGeneration);
        menuL_System->addAction(actionGreedyInverse);
        menuMode->addAction(actionModeSketch);
        menuMode->addAction(actionMode3DView);
        menuPen->addAction(menuColor->menuAction());
        menuPen->addAction(menuWidth->menuAction());
        menuColor->addAction(actionPenColorWall);
        menuColor->addAction(actionPenColorDoor);
        menuColor->addAction(actionPenColorWindow);
        menuWidth->addAction(actionPenWidth20);
        menuWidth->addAction(actionPenWidth10);
        menuWidth->addAction(actionPenWidth5);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Sketch Application", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "&Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionGreedyInverse->setText(QApplication::translate("MainWindowClass", "&Greedy Inverse", 0, QApplication::UnicodeUTF8));
        actionGreedyInverse->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionSaveImage->setText(QApplication::translate("MainWindowClass", "Save Image", 0, QApplication::UnicodeUTF8));
        actionModeSketch->setText(QApplication::translate("MainWindowClass", "Sketch", 0, QApplication::UnicodeUTF8));
        actionMode3DView->setText(QApplication::translate("MainWindowClass", "3D View", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration_2->setText(QApplication::translate("MainWindowClass", "Random Generation", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration->setText(QApplication::translate("MainWindowClass", "&Random Generation", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionLoadSketch->setText(QApplication::translate("MainWindowClass", "&Load Sketch", 0, QApplication::UnicodeUTF8));
        actionLoadSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSaveSketch->setText(QApplication::translate("MainWindowClass", "&Save Sketch", 0, QApplication::UnicodeUTF8));
        actionSaveSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionNewSketch->setText(QApplication::translate("MainWindowClass", "&New Sketch", 0, QApplication::UnicodeUTF8));
        actionNewSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionPenColorWall->setText(QApplication::translate("MainWindowClass", "Wall", 0, QApplication::UnicodeUTF8));
        actionPenColorDoor->setText(QApplication::translate("MainWindowClass", "Door", 0, QApplication::UnicodeUTF8));
        actionPenWidth20->setText(QApplication::translate("MainWindowClass", "20", 0, QApplication::UnicodeUTF8));
        actionPenWidth10->setText(QApplication::translate("MainWindowClass", "10", 0, QApplication::UnicodeUTF8));
        actionPenWidth5->setText(QApplication::translate("MainWindowClass", "5", 0, QApplication::UnicodeUTF8));
        actionPenColorWindow->setText(QApplication::translate("MainWindowClass", "Window", 0, QApplication::UnicodeUTF8));
        actionLoadCamera->setText(QApplication::translate("MainWindowClass", "Load Camera", 0, QApplication::UnicodeUTF8));
        actionSaveCamera->setText(QApplication::translate("MainWindowClass", "Save Camera", 0, QApplication::UnicodeUTF8));
        actionResetCamera->setText(QApplication::translate("MainWindowClass", "Reset Camera", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "&File", 0, QApplication::UnicodeUTF8));
        menuL_System->setTitle(QApplication::translate("MainWindowClass", "&L-System", 0, QApplication::UnicodeUTF8));
        menuMode->setTitle(QApplication::translate("MainWindowClass", "&Mode", 0, QApplication::UnicodeUTF8));
        menuPen->setTitle(QApplication::translate("MainWindowClass", "&Pen", 0, QApplication::UnicodeUTF8));
        menuColor->setTitle(QApplication::translate("MainWindowClass", "&Color", 0, QApplication::UnicodeUTF8));
        menuWidth->setTitle(QApplication::translate("MainWindowClass", "&Width", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
