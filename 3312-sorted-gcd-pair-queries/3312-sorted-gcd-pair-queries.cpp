// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach generates all possible pair combinations from the input array.
// For each pair (nums[i], nums[j]) where 0 <= i < j < n, we calculate their Greatest
// Common Divisor (GCD). We collect all these GCD values in a vector, sort them in
// ascending order, and then answer each query by direct indexing.

// Algorithm:
// 1. Initialize a vector `gcdPairs` to store the GCD of all pairs.
// 2. Loop through all pairs (i, j) with 0 <= i < j < n.
// 3. Compute the GCD for each pair using a helper gcd function and append it to `gcdPairs`.
// 4. Sort `gcdPairs` in ascending order.
// 5. For each query q in queries, retrieve `gcdPairs[q]` and add it to the result list.
// 6. Return the result list.

// Time Complexity:
// O(N^2 * log(min_val) + N^2 * log(N^2) + Q) where N is the size of nums, min_val is the
// maximum possible GCD of a pair, and Q is the number of queries.

// Space Complexity:
// O(N^2) to store the GCD values of all N * (N - 1) / 2 pairs.

namespace BruteForce {
class Solution {
private:
    // Helper function to calculate the greatest common divisor of two integers
    long long gcd(long long a, long long b) {
        // Loop until the remainder becomes zero
        while (b != 0) {
            long long temp = b;
            b = a % b; // Find remainder
            a = temp;  // Shift divisor to dividend
        }
        return a; // GCD is the last non-zero divisor
    }

public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        int n = nums.size(); // Store size of nums
        vector<int> gcdPairs; // Vector to store GCD of all pairs
        
        // Reserve memory for all N*(N-1)/2 pairs to avoid overhead of resizing
        gcdPairs.reserve(n * (n - 1) / 2);
        
        // Iterate through all possible unique pairs of indices
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // Compute GCD for pair (nums[i], nums[j]) and store it
                gcdPairs.push_back(gcd(nums[i], nums[j]));
            }
        }
        
        // Sort all computed GCD values in ascending order
        sort(gcdPairs.begin(), gcdPairs.end());
        
        vector<int> ans; // Initialize answer vector
        ans.reserve(queries.size()); // Pre-allocate memory for queries
        
        // Retrieve the GCD value at the queried index for each query
        for (long long q : queries) {
            ans.push_back(gcdPairs[q]); // Get the element at index q
        }
        
        return ans; // Return the results
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of storing all individual pair GCDs, we can compute frequencies of elements
// in `nums`. We collect all unique numbers and iterate through every pair of unique
// numbers. For a pair of unique numbers (val_i, val_j), their GCD occurs freq[val_i] * freq[val_j]
// times. For pairs composed of the same value val_i, their GCD is val_i and occurs
// freq[val_i] * (freq[val_i] - 1) / 2 times. We accumulate these counts into a `gcd_cnt` array
// of size M + 1 (where M is the maximum element in nums). We then compute the prefix sum of
// `gcd_cnt` and answer each query using binary search. This avoids sorting a massive array and
// avoids using O(N^2) space.

// Algorithm:
// 1. Find the maximum element M in `nums`.
// 2. Count the frequency of each number using a frequency array `freq` of size M + 1.
// 3. Extract all unique elements into a list `unique_nums`.
// 4. Initialize `gcd_cnt` array of size M + 1 to store count of pairs for each GCD.
// 5. Iterate through all unique numbers. For each `val_i`, calculate pair count with itself
//    as freq[val_i] * (freq[val_i] - 1) / 2.
// 6. Iterate through all unique pairs (val_i, val_j) with i < j, calculate their GCD `g`,
//    and add freq[val_i] * freq[val_j] to `gcd_cnt[g]`.
// 7. Compute the prefix sum of `gcd_cnt` to find cumulative frequencies of GCDs.
// 8. For each query, perform a binary search (lower_bound) on the prefix sums to find
//    the GCD value.

// Time Complexity:
// O(U^2 * log(min_val) + M + Q * log(M)) where U is the number of unique elements in nums,
// M is the maximum element in nums, and Q is the number of queries.

// Space Complexity:
// O(M) to store frequencies and cumulative counts of GCDs up to the maximum element.

namespace Better {
class Solution {
private:
    // Helper function to calculate the greatest common divisor of two integers
    long long gcd(long long a, long long b) {
        while (b != 0) {
            long long temp = b;
            b = a % b; // Find remainder
            a = temp;  // Shift divisor to dividend
        }
        return a; // GCD is the last non-zero divisor
    }

public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        // Find the maximum value in nums to bound our arrays
        int m = *max_element(nums.begin(), nums.end());
        
        // Array to count occurrences of each number in nums
        vector<long long> freq(m + 1, 0);
        for (int num : nums) {
            freq[num]++; // Increment count of the number
        }
        
        // Collect all unique numbers present in nums
        vector<int> unique_nums;
        for (int i = 1; i <= m; i++) {
            if (freq[i] > 0) {
                unique_nums.push_back(i); // Add existing number to unique list
            }
        }
        
        // Array to store the count of pairs resulting in each GCD value
        vector<long long> gcd_cnt(m + 1, 0);
        int u = unique_nums.size(); // Number of unique elements
        
