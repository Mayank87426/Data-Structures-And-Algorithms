// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach recursively generates all possible partitions of the array indices.
// For each element in the input array, we have three choices:
// 1. Skip the element and do not include it in either subsequence.
// 2. Include the element in the first subsequence, updating its GCD.
// 3. Include the element in the second subsequence, updating its GCD.
// We recursively branch for each choice. At the end of the array (base case), we check if 
// both subsequences are non-empty and if their GCDs are equal.
// Since we explore all 3 possibilities for each of the N elements, the total states explored 
// is 3^N, leading to an exponential time complexity which will result in a Time Limit Exceeded (TLE).

// Algorithm:
// 1. Define a helper recursive function `dfs(nums, i, gcd1, gcd2)`.
// 2. If index `i` reaches the end of `nums`, return 1 if `gcd1 != 0` (non-empty first subsequence), 
//    `gcd2 != 0` (non-empty second subsequence), and `gcd1 == gcd2`. Otherwise, return 0.
// 3. Recurse for the three choices:
//    - Choice 1: `dfs(nums, i + 1, gcd1, gcd2)`
//    - Choice 2: `dfs(nums, i + 1, gcd(gcd1, nums[i]), gcd2)`
//    - Choice 3: `dfs(nums, i + 1, gcd1, gcd(gcd2, nums[i]))`
// 4. Sum the results of the three choices modulo 10^9 + 7 and return.

// Time Complexity:
// O(3^N) where N is the number of elements in nums, due to the 3-way branching at each step.

// Space Complexity:
// O(N) auxiliary space for the recursion call stack.

namespace BruteForce {
class Solution {
public:
    // Modulo constant used to keep values within bounds and prevent overflow.
    static constexpr int MOD = 1'000'000'007;

    // Helper recursive function to explore all possible subsequence partitions without caching.
    int dfs(vector<int>& nums, int i, int gcd1, int gcd2) {
        // Base case: check if we have reached the end of the array.
        if (i == nums.size()) {
            // Both subsequences must be non-empty (gcd != 0) and have the same GCD value.
            return (gcd1 != 0 && gcd2 != 0 && gcd1 == gcd2);
        }

        // Choice 1: Skip the current element entirely, keeping the GCDs of both subsequences unchanged.
        int x = dfs(nums, i + 1, gcd1, gcd2);

        // Choice 2: Add the current element to the first subsequence, updating its GCD.
        int y = dfs(nums, i + 1, __gcd(gcd1, nums[i]), gcd2);

        // Choice 3: Add the current element to the second subsequence, updating its GCD.
        int z = dfs(nums, i + 1, gcd1, __gcd(gcd2, nums[i]));

        // Combine the results from the three mutually exclusive choices.
        int ans = x;
        // Add the possibilities from the second choice modulo MOD to handle large numbers.
        ans = (1LL * ans + y) % MOD;
        // Add the possibilities from the third choice modulo MOD to handle large numbers.
        ans = (1LL * ans + z) % MOD;
        // Return the accumulated count of valid pairs.
        return ans;
    }

