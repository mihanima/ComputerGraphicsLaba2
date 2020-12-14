#include<cmath>
class MethodGauss
{
public:
	double* decision;
	int size;
private:
	double* gauss(double** a, double* y)
	{
		double* x = new double[size];
		double max;
		int k = 0, index;
		while (k < size)
		{
			// ����� ������ � ������������ a[i][k]
			max = abs(a[k][k]);
			index = k;
			for (int i = k + 1; i < size; i++)
			{
				if (abs(a[i][k]) > max)
				{
					max = abs(a[i][k]);
					index = i;
				}
			}
			// ������������ �����
			if (max < 0) {
				//������� ���
			}
			double temp;
			for (int j = 0; j < size; j++)
			{
				temp = a[k][j];
				a[k][j] = a[index][j];
				a[index][j] = temp;
			}
			temp = y[k];
			y[k] = y[index];
			y[index] = temp;
			// ������������ ���������
			for (int i = k; i < size; i++)
			{
				temp = a[i][k];
				if (abs(temp) == 0)
				{
					continue; // ��� �������� ������������ ����������
				}
				else
				{
					for (int j = 0; j < size; j++)
					{
						a[i][j] = a[i][j] / temp;
					}
				}
				y[i] = y[i] / temp;
				if (i == k)
				{
					continue; // ��������� �� �������� ���� �� ����
				}
				else
				{
					for (int j = 0; j < size; j++)
					{
						a[i][j] = a[i][j] - a[k][j];
					}
					y[i] = y[i] - y[k];
				}
			}
			k++;
		}
		// �������� �����������
		for (k = size - 1; k >= 0; k--)
		{
			x[k] = y[k];
			for (int i = 0; i < k; i++)
			{
				y[i] = y[i] - a[i][k] * x[k];
			}
		}
		return x;
	}
public:
	//���������� ������ ��� ������ ������
	MethodGauss(double matrix[3][4], int copy_row, int copy_col)
	{
		this->size = copy_row;

		double** a = new double* [size];
		for (int i = 0; i < size; i++) {
			a[i] = new double[size];
		}
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				a[i][j] = matrix[i][j];
			}
		}

		double* y = new double[size];
		for (int i = 0; i < size; i++)
		{
			y[i] = matrix[i][copy_col - 1];
		}

		//���������
		this->decision = gauss(a, y);

		//������������ ������
		for (int i = 0; i < size; i++) {
			delete[] a[i];
		}
		delete[] a;
		delete[] y;
	}
	~MethodGauss() {
		delete[] decision;
	}
};