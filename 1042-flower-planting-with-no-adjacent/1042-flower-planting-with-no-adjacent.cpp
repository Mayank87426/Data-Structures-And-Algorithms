class Solution {
public:
    bool check(int node, int col, vector<int>& flowers,
               unordered_map<int, vector<int>>& adj) {
        for (int x : adj[node]) {
            if (flowers[x - 1] == col)
                return false;
        }
        return true;
    }
    bool dfs(int node, unordered_map<int, vector<int>>& adj,
             vector<int>& flowers, int n) {
        int count = 0;
        for (int i = 1; i <= n; i++) {
            if (flowers[i - 1] != -1)
                count++;
        }
        if (count == n)
            return true;
        for (int i = 1; i <= 4; i++) {
            if (check(node, i, flowers, adj)) {
                flowers[node - 1] = i;
                for (int nbr : adj[node]) {
                    if (flowers[nbr - 1] != -1)
                        continue;
                    if (dfs(nbr, adj, flowers, n))
                        return true;
                }
            }
        }
        return false;
    }
    vector<int> gardenNoAdj(int n, vector<vector<int>>& paths) {
        unordered_map<int, vector<int>> adj;
        vector<int> flowers(n, -1);
        for (auto e : paths) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        for (int i = 1; i <= n; i++) {
            if (flowers[i - 1] == -1) {
                auto f = dfs(i, adj, flowers, n);
            }
        }
        return flowers;
    }
};