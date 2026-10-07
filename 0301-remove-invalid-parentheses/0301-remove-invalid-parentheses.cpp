class Solution {
public:
    int maxLen = 0;

    void solve(string& s, int i, int open, int close, string& curr,
               vector<string>& ans) {
        if (close > open)
            return;

        if (i == s.length()) {
            if (open == close) {
                if (curr.length() > maxLen) {
                    maxLen = curr.length();
                    ans.clear();
                    ans.push_back(curr);
                }
                else if (curr.length() == maxLen) {
                    ans.push_back(curr);
                }
            }
            return;
        }

        curr.push_back(s[i]);

        if (s[i] == '(') {
            solve(s, i + 1, open + 1, close, curr, ans);
        }
        else if (s[i] == ')') {
            solve(s, i + 1, open, close + 1, curr, ans);
        }
        else {
            solve(s, i + 1, open, close, curr, ans);
        }

        curr.pop_back();

        if (s[i] == '(' || s[i] == ')') {
            solve(s, i + 1, open, close, curr, ans);
        }
    }

    vector<string> removeInvalidParentheses(string s) {
        vector<string> ans;
        string curr;

        solve(s, 0, 0, 0, curr, ans);

        sort(ans.begin(), ans.end());
        ans.erase(unique(ans.begin(), ans.end()), ans.end());

        return ans;
    }
};
