class Solution {
public:
    vector<int> targetIndices(vector<int>& nums, int target) {
        sort(begin(nums),end(nums));
        vector<int>ans;
        int n=nums.size();
        for(int i=0;i<n;i++){
            if(nums[i]==target){
                ans.push_back(i);
            }
        }
        sort(begin(ans),end(ans));
        return ans;
    }
};