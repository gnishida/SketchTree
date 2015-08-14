#include "MainWindow.h"
#include "MLUtils.h"
#include <QFileDialog>
#include <QDate>
#include "OptionsWindow.h"
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
	connect(ui.actionLoadCamera, SIGNAL(triggered()), this, SLOT(onLoadCamera()));
	connect(ui.actionSaveCamera, SIGNAL(triggered()), this, SLOT(onSaveCamera()));
	connect(ui.actionResetCamera, SIGNAL(triggered()), this, SLOT(onResetCamera()));
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
	connect(ui.actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

void MainWindow::onNewSketch() {
	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		glWidget->sketch[i].fill(qRgba(255, 255, 255, 0));
	}
	glWidget->update();
}

void MainWindow::onLoadSketch() {
	QString filename[parametriclsystem::NUM_LAYERS];
	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		filename[i] = QFileDialog::getOpenFileName(this, tr("Open sketch file..."), "", tr("sketch Files (*.png)"));
		if (filename[i].isEmpty()) return;
	}

	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		glWidget->sketch[i].load(filename[i]);
	}

	glWidget->resizeSketch(glWidget->width(), glWidget->height());
	glWidget->update();
}

void MainWindow::onSaveSketch() {
	QString filename[parametriclsystem::NUM_LAYERS];
	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		filename[i] = QFileDialog::getSaveFileName(this, tr("Save sketch file..."), "", tr("Sketch Files (*.png)"));
		if (filename[i].isEmpty()) return;
	}

	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		glWidget->sketch[i].save(filename[i]);
	}
}

void MainWindow::onSaveImage() {
	if (!QDir("screenshots").exists()) QDir().mkdir("screenshots");
	QString fileName = "screenshots/" + QDate::currentDate().toString("yyMMdd") + "_" + QTime::currentTime().toString("HHmmss") + ".png";
	glWidget->grabFrameBuffer().save(fileName);
	printf("Save %s\n",fileName.toAscii().constData());
}

void MainWindow::onLoadCamera() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open camera file..."), "", tr("Camera Files (*.cam)"));
	if (filename.isEmpty()) return;

	glWidget->camera.loadCameraPose(filename.toUtf8().data());
	glWidget->update();
}

void MainWindow::onSaveCamera() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save camera file..."), "", tr("Camera Files (*.cam)"));
	if (filename.isEmpty()) return;

	glWidget->camera.saveCameraPose(filename.toUtf8().data());
}

void MainWindow::onResetCamera() {
	glWidget->camera.resetCamera();
	glWidget->update();
}

void MainWindow::onRandomGeneration() {
	//glWidget->model = glWidget->lsystem.derive(12345);
	glWidget->model = glWidget->lsystem.derive(time(NULL));

	cout << glWidget->model << endl;

	std::vector<cv::Mat> indicator;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, indicator);
	for (int i = 0; i < indicator.size(); ++i) {
		char filename[256];
		sprintf(filename, "indicator%d.png", i);
		ml::mat_save(filename, indicator[i]);
	}

	glWidget->lsystem.draw(glWidget->model, &glWidget->renderManager);
	glWidget->renderManager.updateShadowMap(glWidget, glWidget->light_dir, glWidget->light_mvpMatrix);
	glWidget->update();
}

void MainWindow::onGreedyInverse() {
	std::vector<cv::Mat> target;
	target.resize(parametriclsystem::NUM_LAYERS);
	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		// スケッチ[BGRA]を、cv::Matに変換する
		target[i] = cv::Mat(glWidget->sketch[i].height(), glWidget->sketch[i].width(), CV_8UC4, const_cast<uchar*>(glWidget->sketch[i].bits()), glWidget->sketch[i].bytesPerLine());

		// Alphaチャネルのみ抽出
		std::vector<cv::Mat> planes;
		cv::split(target[i], planes);
	
		// grid sizeに変換する
		cv::resize(planes[3], target[i], cv::Size(parametriclsystem::GRID_SIZE, parametriclsystem::GRID_SIZE));

		// float型 [0, 1]に変換する
		target[i].convertTo(target[i], CV_32F, 1.0/255.0);

		// 上下反転させる
		cv::flip(target[i], target[i], 0);

		// clamp
		cv::threshold(target[i], target[i], 0.0, 1.0, cv::THRESH_BINARY);

		/////// デバッグ ///////
		/*{
		char filename[256];
		sprintf(filename, "target%d.png", i);
		ml::mat_save(filename, target[i]);
		}*/
		/////// デバッグ ///////
	}

	// ターゲットに近いモデルを生成する
	time_t start = clock();
	glWidget->model = glWidget->lsystem.inverse(target, glWidget->camera.mvpMatrix);
	time_t end = clock();

	//cout << glWidget->model << endl;

	cout << fixed << "Elapsed: " << (double)(end - start) / CLOCKS_PER_SEC  << " [sec]" << endl;

	// 生成したモデルの画像を保存する
	std::vector<cv::Mat> indicator;
	glWidget->lsystem.computeIndicator(glWidget->model, glWidget->camera.mvpMatrix, indicator);
	for (int i = 0; i < indicator.size(); ++i) {
		char filename[256];
		sprintf(filename, "result%d.png", i);
		ml::mat_save(filename, indicator[i] + target[i] * 0.4);
	}

	glWidget->lsystem.draw(glWidget->model, &glWidget->renderManager);
	glWidget->renderManager.updateShadowMap(glWidget, glWidget->light_dir, glWidget->light_mvpMatrix);
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

void MainWindow::onOptions() {
	OptionsWindow dlg(this);
	dlg.ui.lineEditMaxIterations->setText(QString::number(glWidget->lsystem.MAX_ITERATIONS));
	dlg.ui.lineEditMaxIterationsForMCSampling->setText(QString::number(glWidget->lsystem.MAX_ITERATIONS_FOR_MC));
	dlg.ui.lineEditNumMonteCarloSamplings->setText(QString::number(glWidget->lsystem.NUM_MONTE_CARLO_SAMPLING));
	dlg.ui.lineEditMaskRadiusRatio->setText(QString::number(glWidget->lsystem.MASK_RADIUS_RATIO));
	if (dlg.exec() != QDialog::Accepted) return;

	glWidget->lsystem.MAX_ITERATIONS = dlg.ui.lineEditMaxIterations->text().toInt();
	glWidget->lsystem.MAX_ITERATIONS_FOR_MC = dlg.ui.lineEditMaxIterationsForMCSampling->text().toInt();
	glWidget->lsystem.NUM_MONTE_CARLO_SAMPLING = dlg.ui.lineEditNumMonteCarloSamplings->text().toInt();
	glWidget->lsystem.MASK_RADIUS_RATIO = dlg.ui.lineEditMaskRadiusRatio->text().toDouble();
}