#include "QViewport.h"
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include "teapot.h"
#include <cstdlib>
#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

extern "C" {
#include "tga.h"
}

#ifndef GL_EXT_texture_cube_map
# define GL_NORMAL_MAP_EXT                   0x8511
# define GL_REFLECTION_MAP_EXT               0x8512
# define GL_TEXTURE_CUBE_MAP_EXT             0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP_EXT     0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT  0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT  0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT  0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT  0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT  0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT  0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP_EXT       0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT    0x851C
#endif

static GLenum targets[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};
bool mipmaps = true;
bool cube_map_enabled = true;
int cube_map_mode = GL_NORMAL_MAP_EXT;
int wrap = GL_REPEAT;

char *cube_images[6] = {
  "cm_left.tga",
  "cm_right.tga",
  "cm_top.tga",
  "cm_bottom.tga",
  "cm_back.tga",
  "cm_front.tga",
};
char *snow_images[6] = {
  "n_L.tga",
  "p_R.tga",
  "p_U.tga",
  "n_D.tga",
  "n_B.tga",
  "p_F.tga",
};


void g_update_texgen() {
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, cube_map_mode);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, cube_map_mode);
}

void g_update_wrap() {
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, wrap);
}


void g_load_texture(GLenum target, char* filename) {
	FILE* file;
	gliGenericImage* image;

	file = fopen(filename, "rb");
	if (file == NULL) exit(1);

	image = gliReadTGA(file, filename);
	fclose(file);

	if (mipmaps == true)
		gluBuild2DMipmaps(target, image->components, image->width, image->height, image->format, GL_UNSIGNED_BYTE, image->pixels);
	else
		glTexImage2D(target, 0, image->components, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->pixels);
}

void g_construct_cube_map() {
	for (int i = 0; i < 6; i++)
		g_load_texture(targets[i], snow_images[i]);

	if (mipmaps == true) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glEnable(GL_TEXTURE_CUBE_MAP_EXT);

	g_update_texgen();
	g_update_wrap();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
}
static size_t windowWidth = 640;
static size_t windowHeight = 640;

void cameraOrientation(Camera &XYZ, Camera &RTP)
{
	// We set the camera's XYZ coordinates using pythagorean identities
	// so that its orientation is correctly looking at the teapot after we rotate it

	XYZ.x = RTP.z * sinf(RTP.x) * sinf(RTP.y);
	XYZ.z = RTP.z * -cosf(RTP.x) * sinf(RTP.y);
	XYZ.y = RTP.z * -cosf(RTP.y);
}


float angle_light_0 = 0.0f;
float light_0_dist = 3.0f;
GLfloat light_0_pos[] = { light_0_dist * cos(angle_light_0), 0.0f, light_0_dist * sin(angle_light_0), 0.0f };
GLfloat light_0_ambient[] = { 0.1f, 0.2f, 0.4f, 1.0f };
GLfloat light_0_diffuse[] = { 0.1f, 0.2f, 0.8f, 1.0f };
GLfloat light_0_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };

/**
 * Light 1 Information
 */
float angle_light_1 = 9.6f;
float light_1_dist = 3.0f;
GLfloat light_1_pos[] = { light_1_dist * cos(angle_light_1), 0.0f, light_1_dist * sin(angle_light_1), 0.0f };
GLfloat light_1_ambient[] = { 0.4f, 0.2f, 0.1f, 1.0f };
GLfloat light_1_diffuse[] = { 0.8f, 0.2f, 0.1f, 1.0f };
GLfloat light_1_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };

bool lighting = true;
bool light_0_enabled = false;
bool light_1_enabled = false;

/**
 * Set the parameters of the lights used in the scene.
 */
void g_create_lights() {
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);

	glLightfv(GL_LIGHT1, GL_POSITION, light_1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_1_specular);
}

void g_draw_lights() {
	if (lighting == false) return;
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glDisable(GL_LIGHTING);
	glPointSize(6.0f);
	glBegin(GL_POINTS);
	glColor3f(light_0_diffuse[0], light_0_diffuse[1], light_0_diffuse[2]);
	//glVertex3f(light_0_pos[0], light_0_pos[1], light_0_pos[2]);
	glVertex3f(-5, 5, 0);
	glColor3f(light_1_diffuse[0], light_1_diffuse[1], light_1_diffuse[2]);
	glVertex3f(light_1_pos[0], light_1_pos[1], light_1_pos[2]);
	glEnd();
	glEnable(GL_LIGHTING);
	if (cube_map_enabled)
		glEnable(GL_TEXTURE_CUBE_MAP_EXT);
}

