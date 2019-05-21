#include<iostream>
using namespace std;
int n, nr_sol, sol[20];

void afis(int l){
    nr_sol++;
    cout<<"Solutia nr. "<< nr_sol<<" : ";
    for(int i = 0; i < l; i++)
        cout<<sol[i]<<" ";
    cout<<endl;
}

void back(int i, int n) {
    if (n == 0) {
        afis(i);
        return;
    }
    for(int j = 1; j<= n; j++) {
        sol[i]=j;
        back(i+1, n-j);
    }
}

int main(){
    cin>>n;
    nr_sol=0;
    back(0,n);
    cout<<nr_sol<<" solutii";
    return 0;
}

