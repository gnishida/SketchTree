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
	for (int i = 0; i < 2; ++i) {
		glWidget->sketch[i].fill(qRgba(255, 255, 255, 0));
	}
	glWidget->update();
}

void MainWindow::onLoadSketch() {
	QString filename[2];
	for (int i = 0; i < 2; ++i) {
		filename[i] = QFileDialog::getOpenFileName(this, tr("Open sketch file..."), "", tr("sketch Files (*.png)"));
		if (filename[i].isEmpty()) return;
	}

	for (int i = 0; i < 2; ++i) {
		glWidget->sketch[i].load(filename[i]);
	}

	glWidget->update();
}

void MainWindow::onSaveSketch() {
	QString filename[2];
	for (int i = 0; i < 2; ++i) {
		filename[i] = QFileDialog::getSaveFileName(this, tr("Save sketch file..."), "", tr("Sketch Files (*.png)"));
		if (filename[i].isEmpty()) return;
	}

	for (int i = 0; i < 2; ++i) {
		glWidget->sketch[i].save(filename[i]);
	}
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

	std::vector<cv::Mat> indicator;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, glm::mat4(), indicator);
	ml::mat_save("indicator.png", indicator[0]);

	glWidget->lsystem.draw(glWidget->model, glWidget->vertices);
	glWidget->createVAO();
	glWidget->update();
}

void MainWindow::onGreedyInverse() {
	std::vector<cv::Mat> target;
	target.resize(2);
	for (int i = 0; i < 2; ++i) {
		// スケッチ[BGRA]を、cv::Matに変換する
		target[i] = cv::Mat(glWidget->sketch[i].height(), glWidget->sketch[i].width(), CV_8UC4, const_cast<uchar*>(glWidget->sketch[i].bits()), glWidget->sketch[i].bytesPerLine());

		// Alphaチャネルのみ抽出
		std::vector<cv::Mat> planes;
		cv::split(target[i], planes);
	
		// 300x300に変換する
		cv::resize(planes[3], target[i], cv::Size(300, 300));

		// float型 [0, 1]に変換する
		target[i].convertTo(target[i], CV_32F, 1.0/255.0);

		// 上下反転させる
		cv::flip(target[i], target[i], 0);

		char filename[256];
		sprintf(filename, "target%d.png", i);
		ml::mat_save(filename, target[i]);
	}

	// ターゲットに近いモデルを生成する
	time_t start = clock();
	glWidget->model = glWidget->lsystem.inverse(target, glWidget->camera.mvpMatrix);
	time_t end = clock();

	cout << glWidget->model << endl;

	cout << fixed << "Elapsed: " << (double)(end - start) / CLOCKS_PER_SEC  << " [sec]" << endl;

	// 生成したモデルの画像を保存する
	std::vector<cv::Mat> indicator;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, glm::mat4(), indicator);
	ml::mat_save("result.png", indicator[0] + target[0] * 0.4);

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
		glWidget->pen.setType(Pen::TYPE_BRANCH);
	} else {
		glWidget->pen.setType(Pen::TYPE_LEAF);
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
