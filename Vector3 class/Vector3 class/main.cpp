#include <iostream>
#include "Vector3.h"
using namespace std;

void printVec(Vector3 vec, int num)
{
	cout << "Vector "<< num << ":" << endl;
	cout << "	X: " << vec.x << endl;
	cout << "	Y: " << vec.y << endl;
	cout << "	Z: " << vec.z << endl;
}

int main()
{
	Vector3 vec1(1, 2, 3);
	Vector3 vec2(4, 6, 8);

	Vector3 vec3 = vec1 + vec2; // (5, 8, 11)
	Vector3 vec4 = vec1 - vec2; // (-3, -4, - 5)
	Vector3 vec5 = vec1 * 2; // (2, 4, 6)
	Vector3 vec6 = vec2 / 2; // (2, 3, 4)

	printVec(vec1, 1);
	printVec(vec2, 2);
	printVec(vec3, 3);
	printVec(vec4, 4);
	printVec(vec5, 5);
	printVec(vec6, 6);

	cout << "-------------------" << endl;

	vec3 += vec4; // (2, 4, 6)
	vec4 -= vec6; // (-5, -7, -9)
	
	printVec(vec3, 3);
	printVec(vec4, 4);

	cout << "-------------------" << endl;

	cout << "Is vec3 equal to vec5 ?" << endl;
	cout << "	" << ((vec3 == vec5) ? "Yes" : "No") << endl;

	cout << "Is vec3 different than vec6 ?" << endl;
	cout << "	" << ((vec3 != vec6) ? "Yes" : "No") << endl;

	cout << "-------------------" << endl;

	cout << "The ";
	printVec(vec3, 3);
	cout << "normalized is:" << endl;
	vec3.normalize();
	printVec(vec3, 3);
	cout << "as it's module is: " << vec3.module() << endl;

	cout << "-------------------" << endl;

	vec3.zero();
	printVec(vec3, 3);
	cout << "Is vec3 0?" << endl;
	cout << "	" << ((vec3.isZero()) ? "Yes" : "No") << endl;

	cout << "-------------------" << endl;

	cout << "The distance between vec3 and vec5 is: " << vec3.distanceTo(vec5) << endl; // 7.48
	cout << "The distance between vec4 and vec6 is: " << vec4.distanceTo(vec6) << endl; // 17.83

	cout << "-------------------" << endl;

	system("pause");
	return 0;
}