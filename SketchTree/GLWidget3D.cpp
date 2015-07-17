#include "GLWidget3D.h"
#include <iostream>
#include "GLUtils.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), lsystem(300, 1, parametriclsystem::Literal("X", 0, 36.0f, 9.0f)) {
	mode = MODE_SKETCH;
	dragging = false;
	color = QColor(0, 0, 0);
	penWidth = 20;
	
	// これがないと、QPainterによって、OpenGLによる描画がクリアされてしまう
	setAutoFillBackground(false);
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene(int drawMode) {
	if (drawMode == 0) {
		glUniform1i(glGetUniformLocation(program,"shadowState"), 1);
	} else {
		glUniform1i(glGetUniformLocation(program,"shadowState"), 2);
	}

	// use color mode
	glUniform1i(glGetUniformLocation(program, "mode"), 1);

	// bind the vao and draw it.
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

/**
 * Create VAO according to the vertices.
 */
void GLWidget3D::createVAO() {
	// create vao and bind it
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// create VBO and tranfer the vertices data to GPU buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	// configure the attributes in the vao
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	// unbind the vao
	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// シャドウマップを更新
	shadow.makeShadowMap(this, light_dir);
}

void GLWidget3D::drawLineTo(const QPoint &endPoint) {
	QPoint pt1(lastPoint.x(), lastPoint.y());
	QPoint pt2(endPoint.x(), endPoint.y());

	QPainter painter(&sketch);
	painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	painter.drawLine(pt1, pt2);

	lastPoint = endPoint;
}

void GLWidget3D::resizeGL(int width, int height) {
	// sketch imageを更新
	QImage newImage(width, height, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 0));
	QPainter painter(&newImage);
	int offset_x = (width - sketch.size().width()) * 0.5;
	int offset_y = (height - sketch.size().height()) * 0.5;
	painter.drawImage(QPoint(offset_x, offset_y), sketch);
	sketch = newImage;

	// OpenGLの設定を更新
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);
}

void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	if (mode == MODE_SKETCH) {
		lastPoint = e->pos();
		dragging = true;
	} else {
		camera.mousePress(e->x(), e->y());
	}
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
	// init glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-0.1, 1, -0.2));

	// load shaders
	Shader shader;
	program = shader.createProgram("../shaders/vertex.glsl", "../shaders/fragment.glsl");
	glUseProgram(program);

	shadow.initShadow(program, 4096, 4096);

	// set the clear color for the screen
	qglClearColor(QColor(224, 224, 224));

	glutils::drawSphere(glm::vec3(0, 0, 0), 10, glm::vec3(1, 1, 1), glm::mat4(), vertices);
	createVAO();
}

void GLWidget3D::paintEvent(QPaintEvent *event) {
	// OpenGLで描画
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// Model view projection行列をシェーダに渡す
	glUniformMatrix4fv(glGetUniformLocation(program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

	// pass the light direction to the shader
	glUniform1fv(glGetUniformLocation(program, "lightDir"), 3, &light_dir[0]);
	
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
	/*painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(255, 32, 0));
	painter.drawRect(30, 30, 100, 100);
	*/
	painter.drawImage(0, 0, sketch);
	painter.end();
}
