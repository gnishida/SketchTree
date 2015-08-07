#include "OptionsWindow.h"
#include "MainWindow.h"


OptionsWindow::OptionsWindow(MainWindow* parent) : QDialog((QWidget*)parent) {
	// set up the UI
	ui.setupUi(this);
}
