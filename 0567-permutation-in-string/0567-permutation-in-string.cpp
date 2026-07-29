class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s2.length();
        int m = s1.length();
        sort(begin(s1), end(s1));
        int l = 0, r = 0;
        while (r < n) {
            while (l <= r && r - l + 1 > m) {
                l++;
            }
            if (r - l + 1 == m) {
                string str = s2.substr(l, r - l + 1);
                sort(begin(str), end(str));
                if (str == s1)
                    return true;
            }
            r++;
        }
        return false;
    }
};