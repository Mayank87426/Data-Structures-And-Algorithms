class Solution {
public:
    long long gcdSum(vector<int>& nums) {
        int n=nums.size();
        int mx=INT_MIN;
        vector<int>prefixGcd(n);
        for(int i=0;i<n;i++){
           mx=max(mx,nums[i]);
            prefixGcd[i]=__gcd(mx,nums[i]);
        }
        sort(begin(prefixGcd),end(prefixGcd));
        int l=0,r=n-1;
        long long s=0;
        while(l<r){
            int a=prefixGcd[l],b=prefixGcd[r];
            s+=__gcd(a,b);
            l++;
            r--;
        }
        return s;
    }
};