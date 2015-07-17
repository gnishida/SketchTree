#include "GLWidget3D.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
     xRot = 0;
     yRot = 0;
     zRot = 0;

     setAutoFillBackground(false);
}

static void qNormalizeAngle(int &angle) {
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget3D::setXRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != xRot) xRot = angle;

	update();
}

void GLWidget3D::setYRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != yRot) yRot = angle;

	update();
}

void GLWidget3D::setZRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != zRot) zRot = angle;

	update();
}

void GLWidget3D::initializeGL() {
	glEnable(GL_MULTISAMPLE);
	qglClearColor(QColor(113, 112, 117));
}

void GLWidget3D::mousePressEvent(QMouseEvent *event) {
     lastPos = event->pos();
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *event) {
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
}

void GLWidget3D::paintEvent(QPaintEvent *event) {
	// OpenGLで描画
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	setupViewport(width(), height());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(0, 0, 1);
	glVertex3f(-2, -2, 0);
	glVertex3f(2, -2, 0);
	glVertex3f(0, 4, 0);
	glEnd();

	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// QPainterで描画
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(255, 32, 0));
	painter.drawRect(30, 30, 100, 100);
	painter.end();
}

void GLWidget3D::resizeGL(int width, int height) {
	setupViewport(width, height);
}

void GLWidget3D::setupViewport(int width, int height) {
	height = height ? height : 1;
	glViewport(0, 0, (GLint)width, (GLint)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width/(GLfloat)height, 0.01f, 100);
	glMatrixMode(GL_MODELVIEW);
}

