class Solution {
public:
    int numDecodings(string s) {

        int n = s.length();
        vector<int> dp(n + 1, 0);
        dp[n] = 1;
        for (int i = n - 1; i >= 0; i--) {
            if (s[i] == '0')
                continue;
            int oneChar = +dp[i + 1];
            dp[i] += oneChar;
            if (i < n - 1) {
                string str = s.substr(i, 2);
                int v = stoi(str);
                if (v >= 10 && v <= 26) {
                    dp[i] += dp[i + 2];
                }
            }
        }
        return dp[0];
    }
};