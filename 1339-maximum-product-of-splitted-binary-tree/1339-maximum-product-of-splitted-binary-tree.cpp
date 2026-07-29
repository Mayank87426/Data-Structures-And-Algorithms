/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */

// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach simulates the removal of each edge in the binary tree.
// Every node (except the root) is connected to its parent by a single edge. Removing
// that edge splits the tree into two parts: the subtree rooted at the node itself,
// and the remaining part of the tree.
// We can traverse the entire tree, and for each node, we calculate the sum of its
// subtree using a separate recursive DFS function. The sum of the other partition
// is computed as (Total Sum - Subtree Sum). We then multiply these two sums to find
// the product and track the maximum product encountered.

// Algorithm:
// 1. Calculate the total sum of all node values in the tree using a DFS helper function.
// 2. Traverse the tree node-by-node. For each node, run a nested DFS to calculate the
//    sum of the subtree rooted at that node.
// 3. Subtract this subtree sum from the total sum to get the sum of the remaining part of the tree.
// 4. Multiply the two sums and update the maximum product.
// 5. Return the maximum product modulo 10^9 + 7.

// Time Complexity:
// O(N^2) where N is the number of nodes in the binary tree. We visit each of the N nodes,
// and at each node we calculate its subtree sum by traversing its descendants in O(N) time.

// Space Complexity:
// O(H) where H is the height of the binary tree. This space is used by the recursive call stack during tree traversal.

namespace BruteForce {
class Solution {
public:
    // Variable to track the maximum product of subtree sums encountered
    long long maxProd = 0;

    // Calculates the sum of all node values in a subtree rooted at `root` using DFS
    long long getSum(TreeNode* root) {
        // Base case: if the node is null, it contributes 0 to the sum
        if (!root)
            return 0;
        
        // Sum is computed by adding the current node's value to the recursively calculated sums of its left and right subtrees
        return root->val + getSum(root->left) + getSum(root->right);
    }

    // Calculates the total sum of the entire binary tree before checking splits
    long long getTotalSum(TreeNode* root) {
        // Base case: an empty tree has a total sum of 0
        if (!root)
            return 0;
        
        // Recursively aggregates values from all nodes in the tree
        return root->val + getTotalSum(root->left) + getTotalSum(root->right);
    }

    // Traverses every node recursively to evaluate the product if we split at the edge above it
    void calculateProduct(TreeNode* root, long long totalSum) {
        // Base case: if we reach a null node, no split can be performed here
        if (!root)
            return;

        // Calculate the subtree sum at the current node to determine one part of the split
        long long subSum = getSum(root);
        
        // The rest of the tree sum is the total sum minus the current subtree sum
        long long remainingSum = totalSum - subSum;
        
        // Update the maximum product if the current split yields a larger product
        maxProd = std::max(maxProd, subSum * remainingSum);

        // Traverse the left child to evaluate splits on the left side of the tree
        calculateProduct(root->left, totalSum);
        
        // Traverse the right child to evaluate splits on the right side of the tree
        calculateProduct(root->right, totalSum);
    }

