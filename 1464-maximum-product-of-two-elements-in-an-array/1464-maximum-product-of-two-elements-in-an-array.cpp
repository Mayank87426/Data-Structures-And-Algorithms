// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach involves iterating through all possible pairs of indices (i, j)
// where i != j, and calculating the product (nums[i] - 1) * (nums[j] - 1) for each pair.
// We keep track of the maximum product encountered and return it.

// Algorithm:
// 1. Initialize a variable `max_prod` to 0 to store the maximum product found.
// 2. Use a nested loop where the outer loop index `i` runs from 0 to n-1, and the inner loop index `j` runs from i+1 to n-1.
// 3. For each pair, calculate `(nums[i] - 1) * (nums[j] - 1)`.
// 4. Update `max_prod` with the maximum of its current value and the calculated product.
// 5. Return `max_prod`.

// Time Complexity:
// O(N^2) where N is the number of elements in the array, due to checking all unique pairs.

// Space Complexity:
// O(1) as we only use a few helper variables for the maximum product and loop indices.

namespace BruteForce {
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        // Initialize the maximum product variable to track the highest value found
        int max_prod = 0;
        // Get the total number of elements in the input vector
        int n = nums.size();
        
        // Loop through each element in the array as the first candidate
        for (int i = 0; i < n; ++i) {
            // Loop through the remaining elements to form a pair with nums[i]
            for (int j = i + 1; j < n; ++j) {
                // Calculate the product of the elements after subtracting 1 from each
                int prod = (nums[i] - 1) * (nums[j] - 1);
                // Update the maximum product if the current pair's product is larger
                if (prod > max_prod) {
                    max_prod = prod;
                }
            }
        }
        // Return the overall maximum product found
        return max_prod;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The better approach is to sort the array in non-decreasing order. Once sorted, the two
// largest elements in the array will be at the last two positions: nums[n-1] and nums[n-2].
// The maximum product of two elements minus one will then be (nums[n-1] - 1) * (nums[n-2] - 1).

// Algorithm:
// 1. Sort the input array `nums` in non-decreasing order using `std::sort`.
// 2. Get the size of the array `n`.
// 3. Retrieve the two largest elements at indices `n-1` and `n-2`.
// 4. Calculate and return `(nums[n-1] - 1) * (nums[n-2] - 1)`.

// Time Complexity:
// O(N log N) where N is the number of elements in the array, because of the sorting step.

// Space Complexity:
// O(1) or O(log N) depending on the implementation of std::sort.

namespace Better {
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        // Sort the vector in ascending order to bring the largest elements to the end
        sort(begin(nums),end(nums));
        // Retrieve the size of the vector to find the last two indices
        int n=nums.size();
        // Compute and return the product of the two largest elements minus one
        return (nums[n-1]-1)*(nums[n-2]-1);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach avoids sorting by finding the two largest elements, `max1` and `max2`,
// in a single linear scan of the array. `max1` will store the largest element, and `max2`
// will store the second largest element. We initialize both to 0.

// Algorithm:
// 1. Initialize `max1` and `max2` to 0.
// 2. Iterate through each element `num` in the array:
//    - If `num` is greater than `max1`, then `max2` becomes `max1` and `max1` becomes `num`.
//    - Else if `num` is greater than `max2` (but less than or equal to `max1`), update `max2` to `num`.
// 3. After the loop, return `(max1 - 1) * (max2 - 1)`.

// Time Complexity:
// O(N) where N is the number of elements in the array, as we scan the array exactly once.

// Space Complexity:
// O(1) as we only use two scalar variables to keep track of the two largest values.

namespace Optimal {
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        // Track the largest element seen so far
        int max1 = 0;
        // Track the second largest element seen so far
        int max2 = 0;
        
        // Iterate through each element in the array to find the two largest elements
        for (int num : nums) {
            // If the current element is larger than the largest element found so far
            if (num > max1) {
                // The former largest becomes the second largest
                max2 = max1;
                // Update the largest element
                max1 = num;
            } 
            // If the current element is not larger than max1 but is larger than max2
            else if (num > max2) {
                // Update the second largest element
                max2 = num;
            }
        }
        // Calculate and return the maximum product after subtracting 1 from each of the two largest numbers
        return (max1 - 1) * (max2 - 1);
    }
};
}