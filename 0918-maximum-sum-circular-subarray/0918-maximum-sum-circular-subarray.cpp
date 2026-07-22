class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {

        // Intuition:
        // There are only 2 possible answers:
        //
        // 1. Maximum subarray does NOT wrap around.
        //    -> Normal Kadane's Algorithm.
        //
        // 2. Maximum subarray wraps around.
        //    -> Instead of finding what we take,
        //       find what we REMOVE.
        //       Wrapped Sum = Total Sum - Minimum Subarray Sum.
        //
        // Final Answer = max(Normal Kadane, Wrapped Sum)
        //
        // Edge Case:
        // If all elements are negative,
        // Minimum Subarray = Entire Array
        // => Wrapped Sum = Total - Total = 0
        // 0 means we removed the whole array (empty subarray),
        // which is NOT allowed.
        // So ignore wrapped answer in this case.

        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        // Kadane for maximum subarray
        int maxEnding = nums[0];
        int resMax = nums[0];

        // Kadane for minimum subarray
        int minEnding = nums[0];
        int resMin = nums[0];

        for (int i = 1; i < nums.size(); i++) {

            // Either extend previous subarray
            // or start a new one.
            maxEnding = max(nums[i], maxEnding + nums[i]);
            resMax = max(resMax, maxEnding);

            // Same idea for minimum subarray.
            minEnding = min(nums[i], minEnding + nums[i]);
            resMin = min(resMin, minEnding);
        }

        // Maximum sum if subarray wraps around.
        int maxRemainingSum = totalSum - resMin;

        // If wrapped sum becomes 0,
        // it means minimum subarray was the entire array.
        // That would leave an empty subarray, which is invalid.
        if (maxRemainingSum == 0)
            maxRemainingSum = INT_MIN;

        // Best of wrapped and non-wrapped cases.
        return max(resMax, maxRemainingSum);
    }
};