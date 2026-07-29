// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach uses recursion to explore all possible combinations of signs (+ and -) 
// for each element. At each step, we branch into two paths: assigning a positive sign or a 
// negative sign to the current number, and recursively moving to the next index.

// Algorithm:
// 1. Define a helper function `solve(nums, i, target)` that returns the number of ways to reach `target` starting from index `i`.
// 2. Base case: When index `i` reaches the size of `nums`, check if `target` is 0. If it is, we successfully found a valid expression (return 1), otherwise return 0.
// 3. Recursive step: We make two recursive calls:
//    - Assign '+' to `nums[i]`, which reduces the remaining target to `target - nums[i]`.
//    - Assign '-' to `nums[i]`, which increases the remaining target to `target + nums[i]`.
// 4. Return the sum of the results of these two calls.

// Time Complexity:
// O(2^N) where N is the number of elements in `nums`. At each index, we branch into two choices.

// Space Complexity:
// O(N) due to the depth of recursion stack which goes up to N.

namespace BruteForce {
class Solution {
public:
    int solve(vector<int>& nums, int i, int target) {
        // Store the total size of the array
        int n = nums.size();
        
        // If all numbers have been assigned a sign, check if the remaining target is exactly zero
        if (i >= n)
            return target == 0;
            
        // Recursively calculate the ways by choosing a positive sign for the current element
        int takeAsPositive = solve(nums, i + 1, target - nums[i]);
        
        // Recursively calculate the ways by choosing a negative sign for the current element
        int takeAsNeg = solve(nums, i + 1, target + nums[i]);
        
        // Return the sum of ways from both decisions
        return takeAsPositive + takeAsNeg;
    }
    
    int findTargetSumWays(vector<int>& nums, int target) {
        // Start the recursive process from index 0 with the initial target
        return solve(nums, 0, target);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The better approach introduces memoization (top-down dynamic programming) to the brute force recursion. 
// Since many states (defined by index `i` and remaining `target`) are visited repeatedly, caching the results 
// in an `unordered_map` with a key format of `"index_target"` avoids duplicate computations.

// Algorithm:
// 1. Maintain a hash map `mp` to cache results for unique state keys formatted as `"index_target"`.
// 2. Base case: When index `i` reaches the end of the array, return 1 if `target == 0`, else 0.
// 3. Before computing, check if the state `"i_target"` exists in `mp`. If yes, return the cached result.
// 4. Otherwise, compute the result by summing the ways of taking `nums[i]` as positive and taking it as negative.
// 5. Store the calculated result in `mp` and return it.

// Time Complexity:
// O(N * T) where N is the number of elements and T is the sum of elements (range of target). 
// Note that string operations for hashing map keys introduce some overhead.

// Space Complexity:
// O(N * T) to store intermediate states in the memoization table, plus O(N) for recursion stack.

namespace Better {
class Solution {
public:
    // Hash map to cache the target sum ways for a specific index and target value
    unordered_map<string, int> mp;
    
    int solve(vector<int>& nums, int i, int target) {
        // Store the total size of the array
        int n = nums.size();
        
        // Base case: if we have processed all elements, check if target has been reduced to zero
        if (i >= n)
            return target == 0;
            
        // Generate a unique string key representing the current state (index and target)
        string key = to_string(i) + "_" + to_string(target);
        
        // If the result for this state is already calculated, return the cached value
        if (mp.count(key))
            return mp[key];
            
        // Recurse by assigning a positive sign to the current element (subtracting it from target)
        int takeAsPositive = solve(nums, i + 1, target - nums[i]);
        
        // Recurse by assigning a negative sign to the current element (adding it to target)
        int takeAsNeg = solve(nums, i + 1, target + nums[i]);
        
        // Cache the sum of ways from both choices and return it
        return mp[key] = takeAsPositive + takeAsNeg;
    }
    
    int findTargetSumWays(vector<int>& nums, int target) {
        // Start the recursive process from index 0 with the initial target
        return solve(nums, 0, target);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach reduces the problem to the classic Subset Sum / Partition Problem. 
// Let P be the sum of elements assigned a '+' sign, and N be the sum of elements assigned a '-' sign.
// We have:
//   P - N = target
//   P + N = total_sum
// Adding these yields 2 * P = target + total_sum, meaning P = (target + total_sum) / 2.
// Therefore, the problem is equivalent to finding the number of subsets in the array that sum to P.
// We solve this using 1D space-optimized bottom-up Dynamic Programming.

// Algorithm:
// 1. Sum up all elements in `nums` to find `total_sum`.
// 2. Check validity: If target exceeds `total_sum` in magnitude, or if `(target + total_sum)` is odd, return 0.
// 3. Calculate target subset sum: `subsetSum = (target + total_sum) / 2`.
// 4. Initialize a DP array `dp` of size `subsetSum + 1` with 0, and set `dp[0] = 1` (one way to form sum 0).
// 5. For each element `num` in `nums`, iterate backwards from `subsetSum` down to `num` (to avoid reuse of the same element in the same step).
// 6. Update `dp[j] += dp[j - num]`.
// 7. Return `dp[subsetSum]`.

// Time Complexity:
// O(N * subsetSum) where N is the size of the array and subsetSum is (target + total_sum) / 2.

// Space Complexity:
// O(subsetSum) since we only maintain a 1D DP table.

namespace Optimal {
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        // Calculate the sum of all elements in the array
        int total_sum = 0;
        for (int num : nums) {
            total_sum += num;
        }
        
        // If the target is out of bounds or (target + total_sum) is odd, no valid partition exists
        if (abs(target) > total_sum || (target + total_sum) % 2 != 0) {
            return 0;
        }
        
        // Find target sum for the positive subset
        int subsetSum = (target + total_sum) / 2;
        
        // DP array to store the number of ways to form each sum from 0 to subsetSum
        vector<int> dp(subsetSum + 1, 0);
        
        // Base case: There is exactly one way to form a sum of 0 (using an empty subset)
        dp[0] = 1;
        
        // Iterate through each number in the array
        for (int num : nums) {
            // Update DP table backwards to ensure each number is used at most once
            for (int j = subsetSum; j >= num; --j) {
                // Add the number of ways to form (j - num) to the ways to form j
                dp[j] += dp[j - num];
            }
        }
        
        // Return the number of ways to achieve the target subset sum
        return dp[subsetSum];
    }
};
}