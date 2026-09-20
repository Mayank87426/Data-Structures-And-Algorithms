class Solution {
public:
    int t[100001][3][2];
    int solve(vector<int>& prices, int i, int count, bool bought) {
        if (i >= prices.size()||count>=2)
            return 0;
        if (t[i][count][bought] != -1)
            return t[i][count][bought];
        int buy = INT_MIN, sell = INT_MIN;
        if (!bought && count < 2) {
            buy = max(-prices[i] + solve(prices, i + 1, count, true),
                      solve(prices, i + 1, count, false));
        }
        if (bought && count < 2) {
            sell = max(prices[i] + solve(prices, i + 1, count + 1, false),
                       solve(prices, i + 1, count, true));
        }
        return t[i][count][bought] = max(buy, sell);
    }
    int maxProfit(vector<int>& prices) {
        memset(t, -1, sizeof(t));
        return solve(prices, 0, 0, false);
    }
};