        // Compare every unique element to compute pairs and their GCDs
        for (int i = 0; i < u; i++) {
            int val_i = unique_nums[i]; // First unique value
            long long count_i = freq[val_i]; // Frequency of first value
            
            // If the value occurs multiple times, it can form pairs with itself
            if (count_i > 1) {
                // Number of pairs formed by identical elements is count * (count - 1) / 2
                gcd_cnt[val_i] += count_i * (count_i - 1) / 2;
            }
            
            // Check pairs formed between val_i and other distinct unique values
            for (int j = i + 1; j < u; j++) {
                int val_j = unique_nums[j]; // Second unique value
                long long count_j = freq[val_j]; // Frequency of second value
                int g = gcd(val_i, val_j); // Calculate GCD of the two values
                
                // Add the product of their frequencies to the count of GCD 'g'
                gcd_cnt[g] += count_i * count_j;
            }
        }
        
        // Convert GCD counts to prefix sums for range queries
        for (int i = 1; i <= m; i++) {
            gcd_cnt[i] += gcd_cnt[i - 1]; // Cumulative count of pairs with GCD <= i
        }
        
        vector<int> ans; // Initialize answer vector
        ans.reserve(queries.size()); // Pre-allocate memory for queries
        
        // Answer each query using binary search on the cumulative counts
        for (long long q : queries) {
            q++; // Convert 0-based index to 1-based pair count
            // Find the smallest GCD value whose prefix sum is >= q
            int pos = lower_bound(gcd_cnt.begin(), gcd_cnt.end(), q) - gcd_cnt.begin();
            ans.push_back(pos); // Store the GCD value
        }
        
        return ans; // Return the results
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach avoids computing the GCD of pairs explicitly. Instead, it uses
// a sieve-like technique to count pairs with a specific GCD.
// For any number i, we first count the numbers in `nums` that are multiples of i. Let this be C.
// The total pairs we can form using multiples of i is C * (C - 1) / 2. Any such pair has a
// common divisor of i. However, their GREATEST common divisor might be a strict multiple of i
// (e.g. 2i, 3i, etc.).
// To find the exact count of pairs with GCD equal to i, we iterate from M down to 1. For each i,
// we start with the total pairs among multiples of i, and subtract the exact counts of pairs
// with GCD equal to 2i, 3i, etc., which have already been computed.
// Finally, we compute prefix sums of these counts to answer queries via binary search.

// Algorithm:
// 1. Find the maximum element M in `nums`.
// 2. Count the frequency of each number in `nums`.
// 3. For each i from 1 to M, count how many elements in `nums` are multiples of i.
// 4. For each i, calculate total pairs from its multiples: C * (C - 1) / 2.
// 5. From i = M down to 1, subtract counts of pairs with GCD as strict multiples of i.
// 6. Compute prefix sums of the pair counts for GCD values.
// 7. For each query, use lower_bound on prefix sums to find the corresponding GCD value.

// Time Complexity:
// O(N + M * log(M) + Q * log(M)) where N is the size of nums, M is the maximum element in nums,
// and Q is the number of queries. The sieve loops run in harmonic series summation, taking O(M log M).

// Space Complexity:
// O(M) to store the count/frequency arrays of size M + 1.

namespace Optimal {
class Solution {
public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        // Find the maximum value in nums to define array bounds
        int m = *max_element(nums.begin(), nums.end());
        
        // Count array to track frequency of elements, multiples, and pair counts
        vector<long long> cnt(m + 1);
        for (int num : nums) {
            cnt[num]++; // Count initial frequency of each number
        }
        
        // For each number i, compute how many elements in nums are multiples of i
        for (int i = 1; i <= m; i++) {
            for (int j = i * 2; j <= m; j += i) {
                cnt[i] += cnt[j]; // Accumulate counts of multiples of i
            }
        }
        
        // Calculate the total pairs that can be chosen from multiples of i
        for (int i = 1; i <= m; i++) {
            cnt[i] = cnt[i] * (cnt[i] - 1) / 2; // Choose 2 elements among multiples of i
        }
        
        // Subtract counts of pairs whose GCD is a strict multiple of i to get exact GCD counts
        for (int i = m; i >= 1; i--) {
            for (int j = i * 2; j <= m; j += i) {
                cnt[i] -= cnt[j]; // Remove pairs with GCD equal to j (which is a multiple of i)
            }
        }
        
        // Convert exact GCD pair counts to prefix sums for query processing
        for (int i = 1; i <= m; i++) {
            cnt[i] += cnt[i - 1]; // Cumulative count of pairs with GCD <= i
        }
        
        vector<int> ans; // Initialize answer vector
        
        // Process each query to retrieve the GCD value at the sorted index
        for (long long q : queries) {
            q++; // Convert 0-based query index to 1-based count
            
            // Perform binary search to find the smallest GCD value with cumulative pairs >= q
            int pos = lower_bound(cnt.begin(), cnt.end(), q) - cnt.begin();
            ans.push_back(pos); // Append the resulting GCD value
        }
        
        return ans; // Return the final list of query answers
    }
};
}