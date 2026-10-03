class Solution {
public:
    int longestValidParentheses(string s) {
        int n = s.length();
        int open = 0, close = 0;
        int maxLen = 0;
        for (auto c : s) {
            if (c == '(')
                open++;
            else
                close++;
            if (close > open) {
                open = 0, close = 0;
            }
            if (open == close)
                maxLen = max(maxLen, open + close);
        }

        open = 0, close = 0;
        for (int i = n - 1; i >= 0; i--) {
            char c = s[i];
            if (c == '(')
                open++;
            else
                close++;
            if (open > close) {
                open = 0, close = 0;
            }
            if (open == close)
                maxLen = max(maxLen, open + close);
        }
        return maxLen;
    }
};