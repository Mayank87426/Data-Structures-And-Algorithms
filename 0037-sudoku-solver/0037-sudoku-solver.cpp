// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach uses backtracking to fill empty cells one by one.
// For each empty cell (represented by '.'), it tries to place all digits from '1' to '9'.
// To check if a digit placement is valid, it scans the entire row, column, and the 
// respective 3x3 subgrid. If no conflict is found, it recursively tries to solve the rest of the board.
// If a placement leads to a dead end, it backtracks by resetting the cell to '.' and trying the next digit.

// Algorithm:
// 1. Traverse the board cell by cell.
// 2. If the column index reaches 9, proceed to the start of the next row.
// 3. If the row index reaches 9, the board is successfully solved; return true.
// 4. If the current cell is not empty, recursively move to the next cell.
// 5. If the current cell is empty ('.'), try placing digits '1' to '9'.
// 6. For each digit, check its validity across the row, column, and 3x3 subgrid:
//    - If valid, temporarily assign the digit and recursively call solve for the next cell.
//    - If the recursion returns true, propagate the success up.
//    - If the recursion returns false, backtrack by resetting the cell to '.'.
// 7. If no digit works, return false.

// Time Complexity:
// O(9^N) where N is the number of empty cells. Since the grid size is fixed at 9x9, there are at most 81 empty cells.
// In the worst case, checking validity takes O(9) operations per placement, resulting in O(9 * 9^81) operations.

// Space Complexity:
// O(N) where N is the number of empty cells (up to 81), representing the maximum recursion depth of the call stack.

namespace BruteForce {
class Solution {
public:
    // Helper function to check if placing `val` at cell (i, j) is valid
    bool isValid(vector<vector<char>>& board, int i, int j, char val) {
        // Iterate through all columns in the current row `i`
        for (int col = 0; col < 9; col++) {
            // If the value already exists in the row, the placement is invalid
            if (board[i][col] == val)
                return false;
        }

        // Iterate through all rows in the current column `j`
        for (int row = 0; row < 9; row++) {
            // If the value already exists in the column, the placement is invalid
            if (board[row][j] == val)
                return false;
        }

        // Calculate the starting row index of the 3x3 subgrid containing cell (i, j)
        int startRow = (i / 3) * 3;
        // Calculate the starting column index of the 3x3 subgrid containing cell (i, j)
        int startCol = (j / 3) * 3;

        // Iterate through the 3x3 subgrid
        for (int row = startRow; row < startRow + 3; row++) {
            for (int col = startCol; col < startCol + 3; col++) {
                // If the value already exists in the subgrid, the placement is invalid
                if (board[row][col] == val)
                    return false;
            }
        }

        // The placement is valid since no conflicts were found
        return true;
    }

    // Backtracking function to solve the board cell-by-cell
    bool solve(vector<vector<char>>& board, int i, int j) {
        // If we have reached the end of the current row (column index 9), proceed to the start of the next row
        if (j == 9)
            return solve(board, i + 1, 0);

        // If we have successfully traversed past the last row (index 9), the board is fully solved
        if (i == 9)
            return true;

        // If the current cell is already filled, skip it and solve for the next cell in the row
        if (board[i][j] != '.')
            return solve(board, i, j + 1);

        // Try placing digits from '1' to '9' in the current empty cell
        for (char ch = '1'; ch <= '9'; ch++) {
            // Check if placing character `ch` is valid under Sudoku rules
            if (isValid(board, i, j, ch)) {
                // Temporarily place the character on the board
                board[i][j] = ch;

                // Recurse to solve the next cell, and return true if a valid solution is found
                if (solve(board, i, j + 1))
                    return true;

                // Undo the placement (backtrack) if the recursive call did not find a solution
                board[i][j] = '.';
            }
        }

        // Return false if no digit from '1' to '9' leads to a valid solution
        return false;
    }

