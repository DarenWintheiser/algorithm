//
// ����ȥ��ͥƥ��������Ѥ�����®�ǰ���ʬ��
//
// cf. 
//   ��®�ǰ���ʬ��
//     http://www.osak.jp/diary/diary_201310.html#20131017
//
// verified
//   Codeforces 511 DIV1 A Enlarge GCD
//     http://codeforces.com/contest/1034/problem/A
//


/*
    min_factor[i] := i �κǾ����ǰ��ҡ�����ȥ��ͥƥ��������Ѥ���
*/


#include <iostream>
#include <vector>
using namespace std;


const int MAX = 15001000;
bool IsPrime[MAX];
int MinFactor[MAX];
vector<int> preprocess(int n = MAX) {
    vector<int> res;
    for (int i = 0; i < n; ++i) IsPrime[i] = true, MinFactor[i] = -1;
	IsPrime[0] = false; IsPrime[1] = false; 
    MinFactor[0] = 0; MinFactor[1] = 1;
	for (int i = 2; i < n; ++i) {
		if (IsPrime[i]) {
            MinFactor[i] = i;
            res.push_back(i);
			for (int j = i*2; j < n; j += i) {
                IsPrime[j] = false;
                if (MinFactor[j] == -1) MinFactor[j] = i;
            }
		}
	}
    return res;
}

vector<pair<int,int> > prime_factor(int n) {
    vector<pair<int,int> > res;
    while (n != 1) {
        int prime = MinFactor[n];
        int exp = 0;
        while (MinFactor[n] == prime) {
            ++exp;
            n /= prime;
        }
        res.push_back(make_pair(prime, exp));
    }
    return res;
}


// ��������
int GCD(int x, int y) { return y ? GCD(y, x%y) : x; }


int main() {
    // ����
    int n; scanf("%d", &n);
    vector<int> a(n);
    for (int i = 0; i < n; ++i) scanf("%d", &a[i]);

    // ���������ǳ�äƤ���
    int g = 0;
    for (int i = 0; i < n; ++i) g = GCD(g, a[i]);
    for (int i = 0; i < n; ++i) a[i] /= g;

    // ���ǰ��Ҥ��Ȥ� a �˲��Ĥ��뤫�򥫥����
    preprocess();
    vector<int> count(MAX, 0);
    for (int i = 0; i < n; ++i) {
        auto pf = prime_factor(a[i]);
        for (auto p : pf) count[p.first]++;
    }
    int res = 0;
    for (int i = 0; i < MAX; ++i) res = max(res, count[i]);
    if (res == 0) cout << -1 << endl;
    else cout << n-res << endl;
}
