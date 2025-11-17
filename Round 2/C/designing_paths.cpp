#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <tuple>
#include <algorithm>
#include <cmath> // Include cmath for min/max if not provided by algorithm  

using namespace std;  

// Maximum size constraints
const int MAX_COURTS = 500005;
const int MAX_SUM_L = 1000005;

// Graph represented as an adjacency list: pair<to_node, weight>
vector<pair<int, int>> adj[MAX_COURTS + 4 * MAX_SUM_L];
long long D[MAX_COURTS + 4 * MAX_SUM_L];

// Variables for node ID mapping
int N, K, M;
int court_node_count; // N
int next_aux_node_id; 

// Structure to store Segment Tree information for each route
struct RouteTree {
    vector<int> stops; // The actual court IDs A_i,j
    vector<int> in_tree_nodes; // Nodes for the 'In' tree
    vector<int> out_tree_nodes; // Nodes for the 'Out' tree
    int L; // Length of the route
};
vector<RouteTree> routes;

// --- Node ID Management ---

// Court node IDs are 1 to N
int get_court_node(int court_id) {
    return court_id;
}

// Auxiliary node creation
int new_aux_node() {
    return next_aux_node_id++;
}

// --- Segment Tree Construction ---

// Recursive build for the 'In' tree (parent to children: 0-weight)
// start_id is 0-indexed position of the court in the stops vector
int build_in_tree(RouteTree& rt, int l, int r, int node_idx) {
    int current_node = rt.in_tree_nodes[node_idx] = new_aux_node();

    if (l == r) {
        // Leaf node: connect to the court node (Debarking: 0-weight)
        int court_id = rt.stops[l];
        adj[current_node].push_back({get_court_node(court_id), 0});
        return current_node;
    }

    int mid = l + (r - l) / 2;
    int left_child = build_in_tree(rt, l, mid, 2 * node_idx + 1);
    int right_child = build_in_tree(rt, mid + 1, r, 2 * node_idx + 2);

    // Parent to children edges (0-weight)
    adj[current_node].push_back({left_child, 0});
    adj[current_node].push_back({right_child, 0});
    
    return current_node;
}

// Recursive build for the 'Out' tree (children to parent: 0-weight)
int build_out_tree(RouteTree& rt, int l, int r, int node_idx) {
    int current_node = rt.out_tree_nodes[node_idx] = new_aux_node();

    if (l == r) {
        // Leaf node: connect from the court node (Boarding: 0-weight)
        int court_id = rt.stops[l];
        adj[get_court_node(court_id)].push_back({current_node, 0});
        return current_node;
    }

    int mid = l + (r - l) / 2;
    int left_child = build_out_tree(rt, l, mid, 2 * node_idx + 1);
    int right_child = build_out_tree(rt, mid + 1, r, 2 * node_idx + 2);

    // Children to parent edges (0-weight)
    adj[left_child].push_back({current_node, 0});
    adj[right_child].push_back({current_node, 0});

    return current_node;
}

// --- Range Connection (The Ride: 1-weight) ---

// Connects an 'Out' node (source_node) to 'In' tree nodes covering range [ql, qr]
void connect_range(RouteTree& rt, int l, int r, int node_idx, int ql, int qr, int source_node) {
    if (ql > r || qr < l) {
        return; // Current segment is outside the query range
    }
    
    if (ql <= l && r <= qr) {
        // Current segment is fully within the query range: establish the 1-weight edge
        adj[source_node].push_back({rt.in_tree_nodes[node_idx], 1});
        return;
    }

    int mid = l + (r - l) / 2;
    connect_range(rt, l, mid, 2 * node_idx + 1, ql, qr, source_node);
    connect_range(rt, mid + 1, r, 2 * node_idx + 2, ql, qr, source_node);
}

// --- Main Solver ---

void solve() {
    if (!(cin >> N >> K >> M)) return;

    // Reset global state
    routes.clear();
    court_node_count = N;
    next_aux_node_id = N + 1;
    int max_nodes = N + 4 * MAX_SUM_L;
    
    // Clear adjacency list and distance array
    for (int i = 1; i <= max_nodes; ++i) {
        adj[i].clear();
        D[i] = -1; // Use -1 to represent infinity/unreachable, as distances are non-negative
    }

    // 1. Read Input and Build Segment Tree Structures
    for (int i = 0; i < M; ++i) {
        RouteTree rt;
        int L;
        cin >> L;
        rt.L = L;
        rt.stops.resize(L);
        for (int j = 0; j < L; ++j) {
            cin >> rt.stops[j];
        }

        // Segment Tree requires size proportional to power of 2, 
        // up to 4*L nodes for a full tree representation.
        // We use L here as the max index, node_idx starts at 0.
        rt.in_tree_nodes.resize(4 * L);
        rt.out_tree_nodes.resize(4 * L);

        // Build the auxiliary trees
        if (L > 0) {
            build_in_tree(rt, 0, L - 1, 0);
            build_out_tree(rt, 0, L - 1, 0);
        }

        routes.push_back(rt);
    }

    // 2. Add the 1-Weight Ride Edges
    for (RouteTree& rt : routes) {
        int L = rt.L;
        if (L < 2) continue;

        for (int j = 0; j < L - 1; ++j) { // j is the 0-indexed boarding stop
            
            // Find the leaf node ID corresponding to stop j in the Out-Tree.
            int leaf_out_node = -1;
            int court_id = rt.stops[j];
            
            // CORRECTED: Use a standard auto loop instead of structured binding (C++17)
            for(auto const& edge : adj[get_court_node(court_id)]) {
                int neighbor = edge.first;
                int weight = edge.second;
                
                if(weight == 0) {
                    leaf_out_node = neighbor;
                    break;
                }
            }

            if (leaf_out_node == -1) continue; // Should not happen

            // The debarking range is [j+1, min(L-1, j+K)]
            int ql = j + 1; // 0-indexed start of debarking
            int qr = min(L - 1, j + K); // 0-indexed end of debarking
            
            // Connect the 'Out' leaf to the 'In' tree covering the debarking range [ql, qr]
            connect_range(rt, 0, L - 1, 0, ql, qr, leaf_out_node);
        }
    }

    // 3. 0-1 BFS
    // Node 1 is the start court C_1
    int start_node = get_court_node(1);
    D[start_node] = 0;
    deque<int> Q;
    Q.push_front(start_node);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop_front();

        // CORRECTED: Use a standard auto loop instead of structured binding (C++17)
        for (auto const& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            if (D[v] == -1 || D[u] + w < D[v]) {
                D[v] = D[u] + w;
                if (w == 0) {
                    Q.push_front(v);
                } else {
                    Q.push_back(v);
                }
            }
        }
    }

    // 4. Calculate Final Sum
    long long final_sum = 0;
    for (int i = 1; i <= N; ++i) {
        long long di = D[get_court_node(i)];
        if (di == -1) {
            // Unreachable
            final_sum += (long long)i * (-1);
        } else {
            final_sum += (long long)i * di;
        }
    }
    
    cout << final_sum << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("designing_paths_input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int t;
    if (!(cin >> t)) return 0;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ": ";
        solve();
    }
    return 0;

}

