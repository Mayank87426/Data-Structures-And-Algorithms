// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The Brute Force approach explores all potential paths starting from each cell of the grid 
// to construct the target word using depth-first search (DFS) with backtracking.
// To ensure we do not reuse any cell during a single path traversal, we maintain an explicit 
// 2D boolean tracking grid `visited` of size M x N. If we successfully match all characters 
// of the word, we return true. Otherwise, we backtrack by marking the cell as unvisited.

// Algorithm:
// 1. Initialize a 2D boolean grid `visited` with false of size M x N.
// 2. Iterate through each cell (i, j) of the grid to check if it matches the first letter of the word.
// 3. For each potential start, invoke DFS with parameters (board, word, i, j, index = 0, visited).
// 4. In the DFS function:
//    a. If index equals the length of the word, return true (word matched).
//    b. If (i, j) is out of bounds, already visited, or doesn't match `word[index]`, return false.
//    c. Mark the cell as visited.
//    d. Recurse into the four directions (down, up, right, left) with `index + 1`.
//    e. If any direction returns true, return true.
//    f. Otherwise, unmark `visited[i][j]` (backtrack) and return false.
// 5. If no start cell leads to a matched word, return false.

// Time Complexity:
// O(M * N * 3^L) where M and N are board dimensions and L is the length of the word. 
// At each step of the search, we have up to 3 directions to explore (excluding the cell we just came from).

// Space Complexity:
// O(M * N) to store the auxiliary `visited` table, plus O(L) for the recursion call stack.

