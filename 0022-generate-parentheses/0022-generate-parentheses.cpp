class Solution {
public:
    bool isValid(string& s) {
        stack<char> st;

        for (char c : s) {
            if (c == '(' || c == '{' || c == '[') {
                st.push(c);
            } else {
                if (st.empty())
                    return false;

                char top = st.top();
                st.pop();

                if ((c == ')' && top != '(') || (c == '}' && top != '{') ||
                    (c == ']' && top != '[')) {
                    return false;
                }
            }
        }

        return st.empty();
    }

    void solve(int n, vector<string>& ans, string s) {
        if (n == 0) {
            if (isValid(s))
                ans.push_back(s);
            return;
        }

        s.push_back('(');
        solve(n - 1, ans, s);
        s.pop_back();

        s.push_back(')');
        solve(n - 1, ans, s);
    }

    vector<string> generateParenthesis(int n) {
        vector<string> ans;
        solve(2 * n, ans, "");
        return ans;
    }
};
