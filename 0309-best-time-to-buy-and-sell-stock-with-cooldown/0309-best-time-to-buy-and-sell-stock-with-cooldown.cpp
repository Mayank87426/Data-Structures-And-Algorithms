// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach uses recursion to explore all possible choices (buy, sell, or skip)
// on each day. If we choose to buy, we deduct the current stock price and transition to the next
// day in the 'sell' state. If we sell, we add the current price and transition to day 'i + 2'
// due to the 1-day cooldown rule. Skipping simply transitions to the next day with the same state.

// Algorithm:
// 1. Define a helper function `solve(i, buy)` that returns the maximum profit from day `i` onwards.
// 2. If `i` is out of bounds (greater than or equal to size), return 0 profit.
// 3. If `buy` is true, we can either:
//    - Buy: `-prices[i] + solve(i + 1, false)`
//    - Skip: `solve(i + 1, true)`
//    Take the maximum of these two decisions.
// 4. If `buy` is false, we can either:
//    - Sell: `prices[i] + solve(i + 2, true)` (cooldown skips day i+1)
//    - Skip: `solve(i + 1, false)`
//    Take the maximum of these two decisions.
// 5. Start the recursion from day 0 with `buy = true`.

// Time Complexity:
// O(2^N) - At each step we make two recursive choices, leading to an exponential state tree.

// Space Complexity:
// O(N) - The recursive call stack can grow up to depth N in the worst case.

namespace BruteForce {
class Solution {
private:
    // Helper function to recursively compute max profit starting from day i
    int solve(int i, int buy, vector<int>& prices) {
        // Base case: if we exceed the days list, no more profit can be made
        if (i >= prices.size()) {
            return 0;
        }
        
        if (buy) {
            // We can either buy today and switch to sell state for next day, or skip today and stay in buy state
            return max(-prices[i] + solve(i + 1, 0, prices), solve(i + 1, 1, prices));
        } else {
            // We can either sell today and switch to buy state after a cooldown day (i + 2), or skip today and stay in sell state
            return max(prices[i] + solve(i + 2, 1, prices), solve(i + 1, 0, prices));
        }
    }
public:
    int maxProfit(vector<int>& prices) {
        // Initiate recursion from day 0 with buy permission
        return solve(0, 1, prices);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This is the original bottom-up dynamic programming (tabulation) approach. It solves the subproblems 
// iteratively starting from the last day back to day 0. By storing intermediate results in a 2D DP array,
// it avoids redundant computations found in recursion.

// Algorithm:
// 1. Create a DP table `dp` of size `(n + 2) x 2` initialized to 0.
// 2. Iterate backwards from day `n - 1` down to 0.
// 3. On day `i`, calculate:
//    - `dp[i][1]`: maximum profit if we can buy (either buy today or skip).
//    - `dp[i][0]`: maximum profit if we can sell (either sell today and jump to i+2, or skip).
// 4. Return `dp[0][1]`, which represents the maximum profit starting from day 0 with buy state.

// Time Complexity:
// O(N) - We fill a 2D table of size (N + 2) x 2, visiting each state exactly once.

// Space Complexity:
// O(N) - We allocate a DP table of size (N + 2) x 2.

namespace Better {
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        // Store the number of days in the prices vector
        int n = prices.size();
        // Create a DP table of size (n + 2) x 2 initialized to 0.
        // We use n + 2 to handle index out of bound cases (like i + 2) safely.
        // dp[i][1] represents the max profit on day i if we can buy.
        // dp[i][0] represents the max profit on day i if we can sell.
        vector<vector<int>> dp(n + 2, vector<int>(2, 0));

        // Iterate backwards from the last day to the first day (bottom-up DP)
        for (int i = n - 1; i >= 0; i--) {
            // If we are in 'buy' state (status 1) on day i:
            // We can either buy today (-prices[i] + dp[i+1][0]) or skip today (dp[i+1][1]).
            dp[i][1] = max(-prices[i] + dp[i + 1][0], dp[i + 1][1]);
            // If we are in 'sell' state (status 0) on day i:
            // We can either sell today (prices[i] + dp[i+2][1] with 1-day cooldown) or skip today (dp[i+1][0]).
            dp[i][0] = max(prices[i] + dp[i + 2][1], dp[i + 1][0]);
        }

        // Return the maximum profit starting from day 0 with buy permission
        return dp[0][1];
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Since calculating the DP values for day `i` only requires the values from day `i + 1` and day `i + 2`,
// we do not need to store the entire 2D DP table. We can optimize space by only maintaining the state 
// variables representing day `i + 1` and day `i + 2` using three 2-element vectors (or simple variables).

// Algorithm:
// 1. Maintain `front1` representing DP values for day `i + 1` and `front2` for day `i + 2`.
// 2. Iterate backwards from day `n - 1` to 0.
// 3. Compute `cur` for the current day:
//    - `cur[1] = max(-prices[i] + front1[0], front1[1])`
//    - `cur[0] = max(prices[i] + front2[1], front1[0])`
// 4. Update the state: `front2` becomes `front1`, and `front1` becomes `cur`.
// 5. Finally, return `front1[1]` which contains the result for day 0 in buy state.

// Time Complexity:
// O(N) - Single loop running from N-1 down to 0.

// Space Complexity:
// O(1) - Only a constant amount of memory is used for the state vectors.

namespace Optimal {
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        // Store the number of days in the prices vector
        int n = prices.size();
        
        // front1 represents the dp values for day i + 1: front1[0] is sell state, front1[1] is buy state
        vector<int> front1(2, 0);
        // front2 represents the dp values for day i + 2: front2[0] is sell state, front2[1] is buy state
        vector<int> front2(2, 0);
        // cur represents the dp values for the current day i
        vector<int> cur(2, 0);

        // Iterate backwards from the last day to the first day (bottom-up DP)
        for (int i = n - 1; i >= 0; i--) {
            // Max profit if we decide to buy today or skip today
            cur[1] = max(-prices[i] + front1[0], front1[1]);
            // Max profit if we decide to sell today (with cooldown jumping to day i+2) or skip today
            cur[0] = max(prices[i] + front2[1], front1[0]);

            // Shift the state variables for the next iteration: day i+2 becomes day i+1, day i+1 becomes current day
            front2 = front1;
            front1 = cur;
        }

        // Return the maximum profit starting from day 0 in buy state
        return front1[1];
    }
};
}