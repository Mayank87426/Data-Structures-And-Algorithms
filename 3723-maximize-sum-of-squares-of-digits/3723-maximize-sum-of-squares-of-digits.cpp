class Solution {
public:
    string maxSumOfSquares(int num, int sum) {
        string ans = "";
        for (int i = 9; i >= 0; i-- && ans.length() < num) {
            while (sum >= i && ans.length() < num) {
                ans.push_back(i + '0');
                sum -= i;
            }
        }
        return ans.length() == num && sum == 0 ? ans : "";
    }
};