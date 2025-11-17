#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("deciding_points_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int T;
    cin >> T;
    
    for (int t = 1; t <= T; t++) {
        long long N, M;
        cin >> N >> M;
        
        cout << "Case #" << t << ": ";
        
        long long threshold = 2 * M - 2;
        
        if (N <= threshold) {
            if (N >= M) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        } else {
            if (N % 2 == 0) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }
    
    return 0;
}