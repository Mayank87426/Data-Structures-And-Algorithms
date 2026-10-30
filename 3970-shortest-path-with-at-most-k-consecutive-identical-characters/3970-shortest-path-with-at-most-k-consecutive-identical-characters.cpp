class Solution {
public:
    int shortestPath(int n, vector<vector<int>>& edges, string labels, int k) {
        vector<vector<pair<int,int>>>adj(n);
        for(auto&edge:edges){
            int u=edge[0],v=edge[1],w=edge[2];
            adj[u].push_back({v,w});
        }
        priority_queue<tuple<int,int,int>,vector<tuple<int,int,int>>,greater<>>pq;
        vector<vector<int>>dist(n,vector<int>(k+1,INT_MAX));
        pq.push({0,0,1});
        dist[0][1]=0;
        while(!pq.empty()){
            auto [d,u,cur_k]=pq.top();pq.pop();
            if(d>dist[u][cur_k])
                continue;
            if(u==n-1)
                return d;
            for(auto [v,w]:adj[u]){
                int new_k=cur_k;
                if(labels[u]==labels[v])
                    ++new_k;
                else
                    new_k=1;
                if(new_k>k)
                    continue;
                if(dist[v][new_k]>d+w){
                    dist[v][new_k]=d+w;
                    pq.push({dist[v][new_k],v,new_k});
                }
            }
        }
        return -1;
    }
};