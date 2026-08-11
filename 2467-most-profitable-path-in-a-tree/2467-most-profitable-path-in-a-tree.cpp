class Solution {
public:
    bool dfsBob(int node, int parent,
                vector<vector<int>>& adj,
                vector<int>& bobTime,
                int time) {
        if (node == 0) {
            bobTime[node] = time;
            return true;
        }

        for (auto v : adj[node]) {
            if (v == parent)
                continue;

            if (dfsBob(v, node, adj, bobTime, time + 1)) {
                bobTime[node] = time;
                return true;
            }
        }

        return false;
    }

    void dfsAlice(int node, int parent,
                  vector<vector<int>>& adj,
                  vector<int>& bobTime,
                  vector<int>& amount,
                  int time,
                  int currAmount,
                  int& ans) {
        if (time < bobTime[node]) {
            currAmount += amount[node];
        } else if (time == bobTime[node]) {
            currAmount += amount[node] / 2;
        }

        if (adj[node].size() == 1 && node != 0) {
            ans = max(ans, currAmount);
            return;
        }

        for (auto v : adj[node]) {
            if (v != parent) {
                dfsAlice(v, node, adj, bobTime, amount,
                         time + 1, currAmount, ans);
            }
        }
    }

    int mostProfitablePath(vector<vector<int>>& edges, int bob,
                           vector<int>& amount) {
        int n = edges.size() + 1;

        vector<vector<int>> adj(n);

        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<int> bobTime(n, INT_MAX);

        dfsBob(bob, -1, adj, bobTime, 0);

        int ans = INT_MIN;

        dfsAlice(0, -1, adj, bobTime, amount, 0, 0, ans);

        return ans;
    }
};
