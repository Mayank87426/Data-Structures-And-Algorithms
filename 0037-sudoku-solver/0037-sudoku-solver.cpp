class Solution {
public:
    bool isValid(vector<vector<char>>& board, int i, int j, char val) {
        for (int col = 0; col < 9; col++) {
            if (board[i][col] == val)
                return false;
        }

        for (int row = 0; row < 9; row++) {
            if (board[row][j] == val)
                return false;
        }

        int startRow = (i / 3) * 3;
        int startCol = (j / 3) * 3;

        for (int row = startRow; row < startRow + 3; row++) {
            for (int col = startCol; col < startCol + 3; col++) {
                if (board[row][col] == val)
                    return false;
            }
        }

        return true;
    }
    bool solve(vector<vector<char>>& board, int i, int j) {
        if (j == 9)
            return solve(board, i + 1, 0);

        if (i == 9)
            return true;

        if (board[i][j] != '.')
            return solve(board, i, j + 1);

        for (char ch = '1'; ch <= '9'; ch++) {
            if (isValid(board, i, j, ch)) {
                board[i][j] = ch;

                if (solve(board, i, j + 1))
                    return true;

                board[i][j] = '.';
            }
        }

        return false;
    }
    void solveSudoku(vector<vector<char>>& board) {
        bool f = solve(board, 0, 0);
    }
};