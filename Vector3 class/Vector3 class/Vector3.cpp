#include "Vector3.h"
#include <math.h>

Vector3 Vector3::operator+(const Vector3 other) const
{
	return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator-(const Vector3 other) const
{
	return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3 Vector3::operator*(float num) const
{
	return Vector3(this->x * num, this->y * num, this->z * num);
}

Vector3 Vector3::operator/(float num) const
{
	return Vector3(this->x / num, this->y / num, this->z / num);
}

Vector3 Vector3::operator+=(const Vector3 other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return *this;
}

Vector3 Vector3::operator-=(const Vector3 other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return *this;
}

Vector3 Vector3::operator*=(float num)
{
	this->x *= num;
	this->y *= num;
	this->z *= num;

	return *this;
}

Vector3 Vector3::operator/=(float num)
{
	this->x /= num;
	this->y /= num;
	this->z /= num;

	return *this;
}

bool Vector3::operator==(const Vector3 other) const
{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

bool Vector3::operator!=(const Vector3 other) const
{
	return (this->x != other.x || this->y != other.y || this->z != other.z);
}

float Vector3::module() const
{
	return sqrt(pow(this->x, 2.0) + pow(this->y, 2.0) + pow(this->z, 2.0));
}

void Vector3::normalize()
{
	*this /= this->module();
}

void Vector3::zero()
{	
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

bool Vector3::isZero() const
{
	return (this->x == 0 && this->y == 0 && this->z == 0);
}

float Vector3::distanceTo(Vector3 other) const
{
	Vector3 distance = *this - other;

	return distance.module();
}