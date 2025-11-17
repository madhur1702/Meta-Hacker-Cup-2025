#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    freopen("narrowing_down_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int T;
    if(!(cin >> T)) return 0;
    for(int tc = 1; tc <= T; ++tc) {
        int N;
        cin >> N;
        vector<unsigned int> A(N+1);
        for(int i=1;i<=N;++i) cin >> A[i];
        vector<unsigned int> P(N+1);
        P[0] = 0;
        for(int i=1;i<=N;++i) P[i] = P[i-1] ^ A[i];
        unordered_map<unsigned int, vector<int>> occ;
        occ.reserve(N*2);
        for(int i=0;i<=N;++i) occ[P[i]].push_back(i);

        ll S = 0;
        for(ll d = 1; d <= N; ++d) {
            S += d * ( (ll)(N + 1 - d) );
        }
        ll Tsum = 0;
        for(auto &kv : occ) {
            const vector<int> &v = kv.second;
            ll cnt = 0;
            ll sumIdx = 0;
            for(int b = 0; b < (int)v.size(); ++b) {
                Tsum += (ll)b * cnt - sumIdx;
                cnt += 1;
                sumIdx += b;
            }
        }

        ll ans = S - Tsum;
        cout << "Case #" << tc << ": " << ans << '\n';
    }
    return 0;
}
