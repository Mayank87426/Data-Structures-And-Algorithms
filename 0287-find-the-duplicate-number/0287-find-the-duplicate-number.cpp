class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int s = 0, f = 0;

        while (true) {
            s = nums[s];
            f = nums[f];
            f = nums[f];

            if (s == f)
                break;
        }

        s = 0;

        while (true) {
            s = nums[s];
            f = nums[f];

            if (s == f)
                return s;
        }
    }
};
