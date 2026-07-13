class Solution {
public:
    unordered_map<string, int> mp;
    int solve(vector<int>& nums, int i, int target) {
        int n = nums.size();
        if (i >= n)
            return target == 0;
            
        string key = to_string(i) + "_" + to_string(target);
        if (mp.count(key))
            return mp[key];
        int takeAsPositive = solve(nums, i + 1, target - nums[i]);
        int takeAsNeg = solve(nums, i + 1, target + nums[i]);
        return mp[key] = takeAsPositive + takeAsNeg;
    }
    int findTargetSumWays(vector<int>& nums, int target) {
        return solve(nums, 0, target);
    }
};