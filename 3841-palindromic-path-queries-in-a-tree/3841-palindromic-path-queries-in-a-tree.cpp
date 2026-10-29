class Solution {
public:
    int n;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos;
    vector<int> seg;
    vector<int> value;
    int cur_pos;

    int querySeg(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return seg[node];
        int mid = (start + end) / 2;
        return querySeg(2*node, start, mid, l, r)
             ^ querySeg(2*node+1, mid+1, end, l, r);
    }

    void updateSeg(int node, int start, int end, int idx, int val) {
        if (start == end) {
            seg[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid)
            updateSeg(2*node, start, mid, idx, val);
        else
            updateSeg(2*node+1, mid+1, end, idx, val);
        seg[node] = seg[2*node] ^ seg[2*node+1];
    }

    int dfs(int v, int p) {
        int size = 1, max_sub = 0;
        for (int u : adj[v]) {
            if (u != p) {
                parent[u] = v;
                depth[u] = depth[v] + 1;
                int sub = dfs(u, v);
                if (sub > max_sub) {
                    max_sub = sub;
                    heavy[v] = u;
                }
                size += sub;
            }
        }
        return size;
    }

    void decompose(int v, int h) {
        head[v] = h;
        pos[v] = cur_pos++;
        updateSeg(1, 0, n-1, pos[v], value[v]);

        if (heavy[v] != -1)
            decompose(heavy[v], h);

        for (int u : adj[v]) {
            if (u != parent[v] && u != heavy[v])
                decompose(u, u);
        }
    }

    int queryPath(int a, int b) {
        int res = 0;
        while (head[a] != head[b]) {
            if (depth[head[a]] < depth[head[b]])
                swap(a, b);
            res ^= querySeg(1, 0, n-1, pos[head[a]], pos[a]);
            a = parent[head[a]];
        }
        if (depth[a] > depth[b])
            swap(a, b);
        res ^= querySeg(1, 0, n-1, pos[a], pos[b]);
        return res;
    }

    vector<bool> palindromePath(int n_,
                                vector<vector<int>>& edges,
                                string s,
                                vector<string>& queries) {
        n = n_;
        adj.assign(n, {});
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, 0);
        pos.assign(n, 0);
        value.assign(n, 0);
        seg.assign(4*n, 0);

        for (auto &e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        for (int i = 0; i < n; i++)
            value[i] = 1 << (s[i] - 'a');

        dfs(0, -1);
        cur_pos = 0;
        decompose(0, 0);

        vector<bool> ans;

        for (auto &q : queries) {
            string type, a, b;
            stringstream ss(q);
            ss >> type >> a >> b;

            if (type == "query") {
                int u = stoi(a);
                int v = stoi(b);
                int mask = queryPath(u, v);
                ans.push_back(__builtin_popcount(mask) <= 1);
            } else {
                int idx = stoi(a);
                s[idx] = b[0];
                int newMask = 1 << (s[idx] - 'a');
                updateSeg(1, 0, n-1, pos[idx], newMask);
            }
        }

        return ans;
    }
};