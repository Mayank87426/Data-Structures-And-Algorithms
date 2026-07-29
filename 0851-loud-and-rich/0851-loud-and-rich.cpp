// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For each person, we perform a standard depth-first search (DFS) traversal to find all
// reachable richer people. Since we do not memoize results between different starting nodes,
// we potentially visit nodes and edges multiple times. For each starting node, we keep
// track of the quietest person encountered and store this in our answer array.

// Algorithm:
// 1. Build an adjacency list representation of the richer relationships, where an edge goes from a poorer person to a richer person.
// 2. For each person i from 0 to n - 1:
//    a. Initialize the quietest person found so far as i.
//    b. Run a helper DFS from i to traverse all richer people.
//    c. Use a visited array to prevent visiting the same person multiple times during this search.
//    d. In DFS, for each neighbor (richer person):
//       i. If not visited, mark it visited, update the quietest candidate if quiet[neighbor] is smaller, and recursively visit its richer neighbors.
//    e. Store the quietest person found in ans[i].
// 3. Return the populated answer array.

// Time Complexity:
// O(N * (N + E)) where N is the number of people and E is the number of relations in richer. We perform a DFS of size O(N + E) for each of the N people.

// Space Complexity:
// O(N + E) to store the graph, the recursion stack, and the visited array.

namespace BruteForce {
class Solution {
public:
    // Helper function to perform DFS and find the quietest richer person
    void dfs(int node, int& quietest_person, const vector<vector<int>>& graph, const vector<int>& quiet, vector<bool>& visited) {
        // Mark the current node as visited to avoid visiting it again in the current path
        visited[node] = true;
        
        // Update the quietest person if the current node has a smaller quietness value
        if (quiet[node] < quiet[quietest_person]) {
            quietest_person = node;
        }
        
        // Visit all directly richer people from the current node
        for (int richer_person : graph[node]) {
            // Recursively search the richer person if they haven't been visited yet
            if (!visited[richer_person]) {
                dfs(richer_person, quietest_person, graph, quiet, visited);
            }
        }
    }

    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        // Construct adjacency list where graph[poor] contains list of richer people
        vector<vector<int>> graph(n);
        for (const auto& relation : richer) {
            graph[relation[1]].push_back(relation[0]);
        }

        vector<int> ans(n);
        // Compute the result for each person individually
        for (int i = 0; i < n; i++) {
            // Keep track of visited nodes to handle redundant paths in DAG
            vector<bool> visited(n, false);
            // Initialize the quietest person found so far to be the starting person themselves
            int quietest_person = i;
            // Traverse all reachable richer nodes starting from person i
            dfs(i, quietest_person, graph, quiet, visited);
            // Store the quietest candidate in the answer array
            ans[i] = quietest_person;
        }

        return ans;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We can use a topological sort (Kahn's Algorithm) by reversing the relationship. We construct
// a directed graph where edges point from richer to poorer individuals. The indegree of a node
// represents the count of people who are directly richer than that node. By processing nodes with
// an indegree of 0 first, we propagate the best quietest person from richer people to poorer people.

// Algorithm:
// 1. Build an adjacency list where edges go from a richer person to a poorer person, and compute indegrees.
// 2. Initialize the answer array such that each person's quietest known richer person is initially themselves (ans[i] = i).
// 3. Queue all nodes that have an indegree of 0 (no known richer people).
// 4. While the queue is not empty:
//    a. Pop a person `u` from the queue.
//    b. For each person `v` who is directly poorer than `u`:
//       i. If the quietest person reachable from `u` (stored in ans[u]) has a lower quietness value
//          than the quietest person currently recorded for `v` (stored in ans[v]), update ans[v] = ans[u].
//       ii. Decrement the indegree of `v`. If the indegree of `v` becomes 0, push `v` to the queue.
// 5. Return the populated answer array.

// Time Complexity:
// O(N + E) where N is the number of people and E is the size of the richer array, as we visit each node and edge once.

// Space Complexity:
// O(N + E) to store the graph, indegrees, and queue.

namespace Better {
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        // Construct graph and track indegrees (richer -> poorer)
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);
        for (const auto& relation : richer) {
            int rich = relation[0];
            int poor = relation[1];
            graph[rich].push_back(poor);
            indegree[poor]++;
        }

        // Initialize the answer array: everyone is initially their own quietest person
        vector<int> ans(n);
        for (int i = 0; i < n; i++) {
            ans[i] = i;
        }

        // Vector acting as a queue for topological sort
        vector<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push_back(i);
            }
        }

