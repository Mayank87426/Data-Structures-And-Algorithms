class Solution {
public:
    int longestNiceSubarray(vector<int>& nums) {
        int n = nums.size();
        int maxLen = 0;
        int i = 0, j = 0;
        int mask = 0;

        while (j < n) {
            while (i <= j && (mask & nums[j]) != 0) {
                mask ^= nums[i];
                i++;
            }

            mask |= nums[j];
            maxLen = max(maxLen, j - i + 1);

            j++;
        }

        return maxLen;
    }
};
