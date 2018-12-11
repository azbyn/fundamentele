#include <iostream>
using namespace std;

void afiseaza(int n) {
	if (n <= 0) {
		cout << "TY DEBIL\n";
		return;
	}
	for (int cnt = 0; ; ++cnt) {
		for (int i = 0; i <= cnt; ++i) {
			cout << i << " ";
			if (--n <= 0) return;
		}
	}
	cout << "\n";
}
int main()
{
	int n;
	cin >> n;
	afiseaza(n);

	system("pause"); // Please don't do this
	return 0;
}