    // Main entry point function to solve the Sudoku board
    void solveSudoku(vector<vector<char>>& board) {
        // Start the backtracking search from the top-left corner (0, 0)
        bool f = solve(board, 0, 0);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach optimizes the validation check step. Instead of scanning
// rows, columns, and 3x3 subgrids in O(9) time on every check, we pre-calculate
// the existence of digits using boolean arrays.
// Specifically, we use `rows[9][10]`, `cols[9][10]`, and `boxes[3][3][10]` to record
// whether a digit has already been placed in a row, column, or 3x3 subgrid.
// This reduces the validation lookup step to O(1) time.

// Algorithm:
// 1. Initialize tracking arrays `rows`, `cols`, and `boxes` to `false`.
// 2. Scan the initial board to register pre-filled digits in these tracking arrays.
// 3. Perform cell-by-cell backtracking starting from (0, 0).
// 4. At each empty cell, iterate from digit 1 to 9:
//    - If the digit is not present in the current row, column, or subgrid (O(1) check):
//      - Place the digit, update the tracking arrays to true, and recurse.
//      - If the recursion returns true, propagate the success.
//      - Otherwise, backtrack by resetting the cell and updating tracking arrays to false.
// 5. Return false if no valid digit can be placed.

// Time Complexity:
// O(9^N) where N is the number of empty cells. While the overall worst-case complexity remains 
// exponential, the validation checks are optimized to O(1), leading to a significant constant-factor speedup.

// Space Complexity:
// O(N) for the recursion stack and O(1) auxiliary space for the fixed-size tracking arrays (9 * 10 * 3 = 270 booleans).

namespace Better {
class Solution {
private:
    // Tracks the presence of digits 1-9 in each row
    bool rows[9][10];
    // Tracks the presence of digits 1-9 in each column
    bool cols[9][10];
    // Tracks the presence of digits 1-9 in each 3x3 subgrid
    bool boxes[3][3][10];

    // Backtracking function to solve the board cell-by-cell
    bool solve(vector<vector<char>>& board, int i, int j) {
        // If we reach the end of the current row, move to the next row
        if (j == 9)
            return solve(board, i + 1, 0);

        // If we have successfully traversed all rows, the board is solved
        if (i == 9)
            return true;

        // Skip cells that are already filled
        if (board[i][j] != '.')
            return solve(board, i, j + 1);

        // Calculate the subgrid indices
        int boxRow = i / 3;
        int boxCol = j / 3;

        // Try placing digits from 1 to 9
        for (int num = 1; num <= 9; num++) {
            // Check in O(1) time if placing `num` is valid
            if (!rows[i][num] && !cols[j][num] && !boxes[boxRow][boxCol][num]) {
                // Set state to true (placed)
                rows[i][num] = true;
                cols[j][num] = true;
                boxes[boxRow][boxCol][num] = true;
                board[i][j] = '0' + num;

                // Recurse to solve the next cell
                if (solve(board, i, j + 1))
                    return true;

                // Backtrack on failure
                board[i][j] = '.';
                rows[i][num] = false;
                cols[j][num] = false;
                boxes[boxRow][boxCol][num] = false;
            }
        }

        // Return false if no digits from 1 to 9 can solve the Sudoku
        return false;
    }

public:
    void solveSudoku(vector<vector<char>>& board) {
        // Reset the tracking arrays
        for (int i = 0; i < 9; i++) {
            for (int d = 1; d <= 9; d++) {
                rows[i][d] = false;
                cols[i][d] = false;
            }
        }
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                for (int d = 1; d <= 9; d++) {
                    boxes[r][c][d] = false;
                }
            }
        }

        // Populate initial constraints from the given board
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '0';
                    rows[i][num] = true;
                    cols[j][num] = true;
                    boxes[i / 3][j / 3][num] = true;
                }
            }
        }

