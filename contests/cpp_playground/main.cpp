#include <iostream>

using namespace std;

int factmod (int n, int p) {
 int res = 1;
 while (n > 1) {
  res = (res * ((n/p) % 2 ? p-1 : 1)) % p;
  for (int i=2; i<=n%p; ++i)
   res = (res * i) % p;
  n /= p;
 }
 return res % p;
}

int main(){
    int m;
    cin >> m;
    for (int i=0; i<=m; i++){
        int n, t;
        cin >> n >> t;
        if (t <= n)
            cout << 0 << endl;
        else
            cout << factmod(n, t) << endl;
    }
}
