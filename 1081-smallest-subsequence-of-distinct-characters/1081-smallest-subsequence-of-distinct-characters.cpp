class Solution {
public:
    string smallestSubsequence(string s) {
        vector<bool> taken(26, false);
        vector<int> lastIdx(26);

        for (int i = 0; i < s.size(); i++) {
            lastIdx[s[i] - 'a'] = i;
        }

        string str = "";

        for (int i = 0; i < s.size(); i++) {
            char c = s[i];

            if (taken[c - 'a'])
                continue;

            while (!str.empty() &&
                   str.back() > c &&
                   lastIdx[str.back() - 'a'] > i) {
                taken[str.back() - 'a'] = false;
                str.pop_back();
            }

            str += c;
            taken[c - 'a'] = true;
        }

        return str;
    }
};