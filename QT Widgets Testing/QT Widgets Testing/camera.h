#pragma once
//
// Created by jmful on 3/2/2021.
//

#ifndef INC_3785HW1_CAMERA_H
#define INC_3785HW1_CAMERA_H


// Camera class to handle the coordinates of our camera around the teapot

class Camera {

public:
	Camera()
	{
		x = y = z = 0.0;
	}
	Camera(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	double x, y, z;
};


#endif //INC_3785HW1_CAMERA_H

