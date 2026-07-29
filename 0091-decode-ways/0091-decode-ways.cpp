// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach uses recursion to explore all possible decodings.
// At each index, we have up to two choices:
// 1. Decode a single digit if it is not '0'.
// 2. Decode two digits if the substring of length 2 represents a number between 10 and 26.
// The total number of ways is the sum of decodings possible from both branches.

// Algorithm:
// 1. Define a helper function to recursively decode from index i.
// 2. Base case: If we reach the end of the string (i == length), we return 1 (found a valid decoding path).
// 3. If the current character is '0', it cannot start any valid code, so we return 0.
// 4. Recursively find decodings for the rest of the string by decoding one character (i.e. solve(i + 1)).
// 5. If we have at least 2 characters left, check if the two-digit value is between 10 and 26. If so, add solve(i + 2).
// 6. Return the total number of ways.

// Time Complexity:
// O(2^N) where N is the length of the string, because we can branch up to two times at each position.

// Space Complexity:
// O(N) due to the recursion call stack depth.

namespace BruteForce {
class Solution {
private:
    // Helper function to recursively find the number of ways to decode from index i.
    int solve(int i, const string& s) {
        // Base case: if we reach the end of the string, a valid decoding sequence is completed.
        if (i == s.length()) {
            return 1;
        }
        // A leading '0' is invalid and cannot be decoded.
        if (s[i] == '0') {
            return 0;
        }
        // Recursively count decodings by taking a single character.
        int ways = solve(i + 1, s);
        // Check if there are at least two characters left to form a double-digit number.
        if (i + 1 < s.length()) {
            // Compute the numerical value of the 2-digit number starting at index i.
            int val = (s[i] - '0') * 10 + (s[i + 1] - '0');
            // If the value is a valid code between 10 and 26, add the decodings from index i + 2.
            if (val >= 10 && val <= 26) {
                ways += solve(i + 2, s);
            }
        }
        // Return the total ways calculated for the current index i.
        return ways;
    }
public:
    int numDecodings(string s) {
        // Kick off the recursion from the start of the string (index 0).
        return solve(0, s);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This approach is bottom-up dynamic programming (tabulation) using a DP array.
// To avoid redundant computations of the same index multiple times, we store the result 
// of each index in a `dp` vector. Since the subproblems depend on subsequent indices,
// we fill the table backwards from the end of the string to the beginning.

// Algorithm:
// 1. Initialize a `dp` array of size `n + 1` with 0, where `dp[i]` stores the number of decodings from index `i`.
// 2. Set the base case `dp[n] = 1` for empty suffix.
// 3. Iterate backwards from `n - 1` to 0.
// 4. If the current character is '0', skip (it remains 0).
// 5. Otherwise, set `dp[i]` to `dp[i + 1]` (single-digit choice).
// 6. If a double-digit number can be formed (between 10 and 26) starting at index `i`, add `dp[i + 2]` to `dp[i]`.
// 7. Return `dp[0]`.

// Time Complexity:
// O(N) where N is the length of the string, as we compute the answer for each index exactly once.

// Space Complexity:
// O(N) to store the DP array of size N + 1.

namespace Better {
class Solution {
public:
    int numDecodings(string s) {
        // Get the length of the string to size the DP array and bound our loop.
        int n = s.length();
        // Create a dp array of size n + 1, initialized to 0, to hold the decoding counts.
        vector<int> dp(n + 1, 0);
        // Base case: There is exactly 1 way to decode an empty suffix at index n.
        dp[n] = 1;
        // Loop backwards from the end of the string to build bottom-up solutions.
        for (int i = n - 1; i >= 0; i--) {
            // A leading '0' at the current position is invalid and cannot be decoded.
            if (s[i] == '0')
                continue;
            // Get the count of decodings if we decode only one character at the current index.
            int oneChar = +dp[i + 1];
            // Accumulate the single-character decodings into the current index's total.
            dp[i] += oneChar;
            // Check if there is space for a two-character code starting at the current index.
            if (i < n - 1) {
                // Slice a substring of length 2 from the current index.
                string str = s.substr(i, 2);
                // Convert the substring to an integer to validate its range.
                int v = stoi(str);
                // If the value is a valid code between 10 and 26, add the ways from two steps ahead.
                if (v >= 10 && v <= 26) {
                    dp[i] += dp[i + 2];
                }
            }
        }
        // Return the accumulated decoding count starting from index 0.
        return dp[0];
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// This is the space-optimized version of the bottom-up dynamic programming approach.
// We observe that `dp[i]` only depends on `dp[i + 1]` and `dp[i + 2]`.
// Hence, instead of maintaining a full array of size `N + 1`, we can use two variables
// to keep track of the results for the next two states, reducing the space complexity to O(1).

// Algorithm:
// 1. Initialize `next1 = 1` (equivalent to dp[n]) and `next2 = 0` (equivalent to dp[n+1]).
// 2. Iterate backwards from `n - 1` to 0.
// 3. For each character, if it is '0', the number of decodings from index `i` is 0.
// 4. Otherwise, compute the current number of ways by adding `next1` (single digit decode).
// 5. If we can form a double-digit number between 10 and 26 with the next character, add `next2`.
// 6. Update `next2` to `next1`, and `next1` to the current computed ways.
// 7. Return `next1` at the end.

// Time Complexity:
// O(N) where N is the length of the string, since we iterate through the string of length N once.

// Space Complexity:
// O(1) as we only use a constant number of variables to store the states.

namespace Optimal {
class Solution {
public:
    int numDecodings(string s) {
        // Store the length of the input string to control the iteration boundaries.
        int n = s.length();
        // If the string is empty, it cannot be decoded.
        if (n == 0) return 0;
        // next1 stores the number of decodings for index i + 1, initially 1 for the end of the string.
        int next1 = 1;
        // next2 stores the number of decodings for index i + 2, initially 0.
        int next2 = 0;
        // Iterate backwards from the second-to-last character to the first.
        for (int i = n - 1; i >= 0; i--) {
            // Variable to accumulate decoding ways starting at index i.
            int current = 0;
            // Only decode if the current character is not '0', as a leading '0' is invalid.
            if (s[i] != '0') {
                // Add ways from single-digit decoding using next1 (equivalent to dp[i + 1]).
                current += next1;
                // If there's a next character, check if a valid double digit (10-26) can be formed.
                if (i < n - 1) {
                    // Extract the integer value of the 2-digit number starting at index i.
                    int val = (s[i] - '0') * 10 + (s[i + 1] - '0');
                    // Check if the 2-digit value is within the valid range of 10 to 26.
                    if (val >= 10 && val <= 26) {
                        // Add ways from double-digit decoding using next2 (equivalent to dp[i + 2]).
                        current += next2;
                    }
                }
            }
            // Shift state values: the value for i + 2 becomes the old value for i + 1.
            next2 = next1;
            // The value for i + 1 becomes the newly computed current value.
            next1 = current;
        }
        // Return the final result stored in next1, representing decodings from index 0.
        return next1;
    }
};
}