namespace BruteForce {
class Solution {
private:
    // Helper function for standard DFS backtracking using an explicit visited matrix
    bool dfs(vector<vector<char>>& board, const string& word, int i, int j, int idx, vector<vector<bool>>& visited) {
        // If the index equals the word length, we have successfully matched the entire word
        if (idx == word.length()) {
            return true;
        }
        // Retrieve the grid dimensions to perform boundary checks
        int n = board.size();
        int m = board[0].size();
        // Return false if coordinates are out of bounds, cell is already visited, or does not match the word character
        if (i < 0 || i >= n || j < 0 || j >= m || visited[i][j] || board[i][j] != word[idx]) {
            // Unsuccessful path check
            return false;
        }
        
        // Mark the current cell as visited to prevent reusing it on the current path
        visited[i][j] = true;
        
        // Recursively search in all four directions (down, up, right, left) for the next character
        if (dfs(board, word, i + 1, j, idx + 1, visited) ||
            dfs(board, word, i - 1, j, idx + 1, visited) ||
            dfs(board, word, i, j + 1, idx + 1, visited) ||
            dfs(board, word, i, j - 1, idx + 1, visited)) {
            // Return true if any direction leads to a successful match
            return true;
        }
        
        // Unmark the current cell (backtrack) to allow other paths to visit it
        visited[i][j] = false;
        // Return false as no valid path could be formed from the current state
        return false;
    }

public:
    // Main function to check the word's existence on the board
    bool exist(vector<vector<char>>& board, string word) {
        // Retrieve board dimensions
        int n = board.size();
        int m = board[0].size();
        // Create an explicit visited tracking matrix initialized to false
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        // Iterate through all cells to find a starting cell for DFS search
        for (int i = 0; i < n; ++i) {
            // Iterate columns of current row
            for (int j = 0; j < m; ++j) {
                // If a path matching the word is found starting from (i, j), return true
                if (dfs(board, word, i, j, 0, visited)) {
                    return true;
                }
            }
        }
        // Return false if no starting cell yields a complete match
        return false;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach optimizes the space complexity of the backtracking search by performing 
// in-place board modification. Instead of using a separate visited matrix, we temporarily replace 
// the character of the currently visited cell with a placeholder '#' during the DFS traversal.
// We restore the original character of the cell once the recursive calls return (backtracking).
// This approach also performs inline checks of neighboring characters before making the recursive call.

// Algorithm:
// 1. Iterate through each cell (i, j) in the board.
// 2. If `board[i][j]` matches the first character of the word, trigger the recursive search helper.
// 3. In the helper:
//    a. If `idx` is equal to or greater than the word size, return true (full match achieved).
//    b. Perform boundary checks on indices (i, j); return false if out-of-bounds.
//    c. Store the current cell value in `curr` and modify the cell to '#' to mark it as visited.
//    d. Inspect neighboring cells (left, right, top, bottom) against `word[idx]`.
//    e. If a neighbor matches, make a recursive DFS call to that neighbor with `idx + 1`.
//    f. Restore the cell's original value from `curr`.
//    g. Return false if no recursive searches succeed.

// Time Complexity:
// O(M * N * 3^L) where M and N are grid dimensions and L is the length of the word.

// Space Complexity:
// O(L) representing the maximum depth of the call stack during the recursive traversal.

namespace Better {
class Solution {
public:
    // Helper function to recursively search for the word starting from cell (i, j) at index 'idx'
    bool solve(vector<vector<char>>& board, string word, int i, int j,
               int idx) {
        // Store the size of the target word to check if search is complete
        int k = word.size();
        // If the current index has reached the length of the word, it means the whole word is successfully matched
        if (idx >= k)
            return true;
        // Retrieve the grid dimensions to prevent out-of-bound cell accesses
        int n = board.size(), m = board[0].size();
        // Return false if coordinates are out of bounds to avoid invalid memory accesses
        if (i >= n || j >= m || i < 0 || j < 0)
            return false;
        // Keep track of the current cell value to restore it during the backtracking step
        char curr = board[i][j];
        // Mark the current cell as visited to prevent reusing it during this search path
        board[i][j] = '#';
        // Check the left neighbor if within bounds, otherwise assign dummy character '$'
        char left = j - 1 >= 0 ? board[i][j - 1] : '$';
        // Check the right neighbor if within bounds, otherwise assign dummy character '$'
        char right = j + 1 < m  ? board[i][j + 1] : '$';
        // Check the top neighbor if within bounds, otherwise assign dummy character '$'
        char top = i - 1 >= 0 ? board[i - 1][j] : '$';
        // Check the down neighbor if within bounds, otherwise assign dummy character '$'
        char down = i + 1 < n ? board[i + 1][j] : '$';
        // If the left cell matches the next target character in the word, recursively search left
        if (left == word[idx]) {
            // Recursively search the left cell; if it finds a valid path, return true
            if (solve(board, word, i, j - 1, idx + 1))
                return true;
        }
        // If the right cell matches the next target character in the word, recursively search right
        if (right == word[idx]) {
            // Recursively search the right cell; if it finds a valid path, return true
            if (solve(board, word, i, j + 1, idx + 1))
                return true;
        }
        // If the top cell matches the next target character in the word, recursively search up
        if (top == word[idx]) {
            // Recursively search the top cell; if it finds a valid path, return true
            if (solve(board, word, i - 1, j, idx + 1))
                return true;
        }
        // If the bottom cell matches the next target character in the word, recursively search down
        if (down == word[idx]) {
            // Recursively search the bottom cell; if it finds a valid path, return true
            if (solve(board, word, i + 1, j, idx + 1))
                return true;
        }
        // Backtrack: restore the original character of the cell so it can be used in other search paths
        board[i][j] = curr;
        // Return false if none of the neighboring paths successfully matched the remaining characters
        return false;
    }
    
    // Main entry function to check if the word exists in the board
    bool exist(vector<vector<char>>& board, string word) {
        // Retrieve the dimensions of the board
        int n = board.size(), m = board[0].size();
        // Traverse through each cell in the board to find potential starting points for the word
        for (int i = 0; i < n; i++) {
            // Traverse columns for the current row
            for (int j = 0; j < m; j++) {
                // If the cell matches the first character of the word, initiate the backtracking search
                if (board[i][j] == word[0] && solve(board, word, i, j, 1))
                   // Return true if a successful path starting from (i, j) is found
                   return true;
            }
        }
        // Return false if no starting cell leads to a valid path matching the word
        return false;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach enhances backtracking with significant search pruning.
// 1. Quick Feasibility Check: We construct a frequency table of all characters in the board and in the word. 
//    If the board doesn't contain at least the count of each character in the word, we return false immediately.
// 2. Search Direction Optimization: We compare the frequency of the first letter and the last letter of the word 
//    on the board. If the start letter is more common, we reverse the target word. This starts DFS from the 
//    rarer character, drastically reducing the search branching factor and execution time.
// Like the Better approach, we perform in-place board modification to achieve minimal space consumption.

// Algorithm:
// 1. If the length of the word is greater than the total number of cells (M * N), return false immediately.
// 2. Calculate the frequency of each character on the board and in the word. If any character in the word has 
//    a frequency greater than that on the board, return false.
// 3. Compare the frequencies of `word[0]` and `word[wordLen-1]`. If `freq(word[0]) > freq(word[wordLen-1])`, 
//    reverse the word in place to start the search from the rarer character.
// 4. Traverse the board and launch the recursive DFS search.
// 5. In the DFS function:
//    a. If index equals the length of the word, return true.
//    b. If coordinates are out of bounds or the cell doesn't match `word[idx]`, return false.
//    c. Temporarily save the cell character, replace it with '#', and search all four directions recursively.
//    d. Restore the original character, and return true if any search branch succeeded; else return false.

// Time Complexity:
// O(M * N * 3^L) worst case, but practically close to O(L) or O(M * N) for many inputs due to early-pruning and 
// search path reduction via starting from rarer letters.

// Space Complexity:
// O(L) for the recursion stack (since we modify the board in-place and use fixed O(1) space for letter frequency arrays).

namespace Optimal {
class Solution {
private:
    // DFS helper function that searches the board in-place for the word
    bool dfs(vector<vector<char>>& board, const string& word, int i, int j, int idx) {
        // If index matches word length, we have successfully found the whole word
        if (idx == word.length()) {
            return true;
        }
        // Retrieve board dimensions
        int n = board.size();
        int m = board[0].size();
        // Return false if out of bounds or if the current cell character doesn't match the word
        if (i < 0 || i >= n || j < 0 || j >= m || board[i][j] != word[idx]) {
            return false;
        }
        
        // Save the current character to restore it later during backtracking
        char temp = board[i][j];
        // Mark the cell as visited using a dummy character '#' to prevent re-entering it
        board[i][j] = '#';
        
        // Check all 4 neighboring cells recursively for the next character
        if (dfs(board, word, i + 1, j, idx + 1) ||
            dfs(board, word, i - 1, j, idx + 1) ||
            dfs(board, word, i, j + 1, idx + 1) ||
            dfs(board, word, i, j - 1, idx + 1)) {
            // Return true if any path matches the rest of the word
            return true;
        }
        
        // Restore the original character of the cell for other DFS paths
        board[i][j] = temp;
        // Return false since no valid paths were found starting from here
        return false;
    }

public:
    // Main function to check word existence with optimization checks
    bool exist(vector<vector<char>>& board, string word) {
        // Retrieve dimensions
        int n = board.size();
        int m = board[0].size();
        // Get the length of the target word
        int wordLen = word.length();
        // If the word is longer than the total cells in the board, it cannot exist
        if (wordLen > n * m) {
            return false;
        }
        
        // Optimization 1: Count frequency of characters in the board to check feasibility
        vector<int> boardFreq(128, 0);
        // Populating the frequency count of characters on the board
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                boardFreq[board[i][j]]++;
            }
        }
        
        // Count frequency of characters in the target word
        vector<int> wordFreq(128, 0);
        // Verify that the board contains enough occurrences of each character in the word
        for (char c : word) {
            wordFreq[c]++;
            // If the word requires more of character 'c' than available on the board, return false immediately
            if (wordFreq[c] > boardFreq[c]) {
                return false;
            }
        }
        
        // Optimization 2: Reverse search direction if the start character is more common than the end character
        if (boardFreq[word[0]] > boardFreq[word[wordLen - 1]]) {
            // Reverse the search word in place to start searching from the rarer character and minimize branching
            int left = 0, right = wordLen - 1;
            while (left < right) {
                swap(word[left++], word[right--]);
            }
        }
        
        // Search the board for the word starting from each cell
        for (int i = 0; i < n; ++i) {
            // Iterate columns of current row
            for (int j = 0; j < m; ++j) {
                // If a valid path is found starting at (i, j), return true
                if (dfs(board, word, i, j, 0)) {
                    // Successful search result
                    return true;
                }
            }
        }
        // Return false if no cell matches the start of the word or leads to a valid path
        return false;
    }
};
}