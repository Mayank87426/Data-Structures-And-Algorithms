class Solution {
public:
    vector<string> createGrid(int m, int n) {
        vector<vector<char>> grid(m, vector<char>(n, '.'));
        for (int i = 1; i < m; i++) {
            for (int j = 0; j < n - 1; j++) {
                grid[i][j] = '#';
            }
        }
        vector<string> ans;
        for (int i = 0; i < m; i++) {
            string str = "";
            for (int j = 0; j < n; j++) {
                str.push_back(grid[i][j]);
            }
            ans.push_back(str);
            
        }
        return ans;
    }
};