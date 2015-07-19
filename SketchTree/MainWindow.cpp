#include "MainWindow.h"
#include "MLUtils.h"
#include <QFileDialog>
#include <QDate>
#include <time.h>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// UI設定
	QActionGroup* groupMode = new QActionGroup(this);
	ui.actionModeSketch->setCheckable(true);
	ui.actionMode3DView->setCheckable(true);
	ui.actionModeSketch->setActionGroup(groupMode);
	ui.actionMode3DView->setActionGroup(groupMode);
	ui.actionModeSketch->setChecked(true);

	QActionGroup* groupPenColor = new QActionGroup(this);
	ui.actionPenColorBranch->setCheckable(true);
	ui.actionPenColorLeaf->setCheckable(true);
	ui.actionPenColorBranch->setActionGroup(groupPenColor);
	ui.actionPenColorLeaf->setActionGroup(groupPenColor);
	ui.actionPenColorBranch->setChecked(true);

	QActionGroup* groupPenWidth = new QActionGroup(this);
	ui.actionPenWidth20->setCheckable(true);
	ui.actionPenWidth10->setCheckable(true);
	ui.actionPenWidth5->setCheckable(true);
	ui.actionPenWidth20->setActionGroup(groupPenWidth);
	ui.actionPenWidth10->setActionGroup(groupPenWidth);
	ui.actionPenWidth5->setActionGroup(groupPenWidth);
	ui.actionPenWidth20->setChecked(true);

	// メニューハンドラ
	connect(ui.actionNewSketch, SIGNAL(triggered()), this, SLOT(onNewSketch()));
	connect(ui.actionLoadSketch, SIGNAL(triggered()), this, SLOT(onLoadSketch()));
	connect(ui.actionSaveSketch, SIGNAL(triggered()), this, SLOT(onSaveSketch()));
	connect(ui.actionSaveImage, SIGNAL(triggered()), this, SLOT(onSaveImage()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionRandomGeneration, SIGNAL(triggered()), this, SLOT(onRandomGeneration()));
	connect(ui.actionGreedyInverse, SIGNAL(triggered()), this, SLOT(onGreedyInverse()));
	connect(ui.actionModeSketch, SIGNAL(triggered()), this, SLOT(onModeUpdate()));
	connect(ui.actionMode3DView, SIGNAL(triggered()), this, SLOT(onModeUpdate()));
	connect(ui.actionPenColorBranch, SIGNAL(triggered()), this, SLOT(onPenColorUpdate()));
	connect(ui.actionPenColorLeaf, SIGNAL(triggered()), this, SLOT(onPenColorUpdate()));
	connect(ui.actionPenWidth20, SIGNAL(triggered()), this, SLOT(onPenWidthUpdate()));
	connect(ui.actionPenWidth10, SIGNAL(triggered()), this, SLOT(onPenWidthUpdate()));
	connect(ui.actionPenWidth5, SIGNAL(triggered()), this, SLOT(onPenWidthUpdate()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

void MainWindow::onNewSketch() {
	glWidget->sketch.fill(qRgba(255, 255, 255, 0));
	glWidget->update();
}

void MainWindow::onLoadSketch() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open sketch file..."), "", tr("sketch Files (*.png)"));
	if (filename.isEmpty()) return;

	glWidget->sketch.load(filename);

	glWidget->update();
}

void MainWindow::onSaveSketch() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save sketch file..."), "", tr("Sketch Files (*.png)"));
	if (filename.isEmpty()) return;

	glWidget->sketch.save(filename);
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

	cv::Mat indicator;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, glm::mat4(), indicator);
	ml::mat_save("indicator.png", indicator);

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
	glWidget->model = glWidget->lsystem.inverse(target, glWidget->camera.mvpMatrix);
	time_t end = clock();

	cout << glWidget->model << endl;

	cout << fixed << "Elapsed: " << (double)(end - start) / CLOCKS_PER_SEC  << " [sec]" << endl;

	// 生成したモデルの画像を保存する
	cv::Mat img;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, glm::mat4(), img);
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

void MainWindow::onPenColorUpdate() {
	if (ui.actionPenColorBranch->isChecked()) {
		glWidget->pen.setType(Pen::COLOR_BRANCH);
	} else {
		glWidget->pen.setType(Pen::COLOR_LEAF);
	}
}

void MainWindow::onPenWidthUpdate() {
	if (ui.actionPenWidth20->isChecked()) {
		glWidget->pen.setWidth(20);
	} else if (ui.actionPenWidth10->isChecked()) {
		glWidget->pen.setWidth(10);
	} else {
		glWidget->pen.setWidth(5);
	}
}
