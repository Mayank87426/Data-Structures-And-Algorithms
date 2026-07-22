class Solution {
public:
    vector<vector<int>> graph;
    vector<int> ans;
    vector<int> quiet;

    int dfs(int node) {
        if (ans[node] != -1)
            return ans[node];

        ans[node] = node;

        for (int rich : graph[node]) {
            int candidate = dfs(rich);

            if (quiet[candidate] < quiet[ans[node]])
                ans[node] = candidate;
        }

        return ans[node];
    }

    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();

        graph.resize(n);
        ans.assign(n, -1);
        this->quiet = quiet;

        for (auto& e : richer)
            graph[e[1]].push_back(e[0]);

        for (int i = 0; i < n; i++)
            dfs(i);

        return ans;
    }
};