        // Start backtracking from the top-left cell
        solve(board, 0, 0);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach integrates bitmasking and tracks only the empty cells to achieve maximum efficiency.
// 1. Bitmasking: We use integers `rows[9]`, `cols[9]`, and `boxes[9]` as bitmasks. The d-th bit is set
//    if the digit d is present. This uses minimal memory and allows fast bitwise operations.
// 2. Empty Cell Vector: We gather coordinates of all empty cells beforehand. Instead of traversing 
//    every cell and checking if it is empty, we only backtrack over the empty cells, reducing recursion overhead.
// 3. Fast Bitwise Validation:
//    - `occupied = rows[r] | cols[c] | boxes[boxIdx]` tells us which numbers are taken.
//    - `valids = ~occupied & 0x3FE` gives us all available numbers as set bits (from bit 1 to 9).
//    - We extract the lowest set bit using `valids & -valids` to try the next valid number immediately.

// Algorithm:
// 1. Pre-process the board:
//    - Populate the bitmasks for rows, columns, and 3x3 subgrids.
//    - Keep track of empty cell coordinates in a vector of pairs.
// 2. Perform backtracking using the list of empty cells, represented by index `idx`.
// 3. For each empty cell `(r, c)`:
//    - Compute the mask of valid candidates: `valids = ~(rows[r] | cols[c] | boxes[boxIdx]) & 0x3FE`.
//    - Try candidate digits corresponding to the set bits in `valids`.
//    - Update the bitmasks and recursively solve for the next empty cell `idx + 1`.
//    - If the recursive call returns true, propagate it.
//    - On failure, backtrack by clearing the bits in masks and resetting the board cell.
// 4. Return true once all empty cells are successfully filled.

// Time Complexity:
// O(9^N) where N is the number of empty cells. Bitwise operations are extremely fast (taking constant O(1) time),
// and skipping filled cells reduces recursion overhead, resulting in 0ms execution in practice.

// Space Complexity:
// O(N) where N is the number of empty cells (up to 81) to store the empty cell coordinates and the call stack.

namespace Optimal {
class Solution {
private:
    // Bitmask for each row (bit d is set if digit d is placed)
    int rows[9];
    // Bitmask for each column (bit d is set if digit d is placed)
    int cols[9];
    // Bitmask for each 3x3 box (bit d is set if digit d is placed)
    int boxes[9];

    // Backtracking function iterating only over the collected empty cells
    bool solve(vector<vector<char>>& board, int idx, const vector<pair<int, int>>& emptyCells) {
        // If we have successfully filled all empty cells, the board is solved
        if (idx == emptyCells.size())
            return true;

        // Retrieve row and column indices for the current empty cell
        int r = emptyCells[idx].first;
        int c = emptyCells[idx].second;
        // Map 2D grid coordinates to a 1D subgrid index (0 to 8)
        int boxIdx = (r / 3) * 3 + (c / 3);

        // Get the bitmask of all valid digits (1-9) that can be placed in cell (r, c)
        // 0x3FE (binary 1111111110) restricts valid digits to bits 1 through 9
        int valids = ~(rows[r] | cols[c] | boxes[boxIdx]) & 0x3FE;

        // Try every valid candidate digit
        while (valids > 0) {
            // Extract the lowest set bit representing the candidate digit
            int lsbit = valids & -valids;
            // Remove the candidate bit from the set of valid options
            valids ^= lsbit;

            // Determine the digit value based on the position of the set bit
            int digit = 1;
            int temp = lsbit >> 1;
            while (temp > 1) {
                temp >>= 1;
                digit++;
            }

            // Temporarily place the digit in the board cell
            board[r][c] = '0' + digit;
            // Update the bitmasks to mark the digit as placed
            rows[r] |= lsbit;
            cols[c] |= lsbit;
            boxes[boxIdx] |= lsbit;

            // Recurse to fill the next empty cell
            if (solve(board, idx + 1, emptyCells))
                return true;

            // Undo the placement and clear the bitmasks on failure (backtrack)
            board[r][c] = '.';
            rows[r] ^= lsbit;
            cols[c] ^= lsbit;
            boxes[boxIdx] ^= lsbit;
        }

        // Return false if no candidate digit solved the Sudoku
        return false;
    }

public:
    void solveSudoku(vector<vector<char>>& board) {
        // Initialize tracking bitmasks to 0 (no digits placed)
        for (int i = 0; i < 9; i++) {
            rows[i] = 0;
            cols[i] = 0;
            boxes[i] = 0;
        }

        // Vector to store coordinates of all empty cells
        vector<pair<int, int>> emptyCells;

        // Traverse the initial board to populate bitmasks and identify empty cells
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    // Extract the digit value
                    int digit = board[i][j] - '0';
                    // Create the bitmask for the digit (1 << digit)
                    int mask = 1 << digit;
                    // Update masks for row, column, and subgrid
                    rows[i] |= mask;
                    cols[j] |= mask;
                    boxes[(i / 3) * 3 + (j / 3)] |= mask;
                } else {
                    // Store coordinates of the empty cell
                    emptyCells.push_back({i, j});
                }
            }
        }

        // Begin backtracking from the first empty cell
        solve(board, 0, emptyCells);
    }
};
}