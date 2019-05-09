#include <iostream>
using namespace std;
struct Zi {
    int temp;
    int presiune;
};

int main() {
    int n;
    cin >> n;

    Zi x[100];
    for (int i = 0; i < n; ++i)
        cin >> x[i].temp >> x[i].presiune;

    // daca vreți alt algoritm de sortare inlocuiți aici:
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (x[i].temp > x[j].temp || (x[i].temp == x[j].temp && x[i].presiune < x[j].presiune)) {
               Zi t = x[i];
               x[i] = x[j];
               x[j] = t;
            }
        }
    }
    for (int i = 0; i < n; ++i)
       cout << "t:" << x[i].temp << ", p: "<< x[i].presiune << endl;

    return 0;
}
