#pragma once

#include "glew.h"

#include "ui_OptionsWindow.h"

class MainWindow;

class OptionsWindow : public QDialog {
Q_OBJECT

public:
	Ui::OptionsWindow ui;

public:
	OptionsWindow(MainWindow* parent);
};

