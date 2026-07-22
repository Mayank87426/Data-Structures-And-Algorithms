class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int resMax = nums[0], resMin = nums[0];
        int totalSum = accumulate(begin(nums), end(nums), 0);
        int maxEnding = nums[0], minEnding = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            maxEnding = max(nums[i], maxEnding + nums[i]);
            minEnding = min(nums[i], minEnding + nums[i]);
            resMax = max(resMax, maxEnding);
            resMin = min(resMin, minEnding);
        }
        int maxRemainingSum = totalSum - resMin;
        if (maxRemainingSum == 0)
            maxRemainingSum = INT_MIN;
        return max(resMax, maxRemainingSum);
    }
};