#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <glew.h>
#include "Shader.h"
#include "Vertex.h"
#include <QPen>
#include <QGLWidget>
#include <QtGui>
#include "ShadowMapping.h"
#include "Camera.h"

class GLWidget3D : public QGLWidget {
	Q_OBJECT

private:
	Camera camera;
	GLuint vao;
	GLuint program;
	std::vector<Vertex> vertices;
	glm::vec3 light_dir;
	ShadowMapping shadow;

public:
	GLWidget3D(QWidget *parent = 0);

	void drawScene(int drawMode);
	void createVAO();

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void initializeGL();
	void paintEvent(QPaintEvent *event);
};

#endif