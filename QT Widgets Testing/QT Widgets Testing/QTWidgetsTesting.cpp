#include "QTWidgetsTesting.h"
#include <iostream>
#include <qtimer.h>
#include <QColorDialog>

QTWidgetsTesting::QTWidgetsTesting(QWidget *parent) : QMainWindow(parent) {
	this->ui.setupUi(this);
}



void QTWidgetsTesting::teapotRadio(bool val){
	this->ui.viewport->renderTeapot = val;
	this->ui.viewport->renderIcos = !val;
	this->ui.viewport->update();
}

void QTWidgetsTesting::icosRadio(bool val){
	this->ui.viewport->renderTeapot = !val;
	this->ui.viewport->renderIcos = val;
	this->ui.viewport->update();
}

void QTWidgetsTesting::cubeMapCheckBox(bool status){
	this->ui.viewport->renderCubeMap = status;
	this->ui.viewport->update();
}

void QTWidgetsTesting::onLight0(){

	this->ui.viewport->light0 = !this->ui.viewport->light0;
	this->ui.viewport->update();
}

void QTWidgetsTesting::onLight1(){
	this->ui.viewport->light1 = !this->ui.viewport->light1;
	this->ui.viewport->update();
}

void QTWidgetsTesting::onLight2(){
	this->ui.viewport->light2 = !this->ui.viewport->light2;
	this->ui.viewport->update();
}


void QTWidgetsTesting::icosLevel(int level){
	this->ui.viewport->subdivision = level;
	this->ui.viewport->update();
}
