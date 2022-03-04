#include <iostream>
#include <vector>

using namespace std;

vector<string> ans;

void generate (int opened, int left, string tmp) {
    if (opened == left) {
        for (int i = 0; i < left; i++) tmp += ')';
        ans.push_back(tmp);
        tmp = "";
        return;
    }
    if (opened > left || left <= 0 || opened < 0) {
        return;
    }
    generate(opened + 1, left - 1, tmp + '(');
    generate(opened - 1, left - 1, tmp + ')');
}


int main(){
    int n;
    cin >> n;
    string s;
    generate(0, 2*n, s);
    for (const string& str : ans)
        cout << str << endl;
}