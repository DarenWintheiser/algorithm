//
// 実行時に法が決まる Modular Arithmetics
//
// cf.
//   noshi91: modint 構造体を使ってみませんか？ (C++)
//     http://noshi91.hatenablog.com/entry/2019/03/31/174006
//
//   drken: 「1000000007 で割ったあまり」の求め方を総特集！ ～ 逆元から離散対数まで ～
//     https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
//
// verified:
//   Yosupo Library Checker - Binomial Coefficient (Prime Mod)
//     https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod
//
//   ARC 096 E - Everything on It
//     https://atcoder.jp/contests/arc096/tasks/arc096_c
//


#include <bits/stdc++.h>
using namespace std;


// dynamic modint
struct DynamicModint {
    using mint = DynamicModint;
    
    // static menber
    static int MOD;
    
    // inner value
    long long val;
    
    // constructor
    DynamicModint() : val(0) { }
    DynamicModint(long long v) : val(v % MOD) {
        if (val < 0) val += MOD;
    }
    long long get() const { return val; }
    static int get_mod() { return MOD; }
    static void set_mod(int mod) { MOD = mod; }
    
    // arithmetic operators
    mint operator + () const { return mint(*this); }
    mint operator - () const { return mint(0) - mint(*this); }
    mint operator + (const mint &r) const { return mint(*this) += r; }
    mint operator - (const mint &r) const { return mint(*this) -= r; }
    mint operator * (const mint &r) const { return mint(*this) *= r; }
    mint operator / (const mint &r) const { return mint(*this) /= r; }
    mint& operator += (const mint &r) {
        val += r.val;
        if (val >= MOD) val -= MOD;
        return *this;
    }
    mint& operator -= (const mint &r) {
        val -= r.val;
        if (val < 0) val += MOD;
        return *this;
    }
    mint& operator *= (const mint &r) {
        val = val * r.val % MOD;
        return *this;
    }
    mint& operator /= (const mint &r) {
        long long a = r.val, b = MOD, u = 1, v = 0;
        while (b) {
            long long t = a / b;
            a -= t * b, swap(a, b);
            u -= t * v, swap(u, v);
        }
        val = val * u % MOD;
        if (val < 0) val += MOD;
        return *this;
    }
    mint pow(long long n) const {
        mint res(1), mul(*this);
        while (n > 0) {
            if (n & 1) res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }
    mint inv() const {
        mint res(1), div(*this);
        return res / div;
    }

    // other operators
    bool operator == (const mint &r) const {
        return this->val == r.val;
    }
    bool operator != (const mint &r) const {
        return this->val != r.val;
    }
    mint& operator ++ () {
        ++val;
        if (val >= MOD) val -= MOD;
        return *this;
    }
    mint& operator -- () {
        if (val == 0) val += MOD;
        --val;
        return *this;
    }
    mint operator ++ (int) {
        mint res = *this;
        ++*this;
        return res;
    }
    mint operator -- (int) {
        mint res = *this;
        --*this;
        return res;
    }
    friend istream& operator >> (istream &is, mint &x) {
        is >> x.val;
        x.val %= x.get_mod();
        if (x.val < 0) x.val += x.get_mod();
        return is;
    }
    friend ostream& operator << (ostream &os, const mint &x) {
        return os << x.val;
    }
    friend mint pow(const mint &r, long long n) {
        return r.pow(n);
    }
    friend mint inv(const mint &r) {
        return r.inv();
    }
};

int DynamicModint::MOD;

// Binomial coefficient
template<class mint> struct BiCoef {
    vector<mint> fact_, inv_, finv_;
    constexpr BiCoef() {}
    constexpr BiCoef(int n) : fact_(n, 1), inv_(n, 1), finv_(n, 1) {
        init(n);
    }
    constexpr void init(int n) {
        fact_.assign(n, 1), inv_.assign(n, 1), finv_.assign(n, 1);
        int MOD = fact_[0].get_mod();
        for(int i = 2; i < n; i++){
            fact_[i] = fact_[i-1] * i;
            inv_[i] = -inv_[MOD%i] * (MOD/i);
            finv_[i] = finv_[i-1] * inv_[i];
        }
    }
    constexpr mint com(int n, int k) const {
        if (n < k || n < 0 || k < 0) return 0;
        return fact_[n] * finv_[k] * finv_[n-k];
    }
    constexpr mint fact(int n) const {
        if (n < 0) return 0;
        return fact_[n];
    }
    constexpr mint inv(int n) const {
        if (n < 0) return 0;
        return inv_[n];
    }
    constexpr mint finv(int n) const {
        if (n < 0) return 0;
        return finv_[n];
    }
};


/*/////////////////////////////*/
// Examples
/*/////////////////////////////*/

void Yosupo_Binomial_Coefficient() {
    const int MAX = 11000000;
    int T, M;
    cin >> T >> M;
    using mint = DynamicModint;
    mint::set_mod(M);
    
    BiCoef<mint> bc(MAX);
    while (T--) {
        int n, k;
        cin >> n >> k;
        cout << bc.com(n, k) << endl;
    }
}

// スターリング数 (n 個を k グループにわける、n >= k)
template<class mint> struct Stirling {
    vector<vector<mint>> S;
    constexpr Stirling(int MAX) : S(MAX, vector<mint>(MAX, 0)) {
        S[0][0] = 1;
        for (int n = 1; n < MAX; ++n) {
            for (int k = 1; k <= n; ++k) {
                S[n][k] = S[n-1][k-1] + S[n-1][k] * k;
            }
        }
    }
    constexpr mint get(int n, int k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return S[n][k];
    }
};

void ARC_096_E() {
    // 入力
    long long N, M;
    cin >> N >> M;
    using mint = DynamicModint;
    mint::set_mod(M);
    
    // 前計算
    const int MAX = 3100;
    BiCoef<mint> bc(MAX); // 二項係数計算の前処理
    Stirling<mint> sl(MAX); // スターリング数の前処理

    // 2^n や 2^2^n の前計算、2^2^(n+1) = (2^2^n)^2
    vector<mint> two(MAX*MAX, 0), dtwo(MAX, 0);
    two[0] = 1, dtwo[0] = 2;
    for (int i = 1; i < MAX; ++i) dtwo[i] = dtwo[i-1] * dtwo[i-1];
    for (int i = 1; i < MAX*MAX; ++i) two[i] = two[i-1] * 2;
    
    // 求める
    mint res = 0;
    for (int n = 0; n <= N; ++n) {
        mint add = 0;
        for (int k = 0; k <= n; ++k) {
            mint jiyudo = two[(N-n)*k] * dtwo[N-n];
            mint core = sl.get(n, k) + sl.get(n, k+1) * (k+1);
            add += core * jiyudo;
        }
        mint choose = bc.com(N, n);
        add *= choose;
        if (n % 2 == 0) res += add;
        else res -= add;
    }
    cout << res << endl;
}

int main() {
    Yosupo_Binomial_Coefficient();
    //ARC_096_E();
}

