#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

// Vector for positions and directions
struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float x_, float y_, float z_);
	Vec3	operator+(const Vec3& other) const;
	Vec3	operator-(const Vec3& other) const;
	Vec3	operator-() const;
	Vec3	operator+=(const Vec3& other);
	Vec3	operator-=(const Vec3& other);
	Vec3	operator*(float scalar) const;
};

// Vector + colour
struct Vertex
{
	float x, y, z;
	float r, g, b;
	float u, v; // Texture coordinates
};

// Graphics matrix (translation, rotation, scaling, projection, camera transforms)
/**
 * @brief 4x4 matrix
 * Matrix looks like:
 * 0 4 8 12;
 * 1 5 9 13;
 * 2 6 10 14;
 * 3 7 11 15;
 */
struct Mat4
{
	float m[16];
};

// A clean matrix that doesnt change anything when applied
Mat4	mat4_identity();

// Matrix multiplication (column-major order)
Mat4	mat4_mul(const Mat4 &a, const Mat4 &b);

Mat4	operator*(const Mat4& a, const Mat4& b);

// Produces a matrix that moves by xyz in space
Mat4	translate(float x, float y, float z);
Mat4	translate(const Vec3 &v);

// spinning spinning spinning
Mat4	rotate_x(float angle);
Mat4	rotate_y(float angle);
Mat4	rotate_z(float angle);

// depth projection and stuff
Mat4	perspective (float fovyRad, float aspect, float zn, float zf);

// LookAt matrix: camera position, target, up vector
Mat4	lookAt(const Vec3 &position, const Vec3 &target, const Vec3 &up);

// Scale by x, y, z
Mat4	mat4_scale(Vec3 s);

Vec3	sub(Vec3 a, Vec3 b);
Vec3	norm(Vec3 v);
Vec3	cross(Vec3 a, Vec3 b);
float	dot(Vec3 a, Vec3 b);

float	myClamp(float value, float min, float max);