    // Main method to trigger the search.
    int subsequencePairCount(vector<int>& nums) {
        // Start recursive generation from index 0 with GCDs initialized to 0.
        return dfs(nums, 0, 0, 0);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach uses memoization (top-down dynamic programming) on top of the brute force recursion.
// Notice that the state of our recursion is uniquely identified by the tuple (index, gcd1, gcd2).
// Since the array size N is at most 200, and the values in the array are also at most 200, the GCDs of 
// any subsequences can only take values between 0 and 200.
// Thus, the number of unique states is bounded by 200 * 200 * 200 = 8 * 10^6, which is small enough to fit in memory.
// By caching the result of each state in a 3D table `t[201][201][201]`, we avoid repeating calculations 
// for previously visited states, reducing the time complexity to polynomial.

// Algorithm:
// 1. Initialize a 3D memoization array `t[201][201][201]` with -1.
// 2. Define the recursive function `dfs(nums, i, gcd1, gcd2)`.
// 3. If the base case is reached (index `i == nums.size()`), check if both GCDs are non-zero and equal.
// 4. Before calculating a state, check if `t[i][gcd1][gcd2]` is already calculated (not -1). If so, return it.
// 5. Otherwise, calculate the transitions for:
//    - Skipping the element: `dfs(nums, i + 1, gcd1, gcd2)`
//    - Including in first subsequence: `dfs(nums, i + 1, gcd(gcd1, nums[i]), gcd2)`
//    - Including in second subsequence: `dfs(nums, i + 1, gcd1, gcd(gcd2, nums[i]))`
// 6. Sum the transitions modulo 10^9 + 7, store the result in `t[i][gcd1][gcd2]`, and return it.

// Time Complexity:
// O(N * M^2) where N is the size of the array (up to 200) and M is the maximum value in nums (up to 200).
// At each of the N * M^2 states, we do O(1) state transitions (with GCD computation taking logarithmic time).

// Space Complexity:
// O(N * M^2) to store the 3D memoization table, plus O(N) for recursion stack.

namespace Better {
class Solution {
public:
    // The divisor modulo value as specified by the problem constraints to prevent integer overflow.
    static constexpr int MOD = 1'000'000'007;
    // 3D memoization array to store results for state (index, gcd1, gcd2) where index <= 200 and GCD <= 200.
    int t[201][201][201];

    // Recursive depth-first search with memoization to find the count of valid subsequences.
    int dfs(vector<int>& nums, int i, int gcd1, int gcd2) {
        // Base case: if we processed all elements, check if both subsequences are non-empty and have equal GCD.
        if (i == nums.size()) {
            // Return 1 if both GCDs are non-zero (meaning non-empty) and equal, otherwise return 0.
            return (gcd1 != 0 && gcd2 != 0 && gcd1 == gcd2);
        }

        // Check if the result for the current state has already been computed to avoid redundant calculations.
        if (t[i][gcd1][gcd2] != -1)
            // Return the cached result to achieve polynomial time complexity.
            return t[i][gcd1][gcd2];

        // Scenario 1: Exclude the current element from both subsequences and proceed to the next element.
        int x = dfs(nums, i + 1, gcd1, gcd2);

        // Scenario 2: Include the current element in the first subsequence, updating its GCD.
        int y = dfs(nums, i + 1, __gcd(gcd1, nums[i]), gcd2);

        // Scenario 3: Include the current element in the second subsequence, updating its GCD.
        int z = dfs(nums, i + 1, gcd1, __gcd(gcd2, nums[i]));

        // Initialize the answer with the number of ways from the exclusion scenario.
        int ans = x;
        // Accumulate ways from including the element in the first subsequence, taking modulo to prevent overflow.
        ans = (1LL * ans + y) % MOD;
        // Accumulate ways from including the element in the second subsequence, taking modulo to prevent overflow.
        ans = (1LL * ans + z) % MOD;
        // Cache the computed answer for the current state and return it.
        return t[i][gcd1][gcd2] = ans;
    }

    // Main function to initiate the recursion and return the subsequence pair count.
    int subsequencePairCount(vector<int>& nums) {
        // Initialize the memoization table with -1 to indicate that no states have been calculated yet.
        memset(t, -1, sizeof(t));
        // Start the DFS traversal from index 0 with initial GCDs as 0 (representing empty subsequences).
        return dfs(nums, 0, 0, 0);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach uses bottom-up dynamic programming with space optimization.
// Since the DP transition at index `i` only depends on the results from index `i - 1`, we can optimize 
// the space by maintaining only the current DP state and a temporary next DP state.
// We define `dp[g1][g2]` as the number of subsequence pairs formed so far with GCDs `g1` and `g2`.
// This reduces the space complexity from O(N * M^2) to O(M^2), which is extremely small (201 * 201 * 4 bytes ≈ 160 KB).
// Furthermore, we skip states where `dp[g1][g2] == 0`, significantly improving performance in practice.

// Algorithm:
// 1. Declare a 2D array `dp[201][201]` initialized to 0, and set the base case `dp[0][0] = 1`.
// 2. Iterate through each `num` in `nums`:
//    a. Copy the current `dp` state to `next_dp` (representing the choice of skipping `num`).
//    b. Loop through all possible GCDs `g1` and `g2` from 0 to 200.
//    c. If `dp[g1][g2]` is greater than 0:
//       - Transition 1 (Add to seq1): `ng1 = gcd(g1, num)`. Add `dp[g1][g2]` to `next_dp[ng1][g2]`.
//       - Transition 2 (Add to seq2): `ng2 = gcd(g2, num)`. Add `dp[g1][g2]` to `next_dp[g1][ng2]`.
//    c. Update `dp` with `next_dp`.
// 3. After processing all elements, sum `dp[g][g]` for all `g` from 1 to 200 (ignoring empty subsequences).
// 4. Return the sum modulo 10^9 + 7.

// Time Complexity:
// O(N * M^2) where N is the number of elements (up to 200) and M is the maximum value of elements (up to 200).
// In practice, it runs much faster than the worst-case bound due to pruning of unreachable states.

// Space Complexity:
// O(M^2) which is O(1) auxiliary space as the array dimensions are fixed by the maximum value constraint (200).

namespace Optimal {
class Solution {
public:
    // The divisor modulo value as specified by the problem constraints to prevent integer overflow.
    static constexpr int MOD = 1'000'000'007;

    // Iterative DP function with space optimization.
    int subsequencePairCount(vector<int>& nums) {
        // dp[g1][g2] stores the number of subsequence pairs with GCDs g1 and g2.
        int dp[201][201];
        // next_dp is a temporary buffer to compute the next state transitions.
        int next_dp[201][201];
        // Initialize the DP array to 0.
        memset(dp, 0, sizeof(dp));
        // Base case: 1 way to have both subsequences empty (GCDs of 0).
        dp[0][0] = 1;

        // Iterate through each number in the input array.
        for (int num : nums) {
            // Copy current DP state to next_dp as the default choice of not including 'num'.
            memcpy(next_dp, dp, sizeof(dp));
            // Iterate over all possible GCDs for the first subsequence.
            for (int g1 = 0; g1 <= 200; ++g1) {
                // Iterate over all possible GCDs for the second subsequence.
                for (int g2 = 0; g2 <= 200; ++g2) {
                    // Skip unreachable states to optimize performance.
                    if (dp[g1][g2] == 0) continue;

                    // Choice 1: Include 'num' in the first subsequence, updating its GCD.
                    int ng1 = __gcd(g1, num);
                    next_dp[ng1][g2] = (next_dp[ng1][g2] + dp[g1][g2]) % MOD;

                    // Choice 2: Include 'num' in the second subsequence, updating its GCD.
                    int ng2 = __gcd(g2, num);
                    next_dp[g1][ng2] = (next_dp[g1][ng2] + dp[g1][g2]) % MOD;
                }
            }
            // Update the DP state for the next iteration.
            memcpy(dp, next_dp, sizeof(dp));
        }

        // Sum up all valid pairs where both subsequences have the same non-zero GCD.
        int ans = 0;
        for (int g = 1; g <= 200; ++g) {
            // Accumulate counts for equal GCD values.
            ans = (ans + dp[g][g]) % MOD;
        }
        // Return the total count of valid subsequence pairs.
        return ans;
    }
};
}