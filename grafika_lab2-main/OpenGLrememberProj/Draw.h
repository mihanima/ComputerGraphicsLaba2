#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include"MethodGauss.h"
#include"Help.h"




class PointXY {
public:
	double x;
	double y;
	PointXY(double x, double y) {
		this->x = x;
		this->y = y;
	}
	void TexCoord2d() {
		glTexCoord2d(x, y);
	}
};
typedef long double ld;

using namespace std;

class Draw : private Help{
private:
	//������� ��������� ����� ������� �� ��������� � ���������� �������� (������ � ������ �������� 1�1)
	//������������� (��������� ��������� ��� ���������) ��� �������� ���������� �����
	//� - ����� ����� ������� ��������� (�������� ������� ����� ���������, ����� ������� �� ������) - �������������
	//� - �����, ������� ���� ����������� � ����� ����������
	//set - ���������� ��������� ��������� - ������� ��� �������������
	//points - ����� ����� ��� ����������� ������ ��������� - ������ ��� �������������
	//r - ���������� ������� (���������� �� ������� ���� ��������� ������� ��������� �� ������ � ������) - ������ ��� �������������
	static PointXY UpdatePoint(Point A, bool set = false, vector<Point> points = { Point(0, 0, 0) }, double r = 0) {
		static bool installation = false;
		static double width = 0;
		static double height = 0;
		static double min_x, min_y, max_x, max_y;
		static PointXY new_O(0, 0);
		PointXY newCoord(2, 2);
		if (set == true) {
			//���������� ������� ������ ��� ����� ����������
			min_x = points[0].x, min_y = points[0].y, max_x = points[0].x, max_y = points[0].y;
			for (int i = 0; i < points.size(); i++) {
				if (min_x > points[i].x) {
					min_x = points[i].x;
				}
				if (max_x < points[i].x) {
					max_x = points[i].x;
				}
				if (min_y > points[i].y) {
					min_y = points[i].y;
				}
				if (max_y < points[i].y) {
					max_y = points[i].y;
				}
			}

			//������ ����� � ������ �������������� ���� ������� ���� ������
			width = abs(min_x) + abs(max_x) + r;
			height = abs(min_y) + abs(max_y) + r;

			new_O.x = A.x;
			new_O.y = A.y;

			//��������� �����������
			installation = true;

			return newCoord;
		}
		if (installation) {
			//������ ����� ��������� ������������ 
			newCoord.x = (A.x - new_O.x) / width;
			newCoord.y = (A.y - new_O.y) / height;
			return newCoord;
		}
		newCoord.x = 9;
		newCoord.y = 9;
		return newCoord;
	}
public:
	//������ �����
	static void DrawLine(Point A, Point B, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_LINES);
		}
		if (normal < 0) {
			NormalRotation(B);
		}
		A.DrawPoint();
		B.DrawPoint();
		if (flag)
		{
			glEnd();
		}
	}
	//������ ���������������
	static void DrawQuads(vector<Point> points, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_QUADS);
		}

		if (normal > 0) {
			SearchNormal(points[0], points[1], points[2]).Normal3d();
		}
		else {
			Point Normal = SearchNormal(points[0], points[1], points[2]);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		for (int i = 0; i < (int)points.size(); i++) {
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	//������ �������������
	static void DrawRectangle(Point A, Point B, double height, vector<PointXY> texCoord, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_QUADS);
		}

		Point A1 = A + Point(0, 0, height);
		Point B1 = B + Point(0, 0, height);

		if (normal > 0) {
			SearchNormal(A1, A, B).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A1, A, B);
			NormalRotation(Normal);
			Normal.Normal3d();
		}
		
		texCoord[0].TexCoord2d();
		A.DrawPoint();
		texCoord[1].TexCoord2d();
		B.DrawPoint();
		texCoord[2].TexCoord2d();
		B1.DrawPoint();
		texCoord[3].TexCoord2d();
		A1.DrawPoint();
		if (flag) {
			glEnd();
		}
	}
	//������ �������������
	static void DrawPolygon(vector<Point> points, bool flag = false) {
		if (flag) {
			glBegin(GL_POLYGON);
		}
		for (int i = 0; i < (int)points.size(); i++) {
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	//������ �����������
	static void DrawTriangles(vector<Point> points, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(points[0], points[1], points[2]).Normal3d();
		}
		else {
			Point Normal = SearchNormal(points[0], points[1], points[2]);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		for (int i = 0; i < (int)points.size(); i++) {
			UpdatePoint(points[i]).TexCoord2d();
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		UpdatePoint(A).TexCoord2d();
		glVertex3d(A.x, A.y, A.z);

		UpdatePoint(B).TexCoord2d();
		glVertex3d(B.x, B.y, B.z);

		UpdatePoint(C).TexCoord2d();
		glVertex3d(C.x, C.y, C.z);
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, double height, int normal = 1, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}

		if (normal > 0) {
			SearchNormal(A, B, C).Normal3d();
		}
		else {
			Point Normal = SearchNormal(A, B, C);
			NormalRotation(Normal);
			Normal.Normal3d();
		}

		UpdatePoint(A).TexCoord2d();
		glVertex3d(A.x, A.y, height);

		UpdatePoint(B).TexCoord2d();
		glVertex3d(B.x, B.y, height);

		UpdatePoint(C).TexCoord2d();
		glVertex3d(C.x, C.y, height);
		if (flag) {
			glEnd();
		}
	}
	//��������� ����
	static void RandomColor() {
		glColor3d((ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000);
	}
	static void Color(int i) {
		switch (i) {
		case 1:
			glColor3d(0.7, 0.6, 0.9);
			break;
		case 2:
			glColor3d(0.8, 0.3, 0.6);
			break;
		case 3:
			glColor3d(0.3, 0.8, 0.3);
			break;
		}
	}
	
public:
	//���������� ���������� ����� ���� �����
	static vector<Point> DrawConvex(Point B, Point C, double z, Point midpoint = { 0, 0, 0 }) {
		//����� ���������� � ��� ������
		midpoint = SearchMidpoint(B, C);
		double r = SearchDistancePoints(B, midpoint);

		//����� ��� �� = 0
		double x = midpoint.x + r * cos(0);
		double y = midpoint.y + r * sin(0);
		Point O(x, y, 0);

		//���� �������
		Point VectorB = SearchVector(midpoint, B);
		Point VectorO = SearchVector(midpoint, O);
		Point VectorC = SearchVector(midpoint, C);

		//���� ���� ����� ���������
		double angleOC = SearchAngleVector(VectorO, VectorC);
		double angleOB = SearchAngleVector(VectorO, VectorB);

		//������ ���
		double step = 0.1;

		//���� ����� �� ����������
		vector<Point> pointsBC;
		Point newPoint(0, 0, 0);
		
		for (double i = angleOB; i >= angleOC * (-1); i -= step) {
			newPoint.x = midpoint.x + r * cos(i * M_PI / 180);
			newPoint.y = midpoint.y + r * sin(i * M_PI / 180);
			pointsBC.push_back(newPoint);
		}

		//������ ��������������
		vector<PointXY> texCoord;
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {
			double t1 = i / (size - 1.0);
			double t2 = (i + 1.0) / (size - 1.0);
			texCoord.push_back(PointXY(t1, 0));
			texCoord.push_back(PointXY(t2, 0));
			texCoord.push_back(PointXY(t2, 1));
			texCoord.push_back(PointXY(t1, 1));

			DrawRectangle(pointsBC[i], pointsBC[i + 1], z, texCoord);

			texCoord.clear();
		}

		return pointsBC;
	}
public:
	// ���������� ��������� ����� ����� ������� � ���������� ����� ����� c ������� ��������

	static vector<Point> DrawBulge(Point D, Point M, Point C, double z, Point& Apex) {
		double x[] = { C.y, M.y, D.y };
		double y[] = { C.x, M.x, D.x };

		//����� ���������� ���������
		//���������� ������������� �������� y = a * x^2 + b * x + c
	//	Point A(0, 0, 0), M(1, 3, 0), B(-1, 6, 0);
		double matrix[3][4];

		//���������� ������� ��������� c ������� ������ ���������� ���������
		//LeastSquaresMethod(x, y, 3, matrix); 

		//����������� ����� � ��������� y = a * x^2 + b * x + c � ����������� ������� 
		for (int i = 0; i < 3; i++) {
			matrix[i][0] = pow(x[i], 2);
			matrix[i][1] = x[i];
			matrix[i][2] = 1;
			matrix[i][3] = y[i];
		}

		MethodGauss �oefficientsParabola(matrix, 3, 4);

		//������� ��������
		Apex = ApexParabola(�oefficientsParabola.decision[0], �oefficientsParabola.decision[1], �oefficientsParabola.decision[2]);

		//��������� ����� �� ���� D� ���������� ����� ����� �
		vector<Point> pointsDC;
		Point newPoint(0, 0, 0);
		for (double i = D.y; i <= C.y; i += 0.1) {
			newPoint.x = pow(i, 2) * �oefficientsParabola.decision[0] + i * �oefficientsParabola.decision[1] + �oefficientsParabola.decision[2];
			newPoint.y = i;
			pointsDC.push_back(newPoint);
		}

		//������ ���������
		//������ ���������
		vector<PointXY> texCoord;
		int size = pointsDC.size();
		for (int i = 0; i < size - 1; i++) {
			double t1 = i / (size - 1.0);
			double t2 = (i + 1.0) / (size - 1.0);
			texCoord.push_back(PointXY(t1, 0));
			texCoord.push_back(PointXY(t2, 0));
			texCoord.push_back(PointXY(t2, 1));
			texCoord.push_back(PointXY(t1, 1));

			DrawRectangle(pointsDC[i], pointsDC[i + 1], z, texCoord);

			texCoord.clear();
		}

		return pointsDC;
	}
	//���������� ��������� ����� ���� ����� � ������� ���� �����
	static vector<Point> DrawBulge(Point C, Point D, double z, Point center, double r) {
		//����� ��� �� = 0
		double x = center.x + r * cos(0);
		double y = center.y + r * sin(0);
		Point O(x, y, 0);

		//���� �������
		Point VectorD = SearchVector(center, D);
		Point VectorO = SearchVector(center, O);
		Point VectorC = SearchVector(center, C);

		//���� ���� ����� ���������
		double angleOC = SearchAngleVector(VectorO, VectorC);
		double angleOD = SearchAngleVector(VectorO, VectorD);

		//������ ���
		double step = 0.1;

		//���� ����� �� ����������
		vector<Point> pointsCD;
		Point newPoint(0, 0, 0);

		pointsCD.push_back(C);

		for (double i = angleOC; i >= angleOD; i -= step) {
			newPoint.x = center.x + r * cos(i * M_PI / 180);
			newPoint.y = center.y + r * sin(i * M_PI / 180);
			pointsCD.push_back(newPoint);
		}
		pointsCD.push_back(D);

		int size = pointsCD.size();
		//������ ��������������
		vector<PointXY> texCoord;
		for (int i = 0; i < size - 1; i++) {
			double t1 = i / (size - 1.0);
			double t2 = (i + 1.0) / (size - 1.0);
			texCoord.push_back(PointXY(t1, 0));
			texCoord.push_back(PointXY(t2, 0));
			texCoord.push_back(PointXY(t2, 1));
			texCoord.push_back(PointXY(t1, 1));

			DrawRectangle(pointsCD[i], pointsCD[i + 1], z, texCoord, -1);

			texCoord.clear();
		}

		return pointsCD;
	}

	//�������� ��� � ����, ����� ������� ����� (��������� - ��������)
	static void DrawTopBottom(Point A, Point B, Point C, Point D, Point E, Point F, Point G, Point H, double z) {
		glColor3d(0.6, 0.8, 0.3);
		DrawTriangles(H, B, C);
		DrawTriangles(H, B, C, z);

		glColor3d(0.1, 0.2, 0.3);
		DrawTriangles(H, C, F);
		DrawTriangles(H, C, F, z);

		glColor3d(0.1, 0.2, 0.6);
		DrawTriangles(H, G, F);
		DrawTriangles(H, G, F, z);

		glColor3d(0.5, 0.8, 0.3);
		DrawTriangles(C, D, F);
		DrawTriangles(C, D, F, z);

		glColor3d(0.1, 0.6, 0.3);
		DrawTriangles(E, D, F);
		DrawTriangles(E, D, F, z);
	}
	//����������� ���
	static void DrawBottom(Point A, Point B, Point C, Point D, Point E, Point F, Point G, Point H, vector<Point> pointsBC, vector<Point> pointsCD) {
		glBegin(GL_TRIANGLES);
		DrawTriangles(H, B, C, -1);
		DrawTriangles(H, C, F, -1);
		DrawTriangles(G, H, F, -1);
		DrawTriangles(C, D, F, -1);
		DrawTriangles(F, D, E, -1);

		//����������
		Point midPoint = SearchMidpoint( E, D);
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {

			DrawTriangles(pointsBC[i], midPoint, pointsBC[i + 1]);
		}

		//����������
		size = pointsCD.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles( H, pointsCD[i], pointsCD[i + 1], -1);
		}
		glEnd();
	}
	//����������� �����
	static void DrawTop(Point A, Point B, Point C, Point D, Point E, Point F, Point G,Point H, vector<Point> pointsBC, vector<Point> pointsCD, double z, int alpha) {
		
		//����������� ����� ���������
		switch (alpha) {
		case 1:
			//�������� ����� ����������
			glEnable(GL_BLEND);
			//������ ����� ��� ������������� ��������� � ���������
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case 2:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4d(0.7, 0.1, 0.1, 0.6);
			break;
		}

		glBegin(GL_TRIANGLES);

		DrawTriangles(H, B, C, z);
		DrawTriangles(H, C, F, z);
		DrawTriangles(G, H, F, z);
		DrawTriangles(C, D, F, z);
		DrawTriangles(F, D, E, z);

		//����������
		Point midPoint = SearchMidpoint(E,D);
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles(pointsBC[i], midPoint, pointsBC[i + 1], z, -1);
		}

		//����������
		size = pointsCD.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles(H, pointsCD[i], pointsCD[i + 1], z);
		}

		glEnd();

		//��������� ����������
		if (alpha != 0) {
			glDisable(GL_BLEND);
		}
	}
	//����������� ��������� (��������� - ��������)
	static void DrawTopBottomConvex(Point B, Point C, vector<Point> pointsBC, double z) {
		Point midPoint = SearchMidpoint(B, C);
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles(midPoint, pointsBC[i], pointsBC[i + 1]);
			DrawTriangles(midPoint, pointsBC[i], pointsBC[i + 1], z);
		}
	}

	static void DrawTopBottomBulge(Point H, vector<Point> pointsDC, double z) {
		int size = pointsDC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles(H, pointsDC[i], pointsDC[i + 1]);
			DrawTriangles(H, pointsDC[i], pointsDC[i + 1], z);
		}
	}

	static void DrawABCDEFG(int alpha, bool texChange, GLuint texId[]) {

		Point A(0, 0, 0), B(-1, 6, 0), C(3, 5, 0), D(1, 9, 0), E(10, 5, 0), F(7, 5, 0), G(9, -1, 0), H(5, 3, 0), M(1, 3, 0);
		double z = 2;
		Point A1(0, 0, z), B1(-1, 6,z), C1(3, 5, z), D1(1, 9, z), E1(10, 5,z), F1(7, 5, z), G1(9, -1, z), H1(5, 3, z), M1(1, 3, z);

		//����� ��������
		if (texChange) {
			glBindTexture(GL_TEXTURE_2D, texId[0]);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texId[1]);
		}

		//�����
		glBegin(GL_QUADS);
		//glColor3d(0.1, 0.2, 0.3);
		glColor3d(1, 29/256, 0/256);
		vector<PointXY> texCoord = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
		Draw::DrawRectangle(B, C, z, texCoord);
		Draw::DrawRectangle(C, D, z, texCoord);
		Draw::DrawRectangle(E, F, z, texCoord);
		Draw::DrawRectangle(F, G, z, texCoord);
		Draw::DrawRectangle(G, H, z, texCoord);
		Draw::DrawRectangle(H, A, z, texCoord);

		//����������
		vector<Point> pointsBC = Draw::DrawConvex(D, E, z);
		//until here
		//���������
		Point Center(0, 0, 0);
		double r = 0;
		Point ApexParabola(0, 0, 0);
		vector<Point> pointsCD = Draw::DrawBulge(A, M, B, z, ApexParabola);
		glEnd();
		
		vector<Point> points = { A, B, C, D, E, F, G, H };
		//���������� ��������� ��� ���� ������
		UpdatePoint(D, true, points, 1.5);

		glColor3d(0.8, 0.7, 0.3);

		//��� ������
		Draw::DrawBottom(A, B, C, D, E, F, G, H, pointsBC, pointsCD);
		
		//����� ��������
		if (!texChange) {
			glBindTexture(GL_TEXTURE_2D, texId[0]);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texId[1]);
		}

		//����� ������
		Draw::DrawTop(A, B, C, D, E, F, G, H, pointsBC, pointsCD, z, alpha);

	}
};