void QViewport::drawIcos() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	this->icos.subdivide(this->subdivision);
	this->icos.render(true);
	glPopMatrix();
}

QViewport::QViewport(QWidget* parent) : QOpenGLWidget(parent) {
	cameraRTP = Camera(0, 2, 20.0);
	cameraXYZ = Camera(-10, 0, 0);
	cameraOrientation(cameraXYZ, cameraRTP);
	renderTeapot = false;
	renderIcos = true;
	renderCubeMap = true;
	light0 = false;
	light1 = false;
	light2 = true;
	subdivision = 0;
}

QViewport::~QViewport()
{
}


void QViewport::initializeGL() {
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	g_construct_cube_map();
	g_create_lights();
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_AUTO_NORMAL);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 2.0 };
	GLfloat light_diffuse[] = { 2.0, 2.0, 2.0, 2.0 };
	GLfloat light_specular[] = { 2.0, 2.0, 2.0, 2.0 };
	GLfloat light_position[] = { 1.0, 1.0, 5.0, 0.0 };
	GLfloat global_ambient[] = { 0.75, 0.75, 0.75, 1.0 };


	
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glFrontFace(GL_CW);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	g_draw_lights();
}

void QViewport::paintGL() {
	glPushMatrix();
	this->toggleCubeMap();
	glPopMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//Set coordinates for the clipping plane
	gluOrtho2D(0, 1, 0, 1);

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glBegin(GL_QUADS);
	glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
	glEnd();

	// Define the color of the background
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	// Define background parameters
	glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
	glEnd();

	glMatrixMode(GL_PROJECTION);    glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	// Set gluLookAt to match the initial camera position
	gluLookAt(cameraXYZ.x, cameraXYZ.y, cameraXYZ.z,
		0, 0, 0,
		0, 1, 0);
	// Call our display function to draw the teapot and the grid (when applicable)
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glEnd();
	if (renderTeapot) {
		glPushMatrix();
		Draw();
		Teapot(1);
		glPopMatrix();
		this->update();
	}
	
	if (renderIcos) {
		this->drawIcos();
	}
	//glDisable(GL_LIGHTING);
	glPopMatrix();
	
	
	this->lightSettings();
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHTING);
	//glutSwapBuffers();
}

void QViewport::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat)w / (GLfloat)h, 1, 30.0);
	glMatrixMode(GL_MODELVIEW);
}

void QViewport::mouseMoveEvent(QMouseEvent *event)
{
	// Define motion with the mouse
	if (event->buttons() & Qt::LeftButton) {
		// define rate of rotational change and edit spherical coordinates
		
		cameraRTP.x += (event->x() - lastPos.x()) * 0.01; 
		
		cameraRTP.y += (event->y() - lastPos.y()) * 0.01;

		// set the limit of the vertical rotation to +-90 degrees
		if (cameraRTP.y <= 0) {
			cameraRTP.y = 0 + 0.001;
		}
		if (cameraRTP.y >= M_PI) {
			cameraRTP.y = M_PI - 0.001;
		}

	}
	cameraOrientation(cameraXYZ, cameraRTP);
	lastPos = event->pos();
	this->update();
	
}

void QViewport::lightSettings()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHT2);
	if (this->light0) {
		glEnable(GL_LIGHT0);
	}
	if (this->light1) {
		glEnable(GL_LIGHT1);
	}

	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
	this->update();
}

void QViewport::toggleTeapot() {
	this->renderTeapot = true;
	this->renderIcos = false;
}

void QViewport::toggleIcos()
{
	this->renderIcos = true;
	this->renderTeapot = false;
}

void QViewport::toggleCubeMap()
{
	glDisable(GL_TEXTURE_CUBE_MAP);
	if (renderCubeMap) {
		glEnable(GL_TEXTURE_CUBE_MAP);
	}
}
