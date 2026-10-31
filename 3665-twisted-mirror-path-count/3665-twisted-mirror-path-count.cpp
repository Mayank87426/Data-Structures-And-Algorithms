class Solution {
public:
int mod=1e9+7;
    int solve(int i, int j, int m, int n, vector<vector<vector<int>>>& dp,
              vector<vector<int>>& grid, int prev) {

        if (i == 0 && j == 0)
            return 1;

        if (i < 0 || j < 0)
            return 0;

        if (dp[i][j][prev] != -1)
            return dp[i][j][prev];

        if (grid[i][j] == 0) {
            int up = solve(i - 1, j, m, n, dp, grid, 0);
            int down = solve(i, j - 1, m, n, dp, grid, 1);
            return dp[i][j][prev] = (up + down)%mod;
        } else {
            int up = 0, down = 0;
            if (prev == 0) {
                down = solve(i, j - 1, m, n, dp, grid, 1);
            }
            else{
              up = solve(i - 1, j, m, n, dp, grid, 0);  
            }
            return dp[i][j][prev] = (up + down)%mod;
        }
    }

    int uniquePaths(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        vector<vector<vector<int>>> dp(
            n, vector<vector<int>>(m, vector<int>(2, -1)));
        int prev =0;
         return solve(n - 1, m - 1, m, n, dp, grid, prev);
    }
};
