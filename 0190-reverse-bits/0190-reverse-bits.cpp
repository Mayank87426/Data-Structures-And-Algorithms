class Solution {
public:
    int reverseBits(int n) {
        int result = 0;

        for (int i = 31; i >= 0; i--) {
            if ((n & (1 << i)) != 0) {
                result = result | (1 << (31 - i));
            }
        }

        return result;
    }
};