#include "vector.h"
#include <cmath>

Vector::Vector () {
  x = 0; y = 0; z = 0;
}

Vector::Vector (float xVal, float yVal, float zVal) {
  x = xVal; y = yVal; z = zVal;
}

float Vector::norm () {
  return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

void Vector::normalize () {
  float l2norm = this->norm();
  x = x / l2norm; y = y / l2norm; z = z / l2norm;
}

Vector Vector::cross(Vector& vec2) { 
	return Vector(y * vec2.z - z * vec2.y, z * vec2.x - x * vec2.z, x  * vec2.y - y * vec2.x); 
}

float Vector::dot(Vector& vec2) { 
	return x * vec2.x + y * vec2.y + z * vec2.z;
}

Vector Vector::operator - () {
	return Vector(-x, -y, -z);
}

Vector Vector::operator + (const Vector& vec2) {
	return Vector(x + vec2.x, y + vec2.y, z + vec2.z);
}

Vector& Vector::operator += (const Vector& vec2) {
	x += vec2.x; y += vec2.y; z += vec2.z;
	return *this;
}

Vector Vector::operator - (const Vector& vec2) {
	return Vector(x - vec2.x, y - vec2.y, z - vec2.z);
}

Vector& Vector::operator -= (const Vector& vec2) {
	x -= vec2.x; y -= vec2.y; z -= vec2.z;
	return *this;
}

Vector Vector::operator * (const float c) {
	return Vector(x * c, y * c, z * c);
}

Vector& Vector::operator *= (const float c) {
	x *= c; y *= c; z *= c; 
	return *this;
}

Vector Vector::operator / (const float c) {
	return Vector(x / c, y / c, z / c);
}

Vector& Vector::operator /= (const float c) {
	x /= c; y /= c; z /= c; 
	return *this;
}