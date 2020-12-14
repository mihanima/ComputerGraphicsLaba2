
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <GL\GL.h>
#include <GL\GLU.h>

using namespace std;

class Point {
public:
	double x;
	double y;
	double z;
	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Point(const Point& A) {
		this->x = A.x;
		this->y = A.y;
		this->z = A.z;
	}
	void DrawPoint() {
		glVertex3d(this->x, this->y, this->z);
	}
	void Normal3d() {
		glNormal3d(x, y, z);
	}
	~Point() {

	}
	Point operator + (Point A) {
		A.x = A.x + this->x;
		A.y = A.y + this->y;
		A.z = A.z + this->z;
		return A;
	}
}; 
