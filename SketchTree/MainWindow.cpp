#include "MainWindow.h"
#include "MLUtils.h"
#include <QFileDialog>
#include <QDate>
#include <time.h>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	QActionGroup* group = new QActionGroup(this);
	ui.actionModeSketch->setCheckable(true);
	ui.actionMode3DView->setCheckable(true);
	ui.actionModeSketch->setActionGroup(group);
	ui.actionMode3DView->setActionGroup(group);


	connect(ui.actionSaveImage, SIGNAL(triggered()), this, SLOT(onSaveImage()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionRandomGeneration, SIGNAL(triggered()), this, SLOT(onRandomGeneration()));
	connect(ui.actionGreedyInverse, SIGNAL(triggered()), this, SLOT(onGreedyInverse()));

	connect(ui.actionModeSketch, SIGNAL(triggered()), this, SLOT(onModeUpdate()));
	connect(ui.actionMode3DView, SIGNAL(triggered()), this, SLOT(onModeUpdate()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

void MainWindow::onSaveImage() {
	if (!QDir("screenshots").exists()) QDir().mkdir("screenshots");
	QString fileName = "screenshots/" + QDate::currentDate().toString("yyMMdd") + "_" + QTime::currentTime().toString("HHmmss") + ".png";
	glWidget->grabFrameBuffer().save(fileName);
	printf("Save %s\n",fileName.toAscii().constData());
}

void MainWindow::onRandomGeneration() {
	//glWidget->model = glWidget->lsystem.derive(12345);
	glWidget->model = glWidget->lsystem.derive(time(NULL));

	cout << glWidget->model << endl;

	glWidget->lsystem.draw(glWidget->model, glWidget->vertices);
	glWidget->createVAO();
	glWidget->update();
}

void MainWindow::onGreedyInverse() {
	// スケッチ[BGRA]を、cv::Matに変換する
	cv::Mat target(glWidget->sketch.height(), glWidget->sketch.width(), CV_8UC4, const_cast<uchar*>(glWidget->sketch.bits()), glWidget->sketch.bytesPerLine());

	// Alphaチャネルのみ抽出
	std::vector<cv::Mat> planes;
	cv::split(target, planes);
	
	// 300x300に変換する
	cv::resize(planes[3], target, cv::Size(300, 300));

	// float型 [0, 1]に変換する
	target.convertTo(target, CV_32F, 1.0/255.0);

	// 上下反転させる
	cv::flip(target, target, 0);

	// 白黒を反転させる
	//target = 1 - target;

	ml::mat_save("target.png", target);

	// ターゲットに近いモデルを生成する
	time_t start = clock();
	glWidget->model = glWidget->lsystem.inverse(target);
	time_t end = clock();

	cout << glWidget->model << endl;

	cout << fixed << "Elapsed: " << (double)(end - start) / CLOCKS_PER_SEC  << " [sec]" << endl;

	// 生成したモデルの画像を保存する
	cv::Mat img;
	glWidget->lsystem.computeIndicator(glWidget->model, 1.0f, glm::mat4(), img);
	ml::mat_save("result.png", img + target * 0.4);

	glWidget->lsystem.draw(glWidget->model, glWidget->vertices);
	glWidget->createVAO();
	glWidget->update();
}

void MainWindow::onModeUpdate() {
	if (ui.actionModeSketch->isChecked()) {
		glWidget->mode = GLWidget3D::MODE_SKETCH;
	} else {
		glWidget->mode = GLWidget3D::MODE_3DVIEW;
	}
}
