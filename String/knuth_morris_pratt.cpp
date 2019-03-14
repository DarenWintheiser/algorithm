//
// Knuth-Morris-Pratt
//   fail[i] := pat[0:i) �� suffix �� pat �� prefix ���ɤ�������פ��뤫 (Ĺ�� i ̤����)
//   O(N) �ǹ��ۤǤ���
//   ��������Ѥ��ưʲ������줿��
//     ��pat[0:i) �κǾ����� ("abcabcab" �� "abc" �η����֤�����ڤ뤳�ȤǤǤ���Τ� 3)
//     ��ʸ���� S �ˤ����� S[i:] �� prefix �� pat �Ȱ��פ���褦�� i �򤹤٤Ƶ���
//
// verified:
//   ARC 060 F - ����ɽ��
//     https://atcoder.jp/contests/arc060/tasks/arc060_d
//


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


struct KMP {
    string pat;
    vector<int> fail;

    // construct
    KMP(const string &p) { init(p); }
    void init(const string &p) {
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
    vector<int> match(const string &S) {
        int n = (int)S.size(), m = (int)pat.size();
        vector<int> res;
        for (int i = 0, k = 0; i < n; ++i) {
            while (k >= 0 && S[i] != pat[k]) k = fail[k];
            ++k;
            if (k == m) res.push_back(i-m+1);
        }
        return res;
    }
};


vector<long long> divisor(long long n) {
    vector<long long> res;
    for (long long i = 1LL; i*i <= n; ++i) {
        if (n%i == 0LL) {
            res.push_back(i);
            long long temp = n/i;
            if (i != temp) res.push_back(temp);
        }
    }
    sort(res.begin(), res.end());
    return res;
}

int main() {
    string str; cin >> str;
    int n = (int)str.size();
    vector<long long> divs = divisor(n);
    long long syuuki = n;
    for (auto d : divs) {
        bool ok = true;
        for (int j = 0; j + d < n; ++j) {
            if (str[j] != str[j+d]) ok = false;
        }
        if (ok) syuuki = min(syuuki, d);
    }
    if (syuuki == n) cout << 1 << endl << 1 << endl;
    else if (syuuki == 1) cout << n << endl << 1 << endl;
    else {
        vector<int> cannot_cut(n*2, 0);
        string str2 = str;
        reverse(str2.begin(), str2.end());
        KMP kmp1(str);
        KMP kmp2(str2);
        for (int d = 1; d < n; ++d) {
            if (kmp1.period(d) < d && d % kmp1.period(d) == 0) cannot_cut[d] = true;
            if (kmp2.period(d) < d && d % kmp2.period(d) == 0) cannot_cut[n-d] = true;
        }
        int con = 0;
        for (int i = 1; i < n; ++i) if (!cannot_cut[i]) ++con;
        cout << 2 << endl << con << endl;
    }
}
