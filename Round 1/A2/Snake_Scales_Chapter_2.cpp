#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
     freopen("snake_scales_chapter_2_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int T;
    cin >> T;
    
    for (int tc = 1; tc <= T; tc++) {
        int N;
        cin >> N;
        
        vector<long long> A(N);
        for (int i = 0; i < N; i++) {
            cin >> A[i];
        }
        
        if (N == 1) {
            cout << "Case #" << tc << ": " << A[0] << "\n";
            continue;
        }
        
        // Binary search on answer
        long long left = 1, right = 1e9;
        long long answer = right;
        
        while (left <= right) {
            long long h = (left + right) / 2;
            
            // Check if ladder height h works
            // Split into segments where |A[i] - A[i+1]| > h forces new segment
            // Each segment must have min(A[i]) <= h
            bool valid = true;
            long long minInSegment = A[0];
            
            for (int i = 0; i < N; i++) {
                if (i == 0) {
                    minInSegment = A[0];
                } else {
                    if (abs(A[i] - A[i-1]) > h) {
                        // New segment, check previous segment
                        if (minInSegment > h) {
                            valid = false;
                            break;
                        }
                        minInSegment = A[i];
                    } else {
                        minInSegment = min(minInSegment, A[i]);
                    }
                }
            }
            
            // Check last segment
            if (valid && minInSegment > h) {
                valid = false;
            }
            
            if (valid) {
                answer = h;
                right = h - 1;
            } else {
                left = h + 1;
            }
        }
        
        cout << "Case #" << tc << ": " << answer << "\n";
    }
    
    return 0;
}