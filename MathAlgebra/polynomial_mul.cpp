//
// 多項式の乗算 (by NTT), O(N log N)
// 　・係数は p を素数として Fp 体を想定
//
// verified:
//   Yosupo Library Checker - Convolution
//     https://judge.yosupo.jp/problem/convolution_mod
//


#include <bits/stdc++.h>
using namespace std;


// modint
template<int MOD> struct Fp {
    // inner value
    long long val;
    
    // constructor
    constexpr Fp() : val(0) { }
    constexpr Fp(long long v) : val(v % MOD) {
        if (val < 0) val += MOD;
    }
    constexpr long long get() const { return val; }
    constexpr int get_mod() const { return MOD; }
    
    // arithmetic operators
    constexpr Fp operator + () const { return Fp(*this); }
    constexpr Fp operator - () const { return Fp(0) - Fp(*this); }
    constexpr Fp operator + (const Fp &r) const { return Fp(*this) += r; }
    constexpr Fp operator - (const Fp &r) const { return Fp(*this) -= r; }
    constexpr Fp operator * (const Fp &r) const { return Fp(*this) *= r; }
    constexpr Fp operator / (const Fp &r) const { return Fp(*this) /= r; }
    constexpr Fp& operator += (const Fp &r) {
        val += r.val;
        if (val >= MOD) val -= MOD;
        return *this;
    }
    constexpr Fp& operator -= (const Fp &r) {
        val -= r.val;
        if (val < 0) val += MOD;
        return *this;
    }
    constexpr Fp& operator *= (const Fp &r) {
        val = val * r.val % MOD;
        return *this;
    }
    constexpr Fp& operator /= (const Fp &r) {
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
    constexpr Fp pow(long long n) const {
        Fp res(1), mul(*this);
        while (n > 0) {
            if (n & 1) res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }
    constexpr Fp inv() const {
        Fp res(1), div(*this);
        return res / div;
    }

    // other operators
    constexpr bool operator == (const Fp &r) const {
        return this->val == r.val;
    }
    constexpr bool operator != (const Fp &r) const {
        return this->val != r.val;
    }
    constexpr Fp& operator ++ () {
        ++val;
        if (val >= MOD) val -= MOD;
        return *this;
    }
    constexpr Fp& operator -- () {
        if (val == 0) val += MOD;
        --val;
        return *this;
    }
    constexpr Fp operator ++ (int) const {
        Fp res = *this;
        ++*this;
        return res;
    }
    constexpr Fp operator -- (int) const {
        Fp res = *this;
        --*this;
        return res;
    }
    friend constexpr istream& operator >> (istream &is, Fp<MOD> &x) {
        is >> x.val;
        x.val %= MOD;
        if (x.val < 0) x.val += MOD;
        return is;
    }
    friend constexpr ostream& operator << (ostream &os, const Fp<MOD> &x) {
        return os << x.val;
    }
    friend constexpr Fp<MOD> pow(const Fp<MOD> &r, long long n) {
        return r.pow(n);
    }
    friend constexpr Fp<MOD> inv(const Fp<MOD> &r) {
        return r.inv();
    }
};

// NTT
namespace NTT {
long long modpow(long long a, long long n, int mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}

long long modinv(long long a, int mod) {
    long long b = mod, u = 1, v = 0;
    while (b) {
        long long t = a / b;
        a -= t * b, swap(a, b);
        u -= t * v, swap(u, v);
    }
    u %= mod;
    if (u < 0) u += mod;
    return u;
}

int calc_primitive_root(int mod) {
    if (mod == 2) return 1;
    if (mod == 167772161) return 3;
    if (mod == 469762049) return 3;
    if (mod == 754974721) return 11;
    if (mod == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    long long x = (mod - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (long long i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) x /= i;
        }
    }
    if (x > 1) divs[cnt++] = x;
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (modpow(g, (mod - 1) / divs[i], mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}

int get_fft_size(int N, int M) {
    int size_a = 1, size_b = 1;
    while (size_a < N) size_a <<= 1;
    while (size_b < M) size_b <<= 1;
    return max(size_a, size_b) << 1;
}

// number-theoretic transform
template<class mint> void trans(vector<mint> &v, bool inv = false) {
    if (v.empty()) return;
    int N = (int)v.size();
    int MOD = v[0].get_mod();
    int PR = calc_primitive_root(MOD);
    static bool first = true;
    static vector<long long> vbw(30), vibw(30);
    if (first) {
        first = false;
        for (int k = 0; k < 30; ++k) {
            vbw[k] = modpow(PR, (MOD - 1) >> (k + 1), MOD);
            vibw[k] = modinv(vbw[k], MOD);
        }
    }
    for (int i = 0, j = 1; j < N - 1; j++) {
        for (int k = N >> 1; k > (i ^= k); k >>= 1);
        if (i > j) swap(v[i], v[j]);
    }
    for (int k = 0, t = 2; t <= N; ++k, t <<= 1) {
        long long bw = vbw[k];
        if (inv) bw = vibw[k];
        for (int i = 0; i < N; i += t) {
            mint w = 1;
            for (int j = 0; j < t/2; ++j) {
                int j1 = i + j, j2 = i + j + t/2;
                mint c1 = v[j1], c2 = v[j2] * w;
                v[j1] = c1 + c2;
                v[j2] = c1 - c2;
                w *= bw;
            }
        }
    }
    if (inv) {
        long long invN = modinv(N, MOD);
        for (int i = 0; i < N; ++i) v[i] = v[i] * invN;
    }
}

// small case (T = mint, long long)
template<class T> vector<T> naive_mul(const vector<T> &A, const vector<T> &B) {
    if (A.empty() || B.empty()) return {};
    int N = (int)A.size(), M = (int)B.size();
    vector<T> res(N + M - 1);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            res[i + j] += A[i] * B[j];
    return res;
}

// convolution
template<class mint> vector<mint> mul(const vector<mint> &A, const vector<mint> &B) {
    if (A.empty() || B.empty()) return {};
    int N = (int)A.size(), M = (int)B.size();
    if (min(N, M) < 30) return naive_mul(A, B);
    int MOD = A[0].get_mod();
    int size_fft = get_fft_size(N, M);
    
    vector<mint> a(size_fft), b(size_fft), c(size_fft);
    for (int i = 0; i < N; ++i) a[i] = A[i];
    for (int i = 0; i < M; ++i) b[i] = B[i];
    trans(a), trans(b);
    vector<mint> res(size_fft);
    for (int i = 0; i < size_fft; ++i) res[i] = a[i] * b[i];
    trans(res, true);
    res.resize(N + M - 1);
    return res;
}
};

// Polynomial
template<typename mint> struct Poly : vector<mint> {
    using vector<mint>::vector;
 
    // constructor
    constexpr Poly(const vector<mint> &r) : vector<mint>(r) {}
 
    // core operator
    constexpr mint eval(const mint &v) {
        mint res = 0;
        for (int i = (int)this->size()-1; i >= 0; --i) {
            res *= v;
            res += (*this)[i];
        }
        return res;
    }
    constexpr Poly& normalize() {
        while (!this->empty() && this->back() == 0) this->pop_back();
        return *this;
    }
 
    // basic operator
    constexpr Poly operator - () const noexcept {
        Poly res = (*this);
        for (int i = 0; i < (int)res.size(); ++i) res[i] = -res[i];
        return res;
    }
    constexpr Poly operator + (const mint &v) const { return Poly(*this) += v; }
    constexpr Poly operator + (const Poly &r) const { return Poly(*this) += r; }
    constexpr Poly operator - (const mint &v) const { return Poly(*this) -= v; }
    constexpr Poly operator - (const Poly &r) const { return Poly(*this) -= r; }
    constexpr Poly operator * (const mint &v) const { return Poly(*this) *= v; }
    constexpr Poly operator * (const Poly &r) const { return Poly(*this) *= r; }
    constexpr Poly operator << (int x) const { return Poly(*this) <<= x; }
    constexpr Poly operator >> (int x) const { return Poly(*this) >>= x; }
    constexpr Poly& operator += (const mint &v) {
        if (this->empty()) this->resize(1);
        (*this)[0] += v;
        return *this;
    }
    constexpr Poly& operator += (const Poly &r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); ++i) (*this)[i] += r[i];
        return this->normalize();
    }
    constexpr Poly& operator -= (const mint &v) {
        if (this->empty()) this->resize(1);
        (*this)[0] -= v;
        return *this;
    }
    constexpr Poly& operator -= (const Poly &r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); ++i) (*this)[i] -= r[i];
        return this->normalize();
    }
    constexpr Poly& operator *= (const mint &v) {
        for (int i = 0; i < (int)this->size(); ++i) (*this)[i] *= v;
        return *this;
    }
    constexpr Poly& operator *= (const Poly &r) {
        return *this = NTT::mul((*this), r);
    }
    constexpr Poly& operator <<= (int x) {
        Poly res(x, 0);
        res.insert(res.end(), begin(*this), end(*this));
        return *this = res;
    }
    constexpr Poly& operator >>= (int x) {
        Poly res;
        res.insert(res.end(), begin(*this) + x, end(*this));
        return *this = res;
    }
};



/*/////////////////////////////*/
// Examples
/*/////////////////////////////*/

void Yosupo_Convolution() {
    const int MOD = 998244353;
    using mint = Fp<MOD>;
    
    int N, M;
    cin >> N >> M;
    Poly<mint> a(N), b(M);
    for (int i = 0; i < N; ++i) cin >> a[i];
    for (int i = 0; i < M; ++i) cin >> b[i];
    
    Poly<mint> c = a * b;
    for (int i = 0; i < c.size(); ++i) cout << c[i] << " ";
    cout << endl;
}


int main() {
    Yosupo_Convolution();
}

