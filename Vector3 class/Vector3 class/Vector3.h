#ifndef __VECTOR3_H__
#define __VECTOR3_H__

class Vector3
{
public:

	float x, y, z;

public:

	Vector3(): x(0), y(0), z(0)
	{}

	Vector3(float _x, float _y, float _z): x(_x), y(_y), z(_z)
	{}

	~Vector3()
	{}

	Vector3 operator+(const Vector3 other) const;
	Vector3 operator-(const Vector3 other) const;
	Vector3 operator*(float num) const;
	Vector3 operator/(float num) const;

	Vector3 operator+=(const Vector3 other);
	Vector3 operator-=(const Vector3 other);
	Vector3 operator*=(float num);
	Vector3 operator/=(float num);

	bool operator==(const Vector3 other) const;
	bool operator!=(const Vector3 other) const;

	float module() const;
	void normalize();
	void zero();
	bool isZero() const;
	float distanceTo(Vector3 other) const;

};

#endif // !__VECTOR3_H__
