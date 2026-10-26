class Solution {
    void dfs(int i, int j, int m, int n, vector<vector<int>> &grid, long long &sum){
        if(i<0 || i>=m || j<0 || j>=n || grid[i][j]==0)return;
        sum+=grid[i][j];
        grid[i][j]=0;
        int nr[]={1,0,-1,0};
        int nc[]={0,1,0,-1};
        for(int t=0;t<4;t++){
            int r=i+nr[t];
            int c=j+nc[t];
            if(r>=0 && r<m && c>=0 && c<n && grid[r][c]!=0){
                dfs(r,c,m,n,grid,sum);
            }
        }
    }
public:
    int countIslands(vector<vector<int>>& grid, int k) {
        int m=grid.size();
        int n=grid[0].size();
        int ans=0;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(grid[i][j]!=0){
                    long long sum=0;
                    dfs(i,j,m,n,grid,sum);
                    if(sum%k==0){
                        ans++;
                    }
                }
            }
        }
        return ans;
    }
};