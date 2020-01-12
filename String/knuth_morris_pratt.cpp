//
// Knuth-Morris-Pratt
//   fail[i] := pat[0:i) �� suffix �� pat �� prefix ���ɤ�������פ��뤫 (Ĺ�� i ̤����)
//   O(N) �ǹ��ۤǤ���
//   ��������Ѥ��ưʲ������줿��
//     ��pat[0:i) �κǾ����� ("abcabcab" �� "abc" �η����֤�����ڤ뤳�ȤǤǤ���Τ� 3)
//     ��ʸ���� S �ˤ����� S[i:] �� prefix �� pat �Ȱ��פ���褦�� i �򤹤٤Ƶ���
//
// verified:
//   ABC 150 F - Xor Shift
//     https://atcoder.jp/contests/abc150/tasks/abc150_f
//
//   ARC 060 F - ����ɽ��
//     https://atcoder.jp/contests/arc060/tasks/arc060_d
//


#include <iostream>
#include <vector>
using namespace std;

// T = string or vector<long long>
template<class T> struct KMP {
    T pat;
    vector<int> fail;

    // construct
    KMP(const T &p) { init(p); }
    void init(const T &p) {
        pat = p;
        int m = (int)pat.size();
        fail.assign(m+1, -1);
        for (int i = 0, j = -1; i < m; ++i) {
            while (j >= 0 && pat[i] != pat[j]) j = fail[j];
            fail[i+1] = ++j;
        }
    }

    // the period of S[0:i]
    int period(int i) { return i - fail[i]; }
    
    // the index i such that S[i:] has the exact prefix p
    vector<int> match(const T &S) {
        int n = (int)S.size(), m = (int)pat.size();
        vector<int> res;
        for (int i = 0, k = 0; i < n; ++i) {
            while (k >= 0 && S[i] != pat[k]) k = fail[k];
            ++k;
            if (k >= m) res.push_back(i - m + 1), k = fail[k];
        }
        return res;
    }
};


void solve(int N, const vector<long long> &a, const vector<long long> &b) {
    vector<long long> da(N*2), db(N);
    for (int i = 0; i < N*2; ++i) {
        int j = i % N;
        int k = (i + 1) % N;
        da[i] = a[j] ^ a[k];
        if (i < N) db[i] = b[j] ^ b[k];
    }
    KMP<vector<long long> > kmp(db);
    auto v = kmp.match(da);
    for (auto k : v) {
        if (k < N) cout << k << " " << (a[k] ^ b[0]) << endl;
    }
}

int main() {
    int N;
    cin >> N;
    vector<long long> a(N), b(N);
    for (int i = 0; i < N; ++i) cin >> a[i];
    for (int i = 0; i < N; ++i) cin >> b[i];
    solve(N, a, b);
}

