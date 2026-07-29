// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach constructs an adjacency list representation of the equality relationships
// from the equations. For each equation, it performs a Depth First Search (DFS) to check the reachability
// between the two variables. If a '!=' relationship exists between two variables that are reachable from
// each other, or if a '==' relationship exists between two variables that are not reachable, it returns false.

// Algorithm:
// 1. Build an adjacency list `adj` where we add bidirectional edges between variables `u` and `v` for all equations with '=='.
// 2. Iterate through all equations.
// 3. For each equation, initialize a visited array `vis` of size 26 to keep track of traversed variables.
// 4. If the equation is '!=' and there is a path between the two variables, return false.
// 5. If the equation is '==' and there is no path between the two variables, return false.
// 6. If no contradictions are found after checking all equations, return true.

// Time Complexity:
// O(N * (V + E)), where N is the number of equations, V is the number of variables (26), and E is the number of equality edges (up to N). Since V is small (26), this is bounded by O(N * (26 + N)).

// Space Complexity:
// O(V + E) to store the graph and the recursion stack, where V is 26 and E <= N.

namespace BruteForce {
class Solution {
public:
    // Helper function to check reachability between nodes u and v via Depth First Search (DFS)
    bool isAbleToReach(unordered_map<int, vector<int>>& adj, int u, int v,
                       vector<bool>& vis) {
        // Base case: if source and target variables are the same, they are trivially reachable
        if (u == v)
            return true;
        // Mark the current variable as visited to prevent cycle-induced infinite recursion
        vis[u] = true;
        // Iterate through all variables connected to the current variable by equality equations
        for (auto nbr : adj[u]) {
            // Print the neighbor to preserve the exact debug log output from the original code
            cout << nbr;
            // Only traverse the neighbor if it has not been visited yet in the current search path
            if (!vis[nbr]) {
                // If a valid path to target v exists through neighbor, propagate reachability upwards
                if (isAbleToReach(adj, nbr, v, vis))
                    return true;
            }
        }
        // Return false if all connected paths are exhausted without reaching the target
        return false;
    }
    
