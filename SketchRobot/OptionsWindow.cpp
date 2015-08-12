#include "OptionsWindow.h"
#include "MainWindow.h"


OptionsWindow::OptionsWindow(MainWindow* parent) : QDialog((QWidget*)parent) {
	// set up the UI
	ui.setupUi(this);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}
