#ifndef __ICOSAHEDRON__
#define __ICOSAHEDRON__

#include <cmath>
#include "Vector3f.h"

/** Definition of the Golden Ratio (1+-Sqrt(5))/2.0f */
static double phi = (1.0 + sqrt(5.0)) / 2.0;

/** Length of the golden rectangle */
static double icosahedron_half_length = 1.0f;

/**
 * Angle used to calculate the vector of length 1.0 so all points of the
 * icosahedron will be a distance 1.0 away from the origin.
 */
static double golden_ratio_angle = atan(phi / icosahedron_half_length);

/** Radius of all points that form the icosahedron. */
static double icosahedron_radius = 1.0;

/**
 * The W_idth and L_ength of each rectangle used to define the initial points
 * of the icosahedron. All of these points are at a distance 1.0 from the origin.
 */
static double W = icosahedron_radius * std::cos(golden_ratio_angle);
static double L = icosahedron_radius * std::sin(golden_ratio_angle);

/**
 * The 12 initial points that are used to define the icosahedron. These points
 * form three rectangles on the xy plane, yz plane, and the xz plane. Each point
 * in each rectangle has a distance 1.0 from the origin.
 */
const Vector3f icosahedron[12] = {
	Vector3f(-W, 0, L), Vector3f(W, 0, L), Vector3f(-W, 0, -L), Vector3f(W, 0, -L),
	Vector3f(0, L, W), Vector3f(0, L, -W), Vector3f(0, -L, W), Vector3f(0, -L, -W),
	Vector3f(L, W, 0), Vector3f(-L, W, 0), Vector3f(L, -W, 0), Vector3f(-L, -W, 0)
};

/** The indices that are used to draw the icosahedron from its initail vertices. */
const int tindices[20][3] = {
   {0,4,1},	 {0,9,4},  {9,5,4},  {4,5,8},  {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8},  {5,2,3},  {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5},  {7,2,11}
};

/**
 * The Icosahedron class provides a simple interface for creating subdivided
 * icosahedrons and rendering them using OpenGL display lists.
 * To use this class:
 *		- The constructor can be called from any location.
 *		- The function subdivide(..) can only be called after an OpenGL context
 *		  has been established because OpenGL display lists are used in this function.
 *		- The function render(..) can only be called with an OpenGL context.
 */
class Icosahedron {
public:
	/**
	 * Creates an icosahedron. Note that at this state the icosahedron cannot
	 * be rendered. An icosahedron can only be subdivided and then rendered
	 * after an OpenGL context is established.
	 */
	Icosahedron();
	~Icosahedron();

	/**
	 * Subdivides the icosahedron to the desired division level. This function
	 * can only be called when there is an OpenGL context established because
	 * the vertices and normals are compiled into an OpenGL display list.
	 */
	void subdivide(int division_level);

	/**
	 * Renders the icosahedron. If smooth is true then the icosahedron uses
	 * it's display list that contains the normals at on the vertices. If smooth
	 * is false then the icosahedron uses it's display list that contains the
	 * normals on the faces.
	 */
	void render(bool smooth);

	/** Sets the visibility of the icosahedron. */
	void setVisible(bool visible);

	/** Sets the ambient color of the material associated with this icosahedron. */
	void setAmbient(float r, float g, float b, float t);

	/** Sets the diffuse color of the material associated with this icosahedron. */
	void setDiffuse(float r, float g, float b, float t);

	/** Sets the specular color of the material associated with this icosahedron. */
	void setSpecular(float r, float g, float b, float t);

	/** Sets the shininess of the material associated with this icosahedron. */
	void setShininess(int shininess);

private:
	const static int ISO_FACE_COUNT = 20;
	const static long DEFAULT_SUBDIVISION = 0;

	unsigned int flat_list_index;
	unsigned int smooth_list_index;
	bool visible;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	int shininess;

	/** Applies the material associated with this icosahedron. */
	void apply_material();

	/**
	 * Subdivides the base icosahedron defition to the depth provided.
	 * The triangle to subdivide is defined by the three points v1, v2, and v3.
	 * If v_normals is true then the normals are calculated at the vertices
	 * otherwise the normal of the face is calculated.
	 */
	void subdivide(Vector3f v1, Vector3f v2, Vector3f v3, long depth, bool v_normals);

	/** Draws a single triangle and the normal(s) depending on if vertex_normals is true or false. */
	void draw_triangle(Vector3f v1, Vector3f v2, Vector3f v3, bool v_normals);

	/** Calculates the normal of the triangle provided by the points v1, v2, and v3. */
	Vector3f face_normal(Vector3f v1, Vector3f v2, Vector3f v3);
};

#endif