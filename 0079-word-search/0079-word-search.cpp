class Solution {
public:
    bool solve(vector<vector<char>>& board, string word, int i, int j,
               int idx) {
            int k=word.size();
        if (idx >= k)
            return true;
        int n = board.size(), m = board[0].size();
        if (i >= n || j >= m || i < 0 || j < 0)
            return false;
        char curr = board[i][j];
        board[i][j] = '#';
        char left = j - 1 >= 0 ? board[i][j - 1] : '$';
        char right = j + 1 < m  ? board[i][j + 1] : '$';
        char top = i - 1 >= 0 ? board[i - 1][j] : '$';
        char down = i + 1 < n ? board[i + 1][j] : '$';
        if (left == word[idx]) {
            if (solve(board, word, i, j - 1, idx + 1))
                return true;
        }
        if (right == word[idx]) {
            if (solve(board, word, i, j + 1, idx + 1))
                return true;
        }
        if (top == word[idx]) {
            if (solve(board, word, i - 1, j, idx + 1))
                return true;
        }
        if (down == word[idx]) {
            if (solve(board, word, i + 1, j, idx + 1))
                return true;
        }
        board[i][j] = curr;
        return false;
    }
    bool exist(vector<vector<char>>& board, string word) {
        int n = board.size(), m = board[0].size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (board[i][j] == word[0] && solve(board, word, i, j, 1))
                   return true;
            }
        }
        return false;
    }
};