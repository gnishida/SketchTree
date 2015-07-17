/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Fri Jul 17 14:35:23 2015
**      by: Qt User Interface Compiler version 4.8.1
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
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuL_System;
    QMenu *menuMode;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(600, 400);
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
        menuFile->addAction(actionSaveImage);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuL_System->addAction(actionRandomGeneration);
        menuL_System->addAction(actionGreedyInverse);
        menuMode->addAction(actionModeSketch);
        menuMode->addAction(actionMode3DView);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionGreedyInverse->setText(QApplication::translate("MainWindowClass", "Greedy Inverse", 0, QApplication::UnicodeUTF8));
        actionSaveImage->setText(QApplication::translate("MainWindowClass", "Save Image", 0, QApplication::UnicodeUTF8));
        actionModeSketch->setText(QApplication::translate("MainWindowClass", "Sketch", 0, QApplication::UnicodeUTF8));
        actionMode3DView->setText(QApplication::translate("MainWindowClass", "3D View", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration_2->setText(QApplication::translate("MainWindowClass", "Random Generation", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration->setText(QApplication::translate("MainWindowClass", "Random Generation", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuL_System->setTitle(QApplication::translate("MainWindowClass", "L-System", 0, QApplication::UnicodeUTF8));
        menuMode->setTitle(QApplication::translate("MainWindowClass", "Mode", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
