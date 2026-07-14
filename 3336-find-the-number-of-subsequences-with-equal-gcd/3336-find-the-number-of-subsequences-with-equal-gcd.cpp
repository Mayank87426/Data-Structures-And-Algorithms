class Solution {
public:
    static constexpr int MOD = 1'000'000'007;
    int t[201][201][201];

    int dfs(vector<int>& nums, int i, int gcd1, int gcd2) {
        if (i == nums.size()) {
            return (gcd1 != 0 && gcd2 != 0 && gcd1 == gcd2);
        }

        if (t[i][gcd1][gcd2] != -1)
            return t[i][gcd1][gcd2];

        int x = dfs(nums, i + 1, gcd1, gcd2);

        int y = dfs(nums, i + 1, __gcd(gcd1, nums[i]), gcd2);

        int z = dfs(nums, i + 1, gcd1, __gcd(gcd2, nums[i]));

        int ans = x;
        ans = (1LL * ans + y) % MOD;
        ans = (1LL * ans + z) % MOD;
        return t[i][gcd1][gcd2] = ans;
    }

    int subsequencePairCount(vector<int>& nums) {
        memset(t, -1, sizeof(t));
        return dfs(nums, 0, 0, 0);
    }
};