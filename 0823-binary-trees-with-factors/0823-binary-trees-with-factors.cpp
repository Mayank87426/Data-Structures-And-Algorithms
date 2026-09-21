class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        int n = arr.size();
        int MOD = 1e9 + 7;
        sort(arr.begin(), arr.end());
        
        unordered_map<int, int> mp;
        for (int i = 0; i < n; i++) {
            mp[arr[i]] = i;
        }
        
        vector<long long> dp(n, 1);
        long long totalTrees = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (arr[i] % arr[j] == 0) {
                    int right = arr[i] / arr[j];
                    if (mp.count(right)) {
                        dp[i] = (dp[i] + dp[j] * dp[mp[right]]) % MOD;
                    }
                }
            }
            totalTrees = (totalTrees + dp[i]) % MOD;
        }
        
        return totalTrees;
    }
};