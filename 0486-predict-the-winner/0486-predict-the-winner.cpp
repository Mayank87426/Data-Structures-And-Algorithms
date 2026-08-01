class Solution {
public:
    bool solve(vector<int>& nums, int i, int j, bool turn, int p1, int p2) {
        if (i > j)
            return p1 >= p2;
        bool t1 = false;
        if (turn) {
            t1 = solve(nums, i + 1, j, false, p1 + nums[i], p2) ||
                 solve(nums, i, j - 1, false, p1 + nums[j], p2);
        } else {
            t1 = solve(nums, i + 1, j, true, p1, p2 + nums[i]) &&
                 solve(nums, i, j - 1, true, p1, p2 + nums[j]);
        }
        return t1;
    }
    bool predictTheWinner(vector<int>& nums) {
        return solve(nums, 0, nums.size() - 1, true, 0, 0);
    }
};