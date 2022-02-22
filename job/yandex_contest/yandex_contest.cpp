#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int nod(int a, int b) {
	if (b == 0)
		return a;
	else
		return nod(b, a % b);
}

int max_p_del(long n) {
	vector<bool> resh(n + 1, true);
	int max_p = 1;
	resh[0] = false;
	resh[1] = false;
	for (long i = 2; i <= n; i++)
		if (resh[i]) {
			if (n % i == 0)
				max_p = i;
			for (long j = i * i; i <= sqrt(n) && j <= n; j += i)
				resh[j] = false;
		}
	if (resh[n])
		max_p = n;
	return max_p;
}

int main() {
	int zeros = 0;
	int unos = 0;
	vector<bool> vec;
	int len;
	cin >> len;
	char ch;
	cin.read(&ch, 1);
	for (int i = 0; i < len; i++) {
		cin.read(&ch, 1);
		if (ch == '0') {
			zeros++;
			vec.push_back(0);
		}
		else {
			unos++;
			vec.push_back(1);
		}
		if (i == 0) {
			cout << -1 << ' ';
			continue;
		}
		if (zeros > unos && ch == '0' || unos > zeros && ch == '1') {
			cout << 1 << ' ';
			continue;
		}
		int czeros = zeros;
		int cunos = unos;
		if (ch == '0') {
			int j = 1;
			for (int el : vec) {
				if (i + 1 == j)
					break;
				if (czeros > cunos) {
					cout << j << ' ';
					cunos = 0;
					czeros = 0;
					break;
				}
				if (el == 0)
					czeros--;
				else
					cunos--;
				j++;
			}
			if (cunos + czeros > 0)
				cout << -1 << ' ';
		}
		else {
			int j = 1;
			for (int el : vec) {
				if (i + 1 == j)
					break;
				if (cunos > czeros) {
					cout << j << ' ';
					cunos = 0;
					czeros = 0;
					break;
				}
				if (el == 0)
					czeros--;
				else
					cunos--;
				j++;
			}
			if (cunos + czeros > 0)
				cout << -1 << ' ';
		}
	}
}