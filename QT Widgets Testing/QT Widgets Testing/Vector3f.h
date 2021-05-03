
#ifndef __VECTOR3F__
#define __VECTOR3F__

class Vector3f {
public:
	/** Creates a default zero vector (0.0, 0.0, 0.0) */
	Vector3f();

	/** Creates a vector from the provide values (x, y, z) */
	Vector3f(float x, float y, float z);

	/** Returns the x component of this vector. */
	inline float x() { return this->data[0]; }

	/** Returns the y component of this vector. */
	inline float y() { return this->data[1]; }

	/** Returns the z component of this vector. */
	inline float z() { return this->data[2]; }

	/** Sets the x component of this vector to the value provided. */
	inline void setX(float x) { this->data[0] = x; }

	/** Sets the y component of this vector to the value provided. */
	inline void setY(float y) { this->data[1] = y; }

	/** Sets the z component of this vector to the value provided. */
	inline void setZ(float z) { this->data[2] = z; }

	/**
	 * Returns the float array representation of this vector. Useful for
	 * OpenGL calls; use carefully.
	 */
	operator float* () { return this->data; }

	/** Utility Function: Calculates the dot product between the two provided vectors. */
	static float dot(const Vector3f& a, const Vector3f& b);

	/**  Utility Function: Calculates the length of the provided vectors. */
	static float length(const Vector3f& vector);

	/** Utility Function: Normalizes the provided vector. */
	static Vector3f normalize(const Vector3f& vector);

	/** Utility Function: Calculates the cross product between the two provided vectors. */
	static Vector3f cross(const Vector3f& a, const Vector3f& b);

	/** Overloaded subtraction operator to allow the direct subtraction of vectors. */
	Vector3f operator - (const Vector3f& vector);

	/** Overloaded addition operator to allow the direct addition of vectors. */
	Vector3f operator + (const Vector3f& vector);

	/**
	 * Overloaded scalar division operator to allow for the direct division of
	 * the vector by the provided scalar.
	 */
	Vector3f operator / (const float& scalar);

	/**
	 * Overloaded scalar multiplication operator to allow for the direct
	 * multiplication of the vector by the provided scalar.
	 */
	Vector3f operator * (const float& scalar);

private:
	/**
	 * The internal x, y, and z components of the vector are stored in a
	 * float array such that:
	 *		data[0] = x component
	 *		data[1] = y component
	 *		data[2] = z component
	 */
	float data[3];
};

#endif