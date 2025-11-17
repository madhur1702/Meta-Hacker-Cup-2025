#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;

long long power(long long a, long long b, long long mod) {
    long long res = 1;
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

long long modInv(long long a) {
    return power(a, MOD - 2, MOD);
}

long long comb(long long n, long long k) {
    if (k > n || k < 0) return 0;
    if (k == 0) return 1;
    
    if (k > 60) k = n - k;
    if (k > 60) return 0;
    
    long long num = 1, den = 1;
    for (long long i = 0; i < k; i++) {
        num = (num % MOD * ((n - i) % MOD)) % MOD;
        den = (den % MOD * ((i + 1) % MOD)) % MOD;
    }
    
    return (num * modInv(den)) % MOD;
}

long long countWays(vector<pair<long long, int>>& factors, long long K) {
    if (factors.empty()) return 1;
    
    long long result = 1;
    for (auto& p : factors) {
        int exp = p.second;
        result = (result * comb(exp + K - 1, K - 1)) % MOD;
    }
    return result;
}

vector<pair<long long, int>> factorize(long long n) {
    vector<pair<long long, int>> factors;
    
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                cnt++;
                n /= i;
            }
            factors.push_back({i, cnt});
        }
    }
    if (n > 1) factors.push_back({n, 1});
    
    return factors;
}

void generateDivisors(int idx, long long curr, long long A, 
                      vector<pair<long long, int>>& primeFactors,
                      vector<long long>& divisors) {
    if (idx == primeFactors.size()) {
        if (curr <= A) divisors.push_back(curr);
        return;
    }
    
    long long p = primeFactors[idx].first;
    int maxExp = primeFactors[idx].second;
    long long pk = 1;
    
    for (int e = 0; e <= maxExp; e++) {
        if (curr * pk > A) break;
        generateDivisors(idx + 1, curr * pk, A, primeFactors, divisors);
        pk *= p;
    }
}

int main() {
     freopen("final_product_chapter_2_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int T;
    cin >> T;
    
    for (int t = 1; t <= T; t++) {
        long long N, A, B;
        cin >> N >> A >> B;
        
        vector<pair<long long, int>> factorsB = factorize(B);
        
        vector<long long> divisors;
        generateDivisors(0, 1, A, factorsB, divisors);
        
        long long totalWays = 0;
        
        for (long long X : divisors) {
            vector<pair<long long, int>> factorsX = factorize(X);
            vector<pair<long long, int>> factorsBoverX = factorize(B / X);
            
            long long firstPhase = countWays(factorsX, N);
            long long secondPhase = countWays(factorsBoverX, N);
            long long ways = (firstPhase * secondPhase) % MOD;
            totalWays = (totalWays + ways) % MOD;
        }
        
        cout << "Case #" << t << ": " << totalWays << "\n";
    }
    
    return 0;
}