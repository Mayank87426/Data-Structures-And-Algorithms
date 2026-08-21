class Solution {
public:
    int solve(string& s1, string& s2, int i, int j) {
        if (i < 0)
            return j + 1;
        if (j < 0)
            return i + 1;
        if (s1[i] == s2[j]) {
            return solve(s1, s2, i - 1, j - 1);
        }
        return 1 + min({solve(s1, s2, i - 1, j - 1), solve(s1, s2, i, j - 1),
                        solve(s1, s2, i - 1, j)});
    }
    int minDistance(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        int t[n + 1][m + 1];
        memset(t, INT_MAX, sizeof(t));
        for (int i = 0; i <= n; i++) {
            t[i][0] = i ;
        }
        for (int j = 0; j <= m; j++) {
            t[0][j] = j ;
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    t[i][j] = t[i - 1][j - 1];
                } else
                    t[i][j] =
                        1 + min({t[i - 1][j], t[i][j - 1], t[i - 1][j - 1]});
            }
        }
        return t[n][m];
    }
};