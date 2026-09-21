class Solution {
public:
    int t[20002];
    int solve(vector<int>& nums, int i, unordered_map<int, int>& mp) {
        if (i >= nums.size())
            return 0;
        if (t[i] != -1)
            return t[i];

        int not_pick = solve(nums, i + 1, mp);
        int pick = mp[nums[i]] * nums[i];
        if (i + 1 < nums.size() && nums[i + 1] != nums[i] + 1) {
            pick += solve(nums, i + 1, mp);
        } else
            pick += solve(nums, i + 2, mp);
        return t[i] = max(pick, not_pick);
    }
    int deleteAndEarn(vector<int>& nums) {
        unordered_map<int, int> mp;
        for (int x : nums)
            mp[x]++;
        vector<int> v;
        for (auto [a, b] : mp)
            v.push_back(a);
        memset(t, -1, sizeof(t));
        sort(begin(v), end(v));
        return solve(v, 0, mp);
    }
};