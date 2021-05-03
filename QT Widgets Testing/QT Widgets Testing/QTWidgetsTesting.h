#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTWidgetsTesting.h"


class QTWidgetsTesting : public QMainWindow {
	Q_OBJECT

public:
	QTWidgetsTesting(QWidget *parent = Q_NULLPTR);

public slots:
	void teapotRadio(bool val);
	void icosRadio(bool val);
	void cubeMapCheckBox(bool status);
	void onLight0();
	void onLight1();
	void onLight2();
	void icosLevel(int level);
private:
	Ui::QTWidgetsTestingClass ui;
};
