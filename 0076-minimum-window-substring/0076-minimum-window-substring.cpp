class Solution {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> mp;
        for (auto x : t)
            mp[x]++;
        int count = t.length(), n = s.length();
        int maxLen = INT_MAX, startIdx = 0;
        int i = 0, j = 0;
        while (j < n) {
            if (mp[s[j]] > 0) {
                count--;
            }
            mp[s[j]]--;
            while (count == 0 && i <= j) {
                if (maxLen > j - i + 1) {
                    maxLen = j - i + 1;
                    startIdx = i;
                }
                mp[s[i]]++;
                if (mp[s[i]] > 0) {
                    count++;
                }
                i++;
            }
            j++;
        }

        return maxLen == INT_MAX ? "": s.substr(startIdx, maxLen);
    }
};