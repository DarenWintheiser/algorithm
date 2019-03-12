//
// Manacher �Υ��르�ꥺ��
//   res[i] := S[i] ���濴�Ȥ��� S ��δ��Ĺ�κ�Ĺ��ʸ��Ⱦ�� (�濴��ޤ�)
//   ����Ĺ�β�ʸ���᤿���Ȥ��� S = "abc" ���Ф��� "a$b$c" �Ȥ��ä������򤹤�ƥ�������
//
// verified:
//   RUPC 2019 day3-E ����ʸ����
//     https://onlinejudge.u-aizu.ac.jp/beta/room.html#RitsCamp19Day3/problems/E
//


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


vector<int> Manacher(const string &S) {
    int N = (int)S.size();
    vector<int> res(N);
    int i = 0, j = 0;
    while (i < N) {
        while (i-j >= 0 && i+j < N && S[i-j] == S[i+j]) ++j;
        res[i] = j;
        int k = 1;
        while (i-k >= 0 && i+k < N && k+res[i-k] < j) res[i+k] = res[i-k], ++k;
        i += k, j -= k;
    }
    return res;
}


int main() {
    int N; string S;
    cin >> N >> S;

    auto rad = Manacher(S);
    int res = N;
    for (int len = 2; len < N; ++len) {
        bool ok = true;
        for (int i = len - 1; i < N; i += len - 1) {
            if (rad[i] != min(i+1, N-i)) ok = false;
        }
        if (ok) {
            res = len;
            break;
        }
    }
    cout << res << endl;
}
