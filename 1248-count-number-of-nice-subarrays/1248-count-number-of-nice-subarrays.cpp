class Solution {
public:
    int atMostKOddSubarrays(vector<int>& nums, int k) {
        int n = nums.size();
        int l = 0, r = 0;
        int oddCount = 0;
        int count = 0;
        while (r < n) {
            if (nums[r] % 2)
                oddCount++;
            while (l <= r && oddCount > k) {
                if (nums[l] % 2)
                    oddCount--;
                l++;
            }
            count += (r - l + 1);
            r++;
        }
        return count;
    }
    int numberOfSubarrays(vector<int>& nums, int k) {
        return atMostKOddSubarrays(nums, k) - atMostKOddSubarrays(nums, k - 1);
    }
};