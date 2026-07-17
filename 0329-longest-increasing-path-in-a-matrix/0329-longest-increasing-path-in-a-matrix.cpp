class Solution {
public:
    unordered_map<string, int> mp;
    int solve(vector<vector<int>>& matrix, int i, int j, int prev) {
        int n = matrix.size(), m = matrix[0].size();
        if (i < 0 || j < 0 || i >= n || j >= m || matrix[i][j] <= prev)
            return 0;
        string key =
            to_string(i) + "_" + to_string(j) + "_" + to_string(prev) + "_";
        if (mp.count(key))
            return mp[key];
        int up = solve(matrix, i - 1, j, matrix[i][j]);
        int left = solve(matrix, i, j - 1, matrix[i][j]);
        int down = solve(matrix, i + 1, j, matrix[i][j]);
        int right = solve(matrix, i, j + 1, matrix[i][j]);
        return mp[key] = 1 + max({up, left, right, down});
    }
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();
        int maxIncreasingPath = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                maxIncreasingPath =
                    max(maxIncreasingPath, solve(matrix, i, j, -1));
            }
        }
        return maxIncreasingPath;
    }
};