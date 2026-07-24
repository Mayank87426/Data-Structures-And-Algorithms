class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size();
        int count = 0;
        int i = 0, j = 0;
        int diff = INT_MAX;

        while (j < n) {

            while (j < n &&
                   (diff == INT_MAX || (nums[j] - nums[j - 1]) == diff)) {

                if (i != j)
                    diff = nums[j] - nums[j - 1];

                count += max(0, j - i - 1);
                j++;
            }

            diff = INT_MAX;

            if (j == n) break;

            i = j - 1;
            j = i;
        }

        return count;
    }
};