    int maxProduct(TreeNode* root) {
        // Step 1: Pre-calculate the total sum of the entire tree to determine split parts
        long long totalSum = getTotalSum(root);
        
        // Step 2: Visit each node to evaluate the product of partitioning the tree at its parent edge
        calculateProduct(root, totalSum);
        
        // Step 3: Return the result modulo 10^9 + 7 as per the problem constraints
        return maxProd % 1000000007;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of recalculating the subtree sum from scratch for each node, which leads to
// redundant computations, we can record the sum of all subtrees in a single post-order
// traversal. Since the sum of any node's subtree depends on the sums of its children's subtrees,
// we can compute and store these sums in a list (like a vector) in O(N) time.
// Once all subtree sums are stored, we can iterate through the list of stored sums. For each
// sum, the remaining tree sum is (Total Sum - Subtree Sum). We calculate the product of the
// two subtrees, find the maximum, and return it.

// Algorithm:
// 1. Perform a post-order DFS traversal on the tree.
// 2. For each node, calculate its subtree sum as (left subtree sum + right subtree sum + node->val).
// 3. Store this sum in a helper array/vector.
// 4. The value returned by the root of this post-order DFS will be the total sum of the tree.
// 5. Iterate through all the stored subtree sums in the array, compute the product `sum * (totalSum - sum)`,
//    and record the maximum product.
// 6. Return the maximum product modulo 10^9 + 7.

// Time Complexity:
// O(N) where N is the number of nodes. We traverse the tree once to compute and store sums,
// and then iterate over the stored sums in linear time.

// Space Complexity:
// O(N) to store the subtree sums of all N nodes in a vector, plus O(H) for the recursive call stack.

namespace Better {
class Solution {
public:
    // Stores the precomputed sum of every subtree to avoid recalculating them multiple times
    std::vector<long long> subtreeSums;

    // Post-order traversal to calculate and store the sum of every subtree
    long long calculateSubtreeSums(TreeNode* root) {
        // Base case: a null node has a subtree sum of 0
        if (!root)
            return 0;

        // Recursively compute the sum of the left subtree
        long long leftSum = calculateSubtreeSums(root->left);
        
        // Recursively compute the sum of the right subtree
        long long rightSum = calculateSubtreeSums(root->right);

        // The current subtree sum is the sum of children subtrees plus the current node's value
        long long currentSum = leftSum + rightSum + root->val;

        // Record the computed subtree sum to evaluate products later
        subtreeSums.push_back(currentSum);
        
        // Return the computed sum to the parent node for its own sum calculation
        return currentSum;
    }

    int maxProduct(TreeNode* root) {
        // Step 1: Traverse the tree to calculate all subtree sums and determine the total tree sum
        long long totalSum = calculateSubtreeSums(root);

        // Variable to track the maximum product of partitioned tree sums
        long long maxProd = 0;

        // Step 2: Iterate through all recorded subtree sums to find the split that maximizes the product
        for (long long sum : subtreeSums) {
            // Calculate the sum of the remaining part of the tree for this split
            long long remainingSum = totalSum - sum;
            
            // Track the maximum product of the current split vs the previous best
            maxProd = std::max(maxProd, sum * remainingSum);
        }

        // Step 3: Return the maximum product modulo 10^9 + 7
        return maxProd % 1000000007;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach reduces the auxiliary space complexity from O(N) to O(H) by calculating
// the maximum product on the fly during a single DFS traversal after finding the total sum.
// First, we find the total sum of the tree using a standard DFS traversal.
// Then, in a second DFS traversal, for each node, we compute the sum of its subtree (post-order).
// As soon as a subtree's sum is computed, we calculate the potential product `sum * (totalSum - sum)`
// and update the maximum product in a global/member variable. This avoids the need to store the sums of
// all subtrees in a separate vector/array.

// Algorithm:
// 1. Calculate the total sum of all node values using `findTotalSumUsingDfs`.
// 2. Perform a second post-order traversal (`solve`). For each node, compute its subtree sum.
// 3. Update the running maximum product using `sum * (totalSum - sum)`.
// 4. Return the computed subtree sum to be used by the parent node.
// 5. Finally, return the maximum product modulo 10^9 + 7.

// Time Complexity:
// O(N) where N is the number of nodes. We perform two complete DFS traversals of the tree, each taking O(N) time.

// Space Complexity:
// O(H) where H is the height of the binary tree. This space is used by the recursion stack, avoiding O(N) auxiliary space.

namespace Optimal {
class Solution {
public:
    // Tracks the maximum product found by any valid split of the tree
    long long maxProd = 0;
    
    // Type definition for long long to simplify code and prevent overflow
    typedef long long ll;

    // First pass DFS: calculates the total sum of the entire binary tree
    long long findTotalSumUsingDfs(TreeNode* root) {
        // Base case: if the node is null, it contributes 0 to the total sum
        if (!root)
            return 0;
        
        // Post-order aggregation of left subtree sum, right subtree sum, and current node's value
        return findTotalSumUsingDfs(root->left) +
               findTotalSumUsingDfs(root->right) + root->val;
    }

    // Second pass DFS: calculates subtree sums and updates the maximum product on the fly
    long long solve(TreeNode* root, int totalSum) {
        // Base case: if the node is null, it has a subtree sum of 0
        if (!root)
            return 0;

        // Calculate the subtree sum by summing left child, right child, and current node's value
        ll sum = solve(root->left, totalSum) + solve(root->right, totalSum) +
                 root->val;
        
        // Update the maximum product with the product of current subtree and the remaining tree
        maxProd = max(maxProd, sum * (totalSum-sum));
        
        // Return the current subtree sum to be used by parent nodes in their calculations
        return sum;
    }

    int maxProduct(TreeNode* root) {
        // Step 1: Calculate the total sum of the binary tree
        long long totalSum = findTotalSumUsingDfs(root);
        
        // Step 2: Compute subtree sums recursively and evaluate maximum product on the fly
        int v = solve(root, totalSum);
        
        // Step 3: Return the maximum product modulo 10^9 + 7 as requested
        return maxProd % 1000000007;
    }
};
}