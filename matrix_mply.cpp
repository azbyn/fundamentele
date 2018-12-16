#include <iostream>
using namespace std;
const int dim = 100;

//matrici indexate de la 0 ca oamenii normali linie, apoi coloana
struct Mat {
	int val[dim][dim];
	int lin;
	int col;
};

void afiseazaMat(Mat a) {
	for (int i = 0; i < a.lin; ++i) {
		for (int j = 0; j < a.col; j++) {
			cout << a.val[i][j] << " ";
		}
		cout << "\n";
	}
}
void citesteMat(char name, Mat* a) {
	cout << "Nr Linii = ";
	cin >> a->lin;
	cout << "Nr Col = ";
	cin >> a->col;

	for (int i = 0; i < a->lin; ++i) {
		for (int j = 0; j < a->col; j++) {
			cin >> a->val[i][j];
		}
	}
}

void multMat(Mat a, Mat b, Mat* res) {
	if (a.col != b.lin) {
		cout << "TY DEBIL\n";
		return;
	}
	res->col = b.col;
	res->lin = a.lin;

	for (int i = 0; i < res->lin; ++i) {
		for (int j = 0; j < res->col; ++j) {
			res->val[i][j] = 0;
			for (int k = 0; k < b.lin; ++k) {
				res->val[i][j] += a.val[i][k] * b.val[k][j];
			}
		}
	}
}

int main() {
	Mat a, b, res;
	cout << "a: \n";
	citesteMat('a', &a);
	cout << "a: \n";
	afiseazaMat(a);

	cout << "b: \n";
	citesteMat('b', &b);
	cout << "b: \n";
	afiseazaMat(b);


	cout << "\n" << "a*b:" << "\n";
	multMat(a, b, &res);

	afiseazaMat(res);

	system("pause"); // Please don't do this
	return 0;
}