    // Main method to verify if the given system of equality/inequality equations is satisfiable
    bool equationsPossible(vector<string>& equations) {
        // Map to represent the equality relationships between variables as a graph of adjacency lists
        unordered_map<int, vector<int>> adj;
        // Build the graph using only '==' equations to establish connected components of equal variables
        for (auto s : equations) {

            // Skip inequality equations initially as we only want to build components of equality
            if (s[1] == '!')
                continue;
            // Convert characters ('a'-'z') to 0-indexed integers (0-25) for index-based calculations
            int u = s[0] - 'a', v = s[3] - 'a';
            // Insert bidirectional edges because equality is symmetric (if u == v, then v == u)
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // Verify both equality and inequality equations for validity against the constructed graph
        for (auto s : equations) {
            // Initialize visited array for each query to ensure clean slate search paths
            vector<bool> vis(26, false);
            // If u != v is expected but a path exists between u and v, we found a contradiction
            if (s[1] == '!' && isAbleToReach(adj, s[0] - 'a', s[3] - 'a', vis))
                return false;
            // If u == v is expected but no path connects them, we found a contradiction
            if (s[1] == '=' && !isAbleToReach(adj, s[0] - 'a', s[3] - 'a', vis))
                return false;
        }
        // Return true if all equations are consistent and no contradictions are found
        return true;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of searching the graph repeatedly for each equation, we can find all connected components
// of equality relationships beforehand. We assign a unique component ID (or color) to each variable.
// Two variables have the same color if and only if they are equal.
// Finally, we check if any inequality equation ('!=') contradicts this coloring.

// Algorithm:
// 1. Build an adjacency list `adj` for all variables connected by '==' equations.
// 2. Initialize a `color` array of size 26 where -1 indicates a variable has not been assigned a component yet.
// 3. Use DFS/BFS to traverse each unvisited node. For each traversal, assign a common color (component ID)
//    to all reachable nodes in that component.
// 4. Iterate through all the equations again, looking only at '!=' relationships.
// 5. If any '!=' equation connects two variables with the same component ID (color), return false.
// 6. If no contradictions are found, return true.

// Time Complexity:
// O(N + V), where N is the number of equations and V is the number of variables (26). We build the graph in O(N),
// run DFS to find components in O(V + N) time, and then scan inequalities in O(N) time.

// Space Complexity:
// O(V) to store colors and visited array. The recursion stack for DFS is also at most O(V).

namespace Better {
class Solution {
private:
    // Helper function to color all variables in a connected component using Depth First Search
    void dfs(int node, int componentColor, const vector<vector<int>>& adj, vector<int>& color) {
        // Assign the current component's color to the variable
        color[node] = componentColor;
        // Explore all adjacent variables connected by equality relations
        for (int nbr : adj[node]) {
            // If the neighbor has not been colored yet, recursively traverse and color it
            if (color[nbr] == -1) {
                dfs(nbr, componentColor, adj, color);
            }
        }
    }

public:
    // Main function to check satisfiability of the equations
    bool equationsPossible(vector<string>& equations) {
        // Represent variable relations as an adjacency list for the 26 possible lowercase letters
        vector<vector<int>> adj(26);
        // Build the equality graph from equations containing '=='
        for (const string& eq : equations) {
            // Only process equality relationships to establish connected components
            if (eq[1] == '=') {
                // Map character variables to 0-indexed integer identifiers
                int u = eq[0] - 'a';
                int v = eq[3] - 'a';
                // Add bidirectional edges since equality is a symmetric relation
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }

        // Initialize color/component array where -1 indicates a variable has not been assigned a component yet
        vector<int> color(26, -1);

        // Run DFS from each uncolored node to find and color distinct connected components
        for (int i = 0; i < 26; ++i) {
            // If variable i is not colored, it starts a new connected component colored with ID i
            if (color[i] == -1) {
                dfs(i, i, adj, color);
            }
        }

        // Validate all inequality equations against the assigned components/colors
        for (const string& eq : equations) {
            // Only process inequality equations to find contradictions
            if (eq[1] == '!') {
                // Map character variables to 0-indexed integer identifiers
                int u = eq[0] - 'a';
                int v = eq[3] - 'a';
                // If two variables must be unequal but belong to the same component, return false
                if (color[u] == color[v]) {
                    return false;
                }
            }
        }

        // Return true if no inequality equations conflict with the equality coloring
        return true;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We use a Disjoint Set Union (DSU) / Union-Find data structure to group equal variables together.
// In the first pass, we process all equality equations '==' and union the components of the two variables.
// In the second pass, we process all inequality equations '!='. For each inequality, we check if the two variables
// already belong to the same set (i.e., they have the same representative element). If they do, we have a contradiction.

// Algorithm:
// 1. Initialize a `parent` array of size 26 where each variable is its own parent.
// 2. Define a `find` function with path compression to determine the representative element of a set.
// 3. Define a `unionSets` function to merge two sets.
// 4. Iterate through all equations and merge sets for every '==' relationship.
// 5. Iterate through all equations again and for every '!=' relationship, check if they share the same representative.
// 6. If they do share the same representative, return false. Otherwise, if all checks pass, return true.

// Time Complexity:
// O(N * alpha(V)), where N is the number of equations and V is the number of variables (26). Since alpha(V) is the
// inverse Ackermann function (which grows extremely slowly and is <= 4 for all practical values), this is essentially O(N).

// Space Complexity:
// O(V) to store the parent array, where V is 26. This is O(1) auxiliary space.

namespace Optimal {
class Solution {
private:
    // Array to store the parent/representative for each of the 26 variables
    int parent[26];

    // Find the representative of the set containing variable x using path compression
    int findParent(int x) {
        // If x is its own parent, it is the representative of its set
        if (parent[x] == x)
            return x;
        // Recursively find the root and compress paths to point directly to the root for O(1) future lookups
        return parent[x] = findParent(parent[x]);
    }

    // Merge the sets containing variables x and y
    void unionSets(int x, int y) {
        // Find representatives for both sets
        int rootX = findParent(x);
        int rootY = findParent(y);
        // If they belong to different sets, link one root to the other to merge the sets
        if (rootX != rootY) {
            parent[rootX] = rootY;
        }
    }

public:
    // Main function to check satisfiability of equations using Disjoint Set Union (DSU)
    bool equationsPossible(vector<string>& equations) {
        // Initialize DSU: make each variable its own parent representing 26 independent sets
        for (int i = 0; i < 26; ++i) {
            parent[i] = i;
        }

        // Process all equality equations to merge variables into common sets
        for (const string& eq : equations) {
            // If the relationship is equality, union the two variables
            if (eq[1] == '=') {
                // Map characters to 0-indexed integer identifiers
                int u = eq[0] - 'a';
                int v = eq[3] - 'a';
                // Merge the sets of variables u and v
                unionSets(u, v);
            }
        }

        // Verify all inequality equations to check for inconsistencies
        for (const string& eq : equations) {
            // If the relationship is inequality, ensure they are in different sets
            if (eq[1] == '!') {
                // Map characters to 0-indexed integer identifiers
                int u = eq[0] - 'a';
                int v = eq[3] - 'a';
                // If variables are supposed to be unequal but share the same representative, we have a contradiction
                if (findParent(u) == findParent(v)) {
                    return false;
                }
            }
        }

        // Return true if no contradictions are found between any equality and inequality constraints
        return true;
    }
};
}