#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
#include "Icosahedron.h"
#include "camera.h"



class QViewport : public QOpenGLWidget {
	Q_OBJECT

public:
	QViewport(QWidget* parent);
	void drawIcos();
	virtual ~QViewport();

	bool renderTeapot, renderIcos, renderCubeMap, light0, light1, light2;
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void mouseMoveEvent(QMouseEvent *event) override;
	void lightSettings();
	void toggleTeapot();
	void toggleIcos();
	void toggleCubeMap();

	QPoint lastPos;
	Icosahedron icos;
	int subdivision;

	Camera cameraXYZ;
	Camera cameraRTP;
};

