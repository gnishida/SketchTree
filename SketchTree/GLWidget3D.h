#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <glew.h>
#include <QBrush>
#include <QImage>
#include <QPen>
#include <QGLWidget>
#include <QtGui>

class QPaintEvent;
class QWidget;

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	GLWidget3D(QWidget *parent = 0);

     int xRotation() const { return xRot; }
     int yRotation() const { return yRot; }
     int zRotation() const { return zRot; }

 public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

 protected:
     void initializeGL();
     void paintEvent(QPaintEvent *event);
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);

 private:
     void setupViewport(int width, int height);

     GLuint object;
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
 };

 #endif