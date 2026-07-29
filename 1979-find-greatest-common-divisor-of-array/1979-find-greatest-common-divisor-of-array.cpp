// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach involves sorting the entire array to locate the minimum and maximum elements 
// at the boundaries of the sorted array (indices 0 and size-1, respectively). Once we identify these 
// two elements, we perform a linear scan from 1 up to the minimum element to check every possible integer. 
// The largest integer that divides both the minimum and maximum elements is our greatest common divisor (GCD).

// Algorithm:
// 1. Sort the input array `nums` in ascending order.
// 2. Extract the minimum element `mn` as `nums[0]` and the maximum element `mx` as `nums[nums.size() - 1]`.
// 3. Initialize a variable `result` to 1.
// 4. Iterate from 1 up to `mn` (inclusive).
// 5. In each iteration, check if the current integer divides both `mn` and `mx` without a remainder. If so, update `result`.
// 6. Return `result`.

// Time Complexity:
// O(N log N + min(mn, mx)) where N is the number of elements in the array. Sorting takes O(N log N) time, 
// and the loop runs up to min(mn, mx) times.

// Space Complexity:
// O(1) or O(log N) depending on the sorting implementation's auxiliary memory requirements.

namespace BruteForce {
class Solution {
public:
    int findGCD(vector<int>& nums) {
        // Sort the array in ascending order to easily access the minimum and maximum elements.
        sort(nums.begin(), nums.end());
        
        // The first element is the minimum since the array is sorted.
        int mn = nums[0];
        
        // The last element is the maximum since the array is sorted.
        int mx = nums[nums.size() - 1];
        
        // Initialize the GCD to 1, which is the baseline common divisor for any positive integers.
        int result = 1;
        
        // Iterate from 1 up to the minimum element to test potential common divisors.
        for (int i = 1; i <= mn; ++i) {
            // Check if the current integer divides both the minimum and maximum elements without remainder.
            if (mn % i == 0 && mx % i == 0) {
                // Update result since we are searching for the largest common divisor.
                result = i;
            }
        }
        
        // Return the largest common divisor found during the linear scan.
        return result;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The better approach optimizes both finding the min/max and calculating the GCD. Instead of sorting 
// (which takes O(N log N) time), we find the minimum and maximum elements in a single linear scan of 
// O(N) time. To find the GCD, instead of iterating upwards from 1, we scan downwards starting from the 
// minimum element `mn` down to 1. The first value that divides both `mn` and `mx` is guaranteed to be 
// the greatest common divisor, allowing us to terminate the search early.

// Algorithm:
// 1. Initialize `mn` and `mx` with the first element of the array.
// 2. Loop through the rest of the array to find the true minimum and maximum elements in a single pass.
// 3. Loop downwards from `mn` to 1.
// 4. For each integer, check if it divides both `mn` and `mx` with no remainder.
// 5. Return the first such divisor found.

// Time Complexity:
// O(N + min(mn, mx)) where N is the size of the array. We spend O(N) to find the range boundaries 
// and at most min(mn, mx) iterations to find the GCD.

// Space Complexity:
// O(1) auxiliary space as we only use a few tracking variables.

namespace Better {
class Solution {
public:
    int findGCD(vector<int>& nums) {
        // Initialize minimum and maximum values with the first element of the array.
        int mn = nums[0];
        int mx = nums[0];
        
        // Traverse the array to locate the minimum and maximum elements in a single pass.
        for (int i = 1; i < nums.size(); ++i) {
            // If the current element is smaller than our current minimum, update the minimum.
            if (nums[i] < mn) {
                mn = nums[i];
            }
            // If the current element is larger than our current maximum, update the maximum.
            if (nums[i] > mx) {
                mx = nums[i];
            }
        }
        
        // Search downwards starting from the minimum element to find the greatest common divisor efficiently.
        for (int i = mn; i >= 1; --i) {
            // Check if the current value divides both the minimum and maximum elements with no remainder.
            if (mn % i == 0 && mx % i == 0) {
                // Return immediately because the first common divisor found scanning downwards is the greatest.
                return i;
            }
        }
        
        // Fallback return, although 1 is guaranteed to be a common divisor.
        return 1;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach finds the minimum and maximum elements in O(N) time and then computes their GCD 
// using the modulo-based Euclidean algorithm. The Euclidean algorithm operates on the principle that the 
// GCD of two numbers also divides their difference, leading to a logarithmic complexity of O(log(min(mn, mx))) 
// rather than linear.

// Algorithm:
// 1. Find the maximum element `mx` in the input array using `std::max_element`.
// 2. Find the minimum element `mn` in the input array using `std::min_element`.
// 3. Compute and return the GCD of `mx` and `mn` using the built-in `std::gcd` function, which implements the Euclidean algorithm.

// Time Complexity:
// O(N + log(min(mn, mx))) where N is the length of `nums`. Finding min and max elements takes O(N) time, 
// and the Euclidean algorithm takes logarithmic time.

// Space Complexity:
// O(1) auxiliary space as the calculations require no additional memory structures.

namespace Optimal {
class Solution {
public:
    int findGCD(vector<int>& nums) {
        // Find the maximum value in the input array to determine one of the two numbers for the GCD calculation.
        int mx = *max_element(nums.begin(), nums.end());
        
        // Find the minimum value in the input array to determine the second number for the GCD calculation.
        int mn = *min_element(nums.begin(), nums.end());
        
        // Compute and return the GCD of the maximum and minimum elements using the efficient built-in Euclidean algorithm.
        return gcd(mx, mn);
    }
};
}