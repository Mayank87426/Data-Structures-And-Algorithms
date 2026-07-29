// ===============================
// Brute Force Approach
// ===============================

// Approach:
// We model variables as nodes and equations as weighted directed edges.
// An equation a / b = v is represented as a directed edge a -> b with weight v,
// and b -> a with weight 1/v.
// For each query C / D, we use Depth First Search (DFS) to find a path from C to D.
// The product of edge weights along the path gives the division result.
// A visited set is used to prevent cycles during the search.

// Algorithm:
// 1. Build an adjacency list graph mapping each variable to its neighbors and division weights.
// 2. For each equation [A, B] with value V, add a -> (b, V) and b -> (a, 1.0/V).
// 3. For each query [C, D]:
//    - If C or D is not in the graph, answer is -1.0.
//    - Reinitialize visited set.
//    - Run DFS(C, D) to find a path.
//    - If DFS finds D, return 1.0 and multiply by edge weights during backtracking.
//    - If no path is found, return -1.0.

// Time Complexity:
// O(Q * (V + E)) where Q is the number of queries, V is the number of variables (nodes),
// and E is the number of equations (edges). For each query, we may traverse the entire graph.

// Space Complexity:
// O(V + E) for the graph representation and the DFS recursion call stack / visited set.

namespace BruteForce {
class Solution {
public:
    double dfs(string src, string dest,
               unordered_map<string, vector<pair<string, double>>> &graph,
               unordered_set<string> &vis) {

        // Check if the current node is the target node
        if (src == dest)
            return 1.0; // Base case: A variable divided by itself equals 1.0, terminating the path search.


        // Insert the current variable into the visited set
        vis.insert(src); // Prevent traversing back to this variable and getting stuck in cycles.


        // Explore all connected variables (neighbors)
        for (auto &nbr : graph[src]) {

            // Get neighbor variable name
            string next = nbr.first; // Retrieve the next node in path.
            // Get weight of current transition
            double weight = nbr.second; // Retrieve ratio of src to next.


            // Avoid visiting already explored nodes
            if (!vis.count(next)) { // Only traverse unvisited nodes to ensure we don't loop back.

                // Search recursively from next node to destination
                double result = dfs(next, dest, graph, vis); // Propagate forward through graph.


                // If destination was found, multiply weights and return
                if (result != -1.0) { // Check if valid path to target was found.
                    return weight * result; // Compute cumulative ratio along the valid path.
                }
            }
        }


        // No path exists from src to dest
        return -1.0; // Return -1.0 to signal no valid division path exists.
    }


    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {

        // Build the graph using hash maps
        unordered_map<string, vector<pair<string, double>>> graph; // Adjacency list representation.


        // Populate adjacency list with given equations
        for (int i = 0; i < equations.size(); i++) {

            // Get variable nodes
            string a = equations[i][0]; // Dividend variable.
            string b = equations[i][1]; // Divisor variable.

            // Forward relation: a / b = values[i]
            graph[a].push_back({b, values[i]}); // Store direct ratio.

            // Backward relation: b / a = 1.0 / values[i]
            graph[b].push_back({a, 1.0 / values[i]}); // Store inverse ratio.
        }



        // Store query answers
        vector<double> ans; // Result list.


        // Process each query
        for (auto &query : queries) {

            // Get query variables
            string src = query[0]; // Dividend.
            string dest = query[1]; // Divisor.


            // Check if both query variables exist in the graph
            if (!graph.count(src) || !graph.count(dest)) { // Both variables must be present to resolve.
                ans.push_back(-1.0); // If undefined, return -1.0.
                continue; // Skip processing.
            }


            // Set to keep track of visited nodes per query
            unordered_set<string> vis; // Clear visited history for fresh search.


            // DFS finds a path from src to dest
            ans.push_back(dfs(src, dest, graph, vis)); // Run path search and append result.
        }


        // Return computed results
        return ans; // Return list of answers.
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Floyd-Warshall algorithm can be used to find the path (and evaluate the division)
// between all pairs of nodes. Since the number of unique variables is small (at most 40),
// we can assign an integer ID to each variable. We initialize a 2D matrix representing
// the division values between nodes. For equations a / b = v, table[a][b] = v and table[b][a] = 1/v.
// All other entries are initialized to -1.0, and self-loops table[i][i] = 1.0.
// Then, we run Floyd-Warshall to update all pair values: if table[i][k] and table[k][j] are valid (> 0),
// we update table[i][j] = table[i][k] * table[k][j].
// For each query, we lookup the value in the table in O(1) time.

// Algorithm:
// 1. Map each variable to a unique integer ID.
// 2. Initialize a grid of size V x V with -1.0, and set grid[i][i] = 1.0 for all i.
// 3. For each equation a / b = value, populate grid[id(a)][id(b)] = value and grid[id(b)][id(a)] = 1.0 / value.
// 4. Run Floyd-Warshall: triple nested loop for k, i, j from 0 to V-1.
// 5. If grid[i][k] > 0 and grid[k][j] > 0, set grid[i][j] = grid[i][k] * grid[k][j].
// 6. For each query C / D: if C or D are not in the variable ID map, query value is -1.0.
//    Otherwise, query value is grid[id(C)][id(D)].

// Time Complexity:
// O(V^3 + Q) where V is the number of unique variables (<= 40) and Q is the number of queries.

// Space Complexity:
// O(V^2) to store the pair-wise division table.

namespace Better {
class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        // Map string variables to unique integer indices
        unordered_map<string, int> varMap; // Assign integer IDs to unique variables.
        int varCount = 0; // Maintain count of unique variables.
        
        // Find unique variables and map them to IDs
        for (const auto& eq : equations) {
            if (varMap.find(eq[0]) == varMap.end()) {
                varMap[eq[0]] = varCount++; // Insert dividend variable.
            }
            if (varMap.find(eq[1]) == varMap.end()) {
                varMap[eq[1]] = varCount++; // Insert divisor variable.
            }
        }
        
        // Initialize the table with -1.0 indicating unreachable state
        vector<vector<double>> table(varCount, vector<double>(varCount, -1.0)); // Grid of size V x V.
        
        // Base case: a variable divided by itself is 1.0
        for (int i = 0; i < varCount; ++i) {
            table[i][i] = 1.0; // Self-division ratio.
        }
        
        // Populate direct division values from equations
        for (size_t i = 0; i < equations.size(); ++i) {
            int u = varMap[equations[i][0]]; // ID for dividend.
            int v = varMap[equations[i][1]]; // ID for divisor.
            table[u][v] = values[i]; // Forward ratio.
            table[v][u] = 1.0 / values[i]; // Backward ratio.
        }
        
        // Run Floyd-Warshall algorithm to find all-pairs division values
        for (int k = 0; k < varCount; ++k) { // Intermediate node.
            for (int i = 0; i < varCount; ++i) { // Source node.
                for (int j = 0; j < varCount; ++j) { // Target node.
                    // If a valid path exists through intermediate node k
                    if (table[i][k] > 0.0 && table[k][j] > 0.0) {
                        table[i][j] = table[i][k] * table[k][j]; // Compute combined ratio.
                    }
                }
            }
        }
        
        // Answer each query using the precomputed table
        vector<double> ans; // List of results.
        for (const auto& q : queries) {
            // Check if query variables are present in the map
            if (varMap.find(q[0]) == varMap.end() || varMap.find(q[1]) == varMap.end()) {
                ans.push_back(-1.0); // Return -1.0 if variables are unknown.
            } else {
                int u = varMap[q[0]]; // Dividend index.
                int v = varMap[q[1]]; // Divisor index.
                ans.push_back(table[u][v]); // Retrieve direct precomputed value.
            }
        }
        return ans; // Return list of answers.
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Union-Find (Disjoint Set Union) with weights. We can treat variables as elements of sets.
// For each equation a / b = val, we union the sets containing a and b.
// Each node maintains a parent pointer and a weight relative to its parent, such that weight[x] = x / parent[x].
// During find operations, we perform path compression and update the weights transitively.
// For a query C / D:
// 1. If C or D are not in the equations, return -1.0.
// 2. Find the representative root of C and D.
// 3. If they belong to different roots (disjoint components), there is no path, return -1.0.
// 4. If they have the same root, C / D = (C / root) / (D / root) = weight[C] / weight[D].

// Algorithm:
// 1. Initialize parent map parent[x] = x and weight map weight[x] = 1.0 for all unique variables.
// 2. For each equation [A, B] with value val, union A and B:
//    - Find rootA and rootB.
//    - If rootA != rootB, set parent[rootA] = rootB and weight[rootA] = val * weight[B] / weight[A].
// 3. For each query [C, D]:
//    - If C or D do not exist in the DSU, return -1.0.
//    - Find rootC and rootD.
//    - If rootC != rootD, return -1.0.
//    - Otherwise, return weight[C] / weight[D].

// Time Complexity:
// O((N + Q) * alpha(V)) where N is the number of equations, Q is the number of queries,
// V is the number of variables, and alpha is the Inverse Ackermann function.

// Space Complexity:
// O(V) to store the parents and weights in hash maps.

namespace Optimal {
class Solution {
private:
    unordered_map<string, string> parent; // Store parent representative for each variable.
    unordered_map<string, double> weight; // Store ratio of variable to parent.

    // Path compression in DSU, updates weights relative to component root
    string find(string x) {
        if (parent[x] == x) { // Check if node is the root.
            return x; // Root returns itself.
        }
        string original_parent = parent[x]; // Cache old parent.
        string root = find(original_parent); // Recursively find root.
        weight[x] *= weight[original_parent]; // Update weight relative to new root.
        parent[x] = root; // Perform path compression.
        return root; // Return final root.
    }

    // Merge two components and establish their relative ratio
    void unionNodes(string x, string y, double val) {
        string rootX = find(x); // Find root of first node.
        string rootY = find(y); // Find root of second node.
        if (rootX != rootY) { // If roots differ, merge.
            parent[rootX] = rootY; // Connect first component root to second root.
            weight[rootX] = val * weight[y] / weight[x]; // Calculate ratio of rootX / rootY.
        }
    }

public:
    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        // Initialize DSU for each unique variable
        for (const auto& eq : equations) {
            string u = eq[0]; // Dividend variable.
            string v = eq[1]; // Divisor variable.
            if (parent.find(u) == parent.end()) {
                parent[u] = u; // Self-parent pointer initialization.
                weight[u] = 1.0; // Self ratio is 1.0.
            }
            if (parent.find(v) == parent.end()) {
                parent[v] = v; // Self-parent pointer initialization.
                weight[v] = 1.0; // Self ratio is 1.0.
            }
        }

        // Perform union operations for all equations
        for (size_t i = 0; i < equations.size(); ++i) {
            unionNodes(equations[i][0], equations[i][1], values[i]); // Union nodes with edge ratio.
        }

        vector<double> ans; // List of results.
        for (const auto& q : queries) {
            string c = q[0]; // Query dividend.
            string d = q[1]; // Query divisor.
            // Check if both query variables exist in the DSU map
            if (parent.find(c) == parent.end() || parent.find(d) == parent.end()) {
                ans.push_back(-1.0); // Return -1.0 if variables are unknown.
            } else {
                string rootC = find(c); // Find root of dividend.
                string rootD = find(d); // Find root of divisor.
                if (rootC != rootD) { // If they belong to different roots, no path exists.
                    ans.push_back(-1.0); // Unconnected components evaluation is -1.0.
                } else {
                    ans.push_back(weight[c] / weight[d]); // Compute direct ratio c / d.
                }
            }
        }
        return ans; // Return list of query answers.
    }
};
}

/*
=========================================================
MISTAKES IN PREVIOUS CODE:
=========================================================


1) Treating variables as characters instead of strings

Wrong:
    unordered_map<char, vector<pair<char,double>>> mp;


Problem:
    Variable names are complete strings.

    Example:
        "apple" / "banana"

    You cannot split them into:
        'a' -> 'a'
        'e' -> 'b'


Correct:
    unordered_map<string, vector<pair<string,double>>> graph;



---------------------------------------------------------


2) Wrong graph construction

You did:

    mp[x[0]].push_back({y[b-1], value});


Example:

    abc / xyz = 2


Your graph becomes:

    a -> z


But the actual relation is:

    "abc" -> "xyz"


Variables are nodes, not characters.



---------------------------------------------------------


3) Incorrect query handling

You did:

    if (x.length() != y.length())
        return -1;


This is wrong.

Example:

    a / apple

can be valid if a relation exists.

Length of variable names has no meaning.



---------------------------------------------------------


4) Multiplying all DFS paths

You had:

    ans = ans * edge * dfs(...)


Problem:

If a node has multiple neighbours,
you multiply answers from different paths.

DFS should return when the FIRST valid path is found.

Correct:

    if(result != -1)
        return edge * result;



---------------------------------------------------------


5) Using vector<bool>(26)

You assumed variables are:

    'a' to 'z'


But variables can be:

    "x"
    "abc"
    "temperature"


Therefore use:

    unordered_set<string> visited;


=========================================================
REVISION SUMMARY:

1. Make graph:
       variable -> (connected variable, ratio)

2. Add reverse edges:
       a/b = k
       b/a = 1/k

3. For every query:
       DFS from source to destination

4. Multiply weights along the path

5. If no path exists:
       return -1

=========================================================
*/