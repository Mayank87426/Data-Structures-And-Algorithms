class Solution {
public:
    bool check(int n) {
        string s = to_string(n);
        for (int i = 1; i < s.length(); i++) {
            if (s[i] != s[i - 1] + 1)
                return false;
        }
        return true;
    }
    int nextSequential(int low) {
        int len = to_string(low).size();

        while (len <= 9) {
            for (int start = 1; start + len - 1 <= 9; start++) {
                int num = 0;
                for (int d = start; d < start + len; d++)
                    num = num * 10 + d;

                if (num >= low)
                    return num;
            }
            len++;
        }
        return INT_MAX;
    }
    vector<int> sequentialDigits(int low, int high) {
        int digits = to_string(low).length();

        string str = "", d = "";
        int num = 1, k = digits;

        while (k--) {
            str.push_back(num + '0');
            d.push_back('1');
            num++;
        }

        low = nextSequential(low);

        int n = low;
        int j = stoi(d);

        vector<int> sequenceDigits;

        while (n <= high) {

            while (!check(n) && n <= high) {
                n += j;

                if (to_string(n).length() > str.length()) {
                    str.push_back(num + '0');
                    n = stoi(str);
                    num++;
                    j = j * 10 + 1;
                }
            }

            if (n > high)
                break;

            if (sequenceDigits.empty() || sequenceDigits.back() != n)
                sequenceDigits.push_back(n);

            if (to_string(n).length() > str.length()) {
                str.push_back(num + '0');
                n = stoi(str);
                num++;
                j = j * 10 + 1;
            } else {
                n += j;
            }
        }

        return sequenceDigits;
    }
};