#include <iostream>
using namespace std;

struct Drumetie {
    int nr, s;
};

// am putea pune codul de aici in 'planificare', dar banuiesc ca
// asa ați face majoritatea. (ceea ce nu e așa de rău)
void sortare(Drumetie v[], int m) {
    // aici putem sa le sortam după ziua de incepere, pt ca ziua
    // de final e mereu 's+n'
    for (int i = 0; i < m; i++) {
        for (int j = i+1; j < m; j++) {
            if (v[j].s < v[i].s) {
                Drumetie t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
        }
    }
}

// funcția retuneaza indicele ultimului spectacol, ceea ce e destul
// de idiot. (am putea returna k+1, adica lungimea)
int planificare(Drumetie a[], int m, int b[], int n) {
    sortare(a, m);
    b[0] = a[0].nr;
    // in loc sa memoram acest u, am putea memora
    // v[u].s+2
    int u = 0;
    int k = 0;
    for (int i = 1; i < m; i++) {
         if (a[i].s >= a[u].s+n) {
            k += 1;// sau 'k++', ori și mai bine '++k'
            b[k] = a[i].nr;
            u = i;
         }
    }
    return k;
}

int main() {
    Drumetie v[100];
    int m, n;
    int b[100];

    // ar fi indicat sa adaugati si ceva cum ar fi 'cout << "introduce-ti m:";....'
    cin >> m >> n;
    for (int i = 0; i < m; i++) {
        int s;
        cin >> s;// sau 'cin >> v[i].s;'
        v[i].s =s;
        v[i].nr = i+1;
    }
    int k = planificare(v, m, b, 2);

    // daca returnam k+1, mergem pana la k, ca oamenii normali
    for (int i = 0; i < k+1; i++)
        cout << b[i] << " ";
    //int k = planificare();
    return 0;
}

