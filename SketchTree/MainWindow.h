#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glew.h"
#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

public slots:
	void onNewSketch();
	void onLoadSketch();
	void onSaveSketch();
	void onSaveImage();
	void onRandomGeneration();
	void onGreedyInverse();
	void onModeUpdate();
	void onPenColorUpdate();
	void onPenWidthUpdate();
};

#endif // MAINWINDOW_H
