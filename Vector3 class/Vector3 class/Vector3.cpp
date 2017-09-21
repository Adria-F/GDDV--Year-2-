#include "Vector3.h"
#include <math.h>

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float num) const
{
	return Vector3(x * num, y * num, z * num);
}

Vector3 Vector3::operator/(float num) const
{
	return Vector3(x / num, y / num, z / num);
}

Vector3 Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector3 Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vector3 Vector3::operator*=(float num)
{
	x *= num;
	y *= num;
	z *= num;

	return *this;
}

Vector3 Vector3::operator/=(float num)
{
	x /= num;
	y /= num;
	z /= num;

	return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3::operator!=(const Vector3& other) const
{
	return (x != other.x || y != other.y || z != other.z);
}

float Vector3::module() const
{
	return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
}

void Vector3::normalize()
{
	*this /= sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
}

void Vector3::zero()
{	
	x = 0;
	y = 0;
	z = 0;
}

bool Vector3::isZero() const
{
	return (x == 0 && y == 0 && z == 0);
}

float Vector3::distanceTo(const Vector3& other) const
{
	Vector3 distance = *this - other;

	return sqrt(pow(distance.x, 2.0) + pow(distance.y, 2.0) + pow(distance.z, 2.0));
}