        int head = 0;
        // Process nodes in topological order
        while (head < q.size()) {
            int u = q[head++];
            // Propagate the quietest person from u to all directly poorer individuals v
            for (int v : graph[u]) {
                // If the candidate quietest person from u is quieter than the current candidate for v
                if (quiet[ans[u]] < quiet[ans[v]]) {
                    ans[v] = ans[u];
                }
                // Decrement indegree; if no more richer people need to be processed for v, queue it
                indegree[v]--;
                if (indegree[v] == 0) {
                    q.push_back(v);
                }
            }
        }

        return ans;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We use a memoized Depth First Search (DFS) on the directed acyclic graph (DAG) of richer relations.
// An edge is constructed from a poorer person to a richer person. For each node, we recursively
// search all richer paths to find the person with the smallest quietness value. By caching/memoizing
// the results in the answer array `ans`, each node's quietest person is computed exactly once.

// Algorithm:
// 1. Build an adjacency list graph where a directed edge points from a poorer person to a richer person.
// 2. Initialize the answer array `ans` with -1 to indicate uncomputed states.
// 3. For each person from 0 to n - 1, perform a memoized DFS.
// 4. In `dfs(node)`:
//    a. If `ans[node]` is already computed (not -1), return it.
//    b. Otherwise, initialize `ans[node] = node`.
//    c. For each direct neighbor (which represents a richer person):
//       i. Find their quietest person recursively using DFS.
//       ii. If that person is quieter than the current `ans[node]`, update `ans[node]`.
//    d. Return `ans[node]`.
// 5. Return the populated answer array.

// Time Complexity:
// O(N + E) where N is the number of people and E is the size of the richer array, as we visit each node and edge at most once.

// Space Complexity:
// O(N + E) for storing the graph, the recursion stack, and the cached results.

namespace Optimal {
class Solution {
public:
    // Adjacency list representation of the graph: graph[poor] contains lists of richer people
    vector<vector<int>> graph;
    // Memoization array storing the quietest person for each node
    vector<int> ans;
    // Local copy of the quietness array to avoid passing it in recursion
    vector<int> quiet;

    // Recursive helper to perform DFS with memoization
    int dfs(int node) {
        // If the result for the current node has already been calculated, return it
        if (ans[node] != -1)
            return ans[node];

        // Default the quietest person for this node to the node itself
        ans[node] = node;

        // Iterate through all people who are directly richer than the current person
        for (int rich : graph[node]) {
            // Find the quietest person among the reachable richer individuals
            int candidate = dfs(rich);

            // If the candidate's quietness is lower than the current best candidate, update the answer
            if (quiet[candidate] < quiet[ans[node]])
                ans[node] = candidate;
        }

        // Return the final quietest person found for the current node
        return ans[node];
    }

    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();

        // Resize the adjacency list to support n people
        graph.resize(n);
        // Initialize the memoization array with -1 to represent unvisited nodes
        ans.assign(n, -1);
        // Reference the quiet array using the class member
        this->quiet = quiet;

        // Build the graph where each edge points from a poorer person to a richer person
        for (auto& e : richer)
            graph[e[1]].push_back(e[0]);

        // Run memoized DFS starting from each node
        for (int i = 0; i < n; i++)
            dfs(i);

        // Return the finalized answer array
        return ans;
    }
};
}