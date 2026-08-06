class Solution {
public:
    int n, m;
    vector<vector<int>> dir = {{0,1},{1,0},{0,-1},{-1,0}};

    void dfs(vector<vector<int>>& matrix, vector<vector<int>>& dist, int i, int j) {
        if (dist[i][j] != 1)
            return;

        for (auto &d : dir) {
            int new_i = i + d[0];
            int new_j = j + d[1];

            if (new_i < 0 || new_j < 0 || new_i >= n || new_j >= m)
                continue;

            if (matrix[new_i][new_j] <= matrix[i][j])
                continue;

            if (dist[new_i][new_j] == 1)
                dfs(matrix, dist, new_i, new_j);

            dist[i][j] = max(dist[i][j], dist[new_i][new_j] + 1);
        }
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
        n = matrix.size();
        m = matrix[0].size();

        vector<vector<int>> dist(n, vector<int>(m, 1));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (dist[i][j] == 1)
                    dfs(matrix, dist, i, j);
            }
        }

        int ans = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans = max(ans, dist[i][j]);
            }
        }

        return ans;
    }
};