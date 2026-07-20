/*
approach is simple sliding window and couting prefix 0 becaue sum+0=sum
hence each 0 can create a seperate new subarray
count+=1+prefixZeros 1 for sum=goal and prefixZeros 0 create separate subarrays
*/
class Solution {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        int prefixZeros=0;
        int n = nums.size(), sum = 0, count = 0, i = 0;
        for (int j = 0; j < n; j++) {
            sum += nums[j];
            while ((sum > goal||nums[i]==0) && i < j) {
                sum -= nums[i];
                if(nums[i]==0)prefixZeros++;
                else prefixZeros=0;
                i++;
            }
            if (sum == goal)
                count+=1+prefixZeros;
        }
        return count;
    }
};