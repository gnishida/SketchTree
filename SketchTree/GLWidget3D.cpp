#include "GLWidget3D.h"
#include <iostream>
#include "GLUtils.h"

Pen::Pen() {
	this->setType(TYPE_BRANCH);
	this->setWidth(20);
	this->setStyle(Qt::SolidLine);
	this->setCapStyle(Qt::RoundCap);
	this->setJoinStyle(Qt::RoundJoin);
}

void Pen::setType(int type) {
	this->type = type;
	if (type == TYPE_BRANCH) {
		this->setColor(QColor(128, 0, 0, 255));
	} else if (type == TYPE_LEAF) {
		this->setColor(QColor(0, 255, 0, 255));
	}
}

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), lsystem(300, parametriclsystem::Literal("X", 0, 24.0f, 0.0f)) {
	mode = MODE_SKETCH;
	dragging = false;
	pen.setType(Pen::TYPE_BRANCH);
	
	// これがないと、QPainterによって、OpenGLによる描画がクリアされてしまう
	setAutoFillBackground(false);
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene(int drawMode) {
	if (drawMode == 0) {
		glUniform1i(glGetUniformLocation(renderManager.program,"shadowState"), 1);
	} else {
		glUniform1i(glGetUniformLocation(renderManager.program,"shadowState"), 2);
	}

	renderManager.renderAll();
}

void GLWidget3D::drawLineTo(const QPoint &endPoint) {
	QPoint pt1(lastPoint.x(), lastPoint.y());
	QPoint pt2(endPoint.x(), endPoint.y());

	QPainter painter(&sketch[pen.type]);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	painter.drawLine(pt1, pt2);

	lastPoint = endPoint;
}

void GLWidget3D::drawCircle(const QPoint &point) {
	QPoint p(point.x(), point.y());

	QPainter painter(&sketch[pen.type]);
	painter.setPen(QPen(pen.color(), 1));
	painter.setBrush(QBrush(pen.color()));
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	painter.drawEllipse(p, (int)(pen.width() * 0.5), (int)(pen.width() * 0.5));
}

void GLWidget3D::resizeGL(int width, int height) {
	// OpenGLの設定を更新
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);
}

void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	if (mode == MODE_SKETCH) {
		lastPoint = e->pos();
		drawCircle(e->pos());
		dragging = true;
	} else {
		camera.mousePress(e->x(), e->y());
	}

	update();
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
	dragging = false;
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (mode == MODE_SKETCH) {
		drawLineTo(e->pos());
	} else {
		if (e->buttons() & Qt::LeftButton) { // Rotate
			camera.rotate(e->x(), e->y());
		} else if (e->buttons() & Qt::MidButton) { // Move
			camera.move(e->x(), e->y());
		} else if (e->buttons() & Qt::RightButton) { // Zoom
			camera.zoom(e->x(), e->y());
		}
	}

	update();
}

void GLWidget3D::initializeGL() {
	renderManager.init();

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-0.1, -0.2, -1));

	// set the clear color for the screen
	qglClearColor(QColor(224, 224, 224));

	std::vector<Vertex> vertices;
	glutils::drawAxes(1, 40, glm::mat4(), vertices);
	renderManager.addObject("axis", "", vertices);
}

void GLWidget3D::paintEvent(QPaintEvent *event) {
	// OpenGLで描画
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(renderManager.program);
	renderManager.updateShadowMap(this, light_dir);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// Model view projection行列をシェーダに渡す
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

	// pass the light direction to the shader
	glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
	
	drawScene(0);

	// OpenGLの設定を元に戻す
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// QPainterで描画
	QPainter painter(this);
	painter.setOpacity(0.5);
	if (!sketch[0].isNull()) {
		for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
			QImage img = sketch[i].scaled(width(), height(), Qt::KeepAspectRatio);
			painter.drawImage((width() - img.width()) * 0.5, (height() - img.height()) * 0.5, img);
		}
	}
	painter.end();
}
