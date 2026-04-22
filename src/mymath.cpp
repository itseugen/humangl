#include "mymath.hpp"

/**
 * This creates the identy matrix and then applies different effects
 * to apply i later use my actual matrix multiplied by these effect matrixes
 */

/**
 * @brief Creates an 4x4 identity matrix (1 on the main diagonal, 0 everywhere else)
 * @return returns the identity matrix
 */
Mat4	mat4_identity()
{
	Mat4 I;
	for (int i = 0; i < 16; ++i)
	{
		I.m[i] = 0.0f;
	}
	I.m[0] = I.m[5] = I.m[10] = I.m[15] = 1.0f;
	return I;
}

/**
 * @brief Multiplies two matrixes (coloumn major order)
 * @param a 
 * @param b 
 * @return The resulting matrix after multiplication
 */
Mat4	mat4_mul(const Mat4 &a, const Mat4 &b)
{
	Mat4 r;

	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			float sum = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				sum += a.m[k*4 + row] * b.m[col*4 + k];
			}
			r.m[col*4 + row] = sum;
		}
	}
	return r;
}

/**
 * @brief Translation Matrix in the last column according to OPENGL
 * @param x 
 * @param y 
 * @param z 
 * @return Returns the translated matrix
 */
Mat4	translate(float x, float y, float z)
{
	Mat4 r = mat4_identity();
	r.m[12] = x;
	r.m[13] = y;
	r.m[14] = z;
	return r;
}

/**
 * @brief Rotation around the Y axis
 * @param angle in radians
 * @return Returns the rotation matrix
 */
Mat4	rotate_y(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	Mat4 r = mat4_identity();
	r.m[0] = c;
	r.m[2] = -s;
	r.m[8] = s;
	r.m[10] = c;
	return r;
}

Mat4 rotate_z(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	Mat4 r = mat4_identity();
	r.m[0] = c;
	r.m[1] = -s;
	r.m[4] = s;
	r.m[5] = c;
	return r;
}

/**
 * @brief Gives the 3D scene depth
 * @param fovyRad Vertical fov in rad
 * @param aspect width/height of the viewport
 * @param zn near clipping plane (anything close than zn is clipped)
 * @param zf far clipping plane (anything further than zf is clipped)
 * @return The perspective matrix
 */
Mat4	perspective (float fovyRad, float aspect, float zn, float zf)
{
	float f = 1.0f / tanf(fovyRad * 0.5f);
	Mat4 r;

	for (int i = 0; i < 16; ++i)
	{
		r.m[i] = 0.0f;
	}
	r.m[0] = f / aspect;
	r.m[5] = f;
	r.m[10] = (zf + zn) / (zn - zf);
	r.m[11] = -1.0f;
	r.m[14] = (2.0f * zf * zn) / (zn - zf);
	return r;
}

/**
 * @brief Creates the camera position
 * @param position The camera
 * @param target The target
 * @param up The up vector
 * @return The view matrix
 */
Mat4	lookAt(const Vec3 &position, const Vec3 &target, const Vec3 &up)
{
	// Camera to target pointer
	Vec3 f = norm(sub(target, position));
	// Perpendiculat ti f and up -> right direction
	Vec3 s = norm(cross(f, up));
	// perpendicular f and s -> camera up
	Vec3 u = cross(s, f);

	Mat4 r = mat4_identity();
	r.m[0] = s.x; r.m[4] = s.y; r.m[8] = s.z;
	r.m[1] = u.x; r.m[5] = u.y; r.m[9] = u.z;
	r.m[2] = -f.x; r.m[6] = -f.y; r.m[10] = -f.z;
	r.m[12] = -dot(s, position);
	r.m[13] = -dot(u, position);
	r.m[14] = dot(f, position);
	return r;
}

Vec3	sub(Vec3 a, Vec3 b)
{
	return Vec3{a.x-b.x,a.y-b.y,a.z-b.z};
};

Vec3	norm(Vec3 v)
{
	float L = sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
	return Vec3{v.x/L,v.y/L,v.z/L};
};

Vec3	cross(Vec3 a, Vec3 b)
{
	return Vec3{a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
};

float	dot(Vec3 a, Vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
};

Vec3	Vec3::operator*(float scalar) const
{
	return Vec3{x * scalar, y * scalar, z * scalar};
}

Vec3	Vec3::operator+(const Vec3& other) const
{
	return Vec3{x + other.x, y + other.y, z + other.z};
}

Vec3	Vec3::operator-(const Vec3& other) const
{
	return Vec3{x - other.x, y - other.y, z - other.z};
}

Vec3	Vec3::operator+=(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3	Vec3::operator-=(const Vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3::Vec3() {}

Vec3::Vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}

Mat4	mat4_scale(Vec3 s)
{
	Mat4 r = mat4_identity();
	r.m[0] = s.x;
	r.m[5] = s.y;
	r.m[10] = s.z;
	return r;
}

Mat4	operator*(const Mat4& a, const Mat4& b)
{
	return mat4_mul(a, b);
}
