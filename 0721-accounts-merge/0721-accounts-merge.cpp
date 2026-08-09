class DisjointSet {
    vector<int> rank, parent, size;
public:
    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1);
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int findUPar(int node) {
        if (node == parent[node])
            return node;
        return parent[node] = findUPar(parent[node]);
    }

    void unionByRank(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v;
        }
        else if (rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u;
        }
        else {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }

    void unionBySize(int u, int v) {
        int ulp_u = findUPar(u);
        int ulp_v = findUPar(v);
        if (ulp_u == ulp_v) return;
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        }
        else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};


class Solution {
public:

    vector<vector<string>> accountsMerge(vector<vector<string>>& details) {

        // Number of accounts
        int n = details.size();

        // Create DSU for all accounts.
        // Each account is treated as a separate node initially.
        DisjointSet ds(n);

        // Sort the accounts.
        // This helps us later when sorting the final answer.
        sort(details.begin(), details.end());

        // Stores the account index where an email was first encountered.
        //
        // Example:
        // "a@gmail.com" -> 0
        // "b@gmail.com" -> 1
        unordered_map<string, int> mapMailNode;


        // Traverse all accounts
        for (int i = 0; i < n; i++) {

            // Start from 1 because index 0 contains the person's name.
            for (int j = 1; j < details[i].size(); j++) {

                // Get the current email
                string mail = details[i][j];

                // If this email has not been seen before
                if (mapMailNode.find(mail) == mapMailNode.end()) {

                    // Store the current account as the first account
                    // where this email was found.
                    mapMailNode[mail] = i;
                }

                else {

                    // This email already belongs to another account.
                    // Therefore, both accounts belong to the same person.
                    //
                    // Merge their DSU components.
                    ds.unionBySize(i, mapMailNode[mail]);
                }
            }
        }


        // mergedMail[i] contains all emails belonging to
        // the DSU component whose representative is i.
        vector<vector<string>> mergedMail(n);


        // Traverse all emails that we encountered.
        for (auto it : mapMailNode) {

            // Get the email
            string mail = it.first;

            // Get the account where this email was first found
            int node = it.second;

            // Find the ultimate parent of that account.
            // This tells us which merged account the email belongs to.
            int parent = ds.findUPar(node);

            // Add the email to its corresponding component.
            mergedMail[parent].push_back(mail);
        }


        // Stores the final merged accounts.
        vector<vector<string>> ans;


        // Traverse all possible DSU representatives.
        for (int i = 0; i < n; i++) {

            // If this component has no emails,
            // there is nothing to add to the answer.
            if (mergedMail[i].size() == 0)
                continue;

            // Emails must be sorted alphabetically.
            sort(mergedMail[i].begin(), mergedMail[i].end());

            // Temporary vector for one merged account.
            vector<string> temp;

            // Add the account owner's name.
            temp.push_back(details[i][0]);

            // Add all emails belonging to this account.
            for (auto mail : mergedMail[i]) {
                temp.push_back(mail);
            }

            // Add this merged account to the final answer.
            ans.push_back(temp);
        }


        // Sort the final accounts lexicographically.
        sort(ans.begin(), ans.end());

        // Return the final merged accounts.
        return ans;
    }
};
