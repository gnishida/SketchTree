#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderManager.h"
#include "Shader.h"
#include "Vertex.h"
#include <QPen>
#include <QGLWidget>
#include <QtGui>
#include "ParametricLSystem.h"
#include "ShadowMapping.h"
#include "Camera.h"

class Pen : public QPen {
public:
	static enum { TYPE_WALL = 0, TYPE_DOOR, TYPE_WINDOW };

public:
	int type;

public:
	Pen();

	void setType(int type);
};

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	static enum { MODE_SKETCH = 0, MODE_3DVIEW };

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	RenderManager renderManager;

	parametriclsystem::ParametricLSystem lsystem;
	parametriclsystem::String model;

	int mode;	// 0 -- sketch / 1 -- 3D view
	bool dragging;
	QPoint lastPoint;
	QImage sketch[parametriclsystem::NUM_LAYERS];
	Pen pen;

public:
	GLWidget3D(QWidget *parent = 0);

	void drawScene(int drawMode);
	void drawLineTo(const QPoint &endPoint);
	void drawCircle(const QPoint &point);
	void eraseLineTo(const QPoint &endPoint);
	void eraseCircle(const QPoint &point);

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *e);
	void initializeGL();
	void paintEvent(QPaintEvent *event);
};

#endif