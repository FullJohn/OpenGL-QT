#include "Icosahedron.h"
#include "gl/freeglut.h"

Icosahedron::Icosahedron() {
	this->visible = true;
	for (int i = 0; i < 4; i++)
		this->ambient[i] = 0.3f;
	this->ambient[3] = 1.0f;

	for (int i = 0; i < 4; i++)
		this->diffuse[i] = 0.6f;
	this->diffuse[3] = 1.0f;

	for (int i = 0; i < 4; i++)
		this->specular[i] = 0.5f;
	this->specular[3] = 1.0f;

	this->shininess = 24;
}

Icosahedron::~Icosahedron() {
	glDeleteLists(this->smooth_list_index, 1);
	glDeleteLists(this->flat_list_index, 1);
}

void Icosahedron::subdivide(int division_level) {
	glDeleteLists(this->smooth_list_index, 1);
	glDeleteLists(this->flat_list_index, 1);

	this->smooth_list_index = glGenLists(1);
	glNewList(this->smooth_list_index, GL_COMPILE);
	for (int i = 0; i < ISO_FACE_COUNT; i++)
		subdivide(icosahedron[tindices[i][0]], icosahedron[tindices[i][1]], icosahedron[tindices[i][2]], division_level, true);
	glEndList();

	this->flat_list_index = glGenLists(1);
	glNewList(this->flat_list_index, GL_COMPILE);
	for (int i = 0; i < ISO_FACE_COUNT; i++)
		subdivide(icosahedron[tindices[i][0]], icosahedron[tindices[i][1]], icosahedron[tindices[i][2]], division_level, false);
	glEndList();
}

void Icosahedron::render(bool smooth) {
	if (this->visible == false)
		return;

	this->apply_material();
	if (smooth == true) {
		glShadeModel(GL_SMOOTH);
		glCallList(this->smooth_list_index);
	}
	else {
		glShadeModel(GL_FLAT);
		glCallList(this->flat_list_index);
	}
}

void Icosahedron::setVisible(bool visible) {
	this->visible = visible;
}

void Icosahedron::setAmbient(float r, float g, float b, float t) {
	this->ambient[0] = r;
	this->ambient[1] = g;
	this->ambient[2] = b;
	this->ambient[3] = t;
}

void Icosahedron::setDiffuse(float r, float g, float b, float t) {
	this->diffuse[0] = r;
	this->diffuse[1] = g;
	this->diffuse[2] = b;
	this->diffuse[3] = t;
}

void Icosahedron::setSpecular(float r, float g, float b, float t) {
	this->specular[0] = r;
	this->specular[1] = g;
	this->specular[2] = b;
	this->specular[3] = t;
}

void Icosahedron::setShininess(int shininess) {
	this->shininess = shininess;
}

void Icosahedron::apply_material() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
	glMaterialiv(GL_FRONT, GL_SHININESS, &this->shininess);
}

void Icosahedron::subdivide(Vector3f v1, Vector3f v2, Vector3f v3, long depth, bool v_normals) {
	if (depth == 0) { draw_triangle(v1, v2, v3, v_normals); return; }

	Vector3f new_v1 = (v1 + v2) / 2.0f;
	Vector3f new_v2 = (v2 + v3) / 2.0f;
	Vector3f new_v3 = (v3 + v1) / 2.0f;

	Vector3f n_v1 = Vector3f::normalize(new_v1);
	Vector3f n_v2 = Vector3f::normalize(new_v2);
	Vector3f n_v3 = Vector3f::normalize(new_v3);

	subdivide(v1, n_v1, n_v3, depth - 1, v_normals);
	subdivide(v2, n_v2, n_v1, depth - 1, v_normals);
	subdivide(v3, n_v3, n_v2, depth - 1, v_normals);
	subdivide(n_v1, n_v2, n_v3, depth - 1, v_normals);
}

void Icosahedron::draw_triangle(Vector3f v1, Vector3f v2, Vector3f v3, bool v_normals) {
	if (v_normals == true) {
		glBegin(GL_TRIANGLES);
		glNormal3fv(v1);
		glVertex3fv(v1);
		glNormal3fv(v2);
		glVertex3fv(v2);
		glNormal3fv(v3);
		glVertex3fv(v3);
		glEnd();
	}
	else {
		Vector3f normal = this->face_normal(v1, v2, v3);
		glBegin(GL_TRIANGLES);
		glNormal3fv(normal);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glEnd();
	}
}

Vector3f Icosahedron::face_normal(Vector3f v1, Vector3f v2, Vector3f v3) {
	Vector3f b = v2 - v1;
	Vector3f a = v3 - v1;
	Vector3f perp = Vector3f::cross(a, b);
	Vector3f normal = Vector3f::normalize(perp);
	return normal;
}