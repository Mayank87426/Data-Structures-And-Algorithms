// ===============================
// Brute Force Approach
// ===============================

// Approach:
// We perform a standard Breadth-First Search (BFS) level-order traversal. For every level we visit, if it is 
// an odd level, we collect all the nodes of that level into a temporary list. Once all nodes of that level 
// are collected, we use two pointers (one at the beginning, one at the end) to swap their values, moving 
// towards the middle. This reverses the values at the odd level.

// Algorithm:
// 1. Initialize a queue with the root node.
// 2. Maintain a level tracker starting at 0.
// 3. While the queue is not empty, get the number of nodes at the current level.
// 4. Create a vector `currentLevelNodes` to store the nodes of the current level if the level is odd.
// 5. Traverse all nodes of the current level. For each node, pop it from the queue.
// 6. If the current level is odd, append the node to `currentLevelNodes`.
// 7. Push the children (left and right) of the current node to the queue if they exist.
// 8. If the current level is odd, use two pointers (`left` and `right`) to swap the values of `currentLevelNodes[left]` and `currentLevelNodes[right]`, incrementing `left` and decrementing `right` until they meet.
// 9. Increment the level tracker and repeat until the queue is empty.
// 10. Return the root of the tree.

// Time Complexity:
// O(N) where N is the number of nodes in the binary tree. Each node is visited and processed a constant number of times.

// Space Complexity:
// O(N) since the queue and the level-nodes array can store up to the maximum width of the tree at the deepest level, which is O(N) for a perfect binary tree.

namespace BruteForce {
class Solution {
public:
    TreeNode* reverseOddLevels(TreeNode* root) {
        // Return null if the tree is empty
        if (!root) {
            return nullptr;
        }
        
        // Queue to store nodes for level-order traversal (BFS)
        queue<TreeNode*> q;
        // Start traversal by pushing the root node
        q.push(root);
        // Track the current level index starting from 0 (root)
        int level = 0;
        
        // Loop until all levels of the tree are traversed
        while (!q.empty()) {
            // Count the number of nodes at the current level
            int size = q.size();
            // Vector to hold nodes of the current level for swapping (only used on odd levels)
            vector<TreeNode*> currentLevelNodes;
            
            // Process all nodes at the current level
            for (int i = 0; i < size; ++i) {
                // Get the front node from the queue
                TreeNode* node = q.front();
                // Remove the processed node from the queue
                q.pop();
                
                // If it is an odd level, collect the node to swap its value later
                if (level % 2 == 1) {
                    currentLevelNodes.push_back(node);
                }
                
                // Push the left child to the queue if it exists
                if (node->left) {
                    q.push(node->left);
                }
                // Push the right child to the queue if it exists
                if (node->right) {
                    q.push(node->right);
                }
            }
            
            // If we just finished processing an odd level, swap values of the collected nodes
            if (level % 2 == 1) {
                // Pointer to the start of the level nodes
                int left = 0;
                // Pointer to the end of the level nodes
                int right = currentLevelNodes.size() - 1;
                // Swap values from outer ends moving towards the middle
                while (left < right) {
                    // Temporarily store the left node's value
                    int temp = currentLevelNodes[left]->val;
                    // Copy the right node's value to the left node
                    currentLevelNodes[left]->val = currentLevelNodes[right]->val;
                    // Assign the temporary value to the right node
                    currentLevelNodes[right]->val = temp;
                    // Move the left pointer forward
                    left++;
                    // Move the right pointer backward
                    right--;
                }
            }
            // Move to the next level
            level++;
        }
        // Return the modified root of the tree
        return root;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This approach uses BFS (Level-order traversal) with a space-saving optimization. Instead of storing entire 
// TreeNode* objects in a vector for swapping, we only store the values (int) of the next level when we are at 
// an even level (meaning the next level is odd). Since we traverse level-by-level from left to right, the 
// values in `prev` represent the odd level nodes from left to right. When we traverse the odd level, we update 
// each node's value to the last element of `prev` (and pop it), which effectively reverses the level's values 
// because we access `prev` from right to left (LIFO order).

// Algorithm:
// 1. Initialize a queue `q` with the root node.
// 2. Define `level = 0` and a vector `prev` to store values of the next level.
// 3. While `q` is not empty, determine the number of nodes at the current level, `n`.
// 4. Inside the loop, pop each node.
// 5. If the node has a left child:
//    - Push it to the queue.
//    - If the current level is even (meaning the child is at an odd level), push its value into `prev`.
// 6. If the node has a right child:
//    - Push it to the queue.
//    - If the current level is even (meaning the child is at an odd level), push its value into `prev`.
// 7. If the current level is odd:
//    - Assign `node->val` to the last value of `prev` (`prev.back()`) and remove it from `prev` (`prev.pop_back()`).
// 8. Increment `level` and repeat.
// 9. Return the root of the tree.

// Time Complexity:
// O(N) because every node is visited exactly once.

// Space Complexity:
// O(N) due to the queue and the `prev` values array (which stores up to N/2 values at the maximum level width).

namespace Better {
class Solution {
public:
    // Helper function to perform BFS and reverse values of odd levels
    void oddLevelReverse(TreeNode* root) {
        // Queue to store nodes for level-order traversal
        queue<TreeNode*> q;
        // Start the traversal by pushing the root node
        q.push(root);
        // Track the current level index starting from 0
        int level = 0;
        // Vector to store node values of the odd levels (populated during even levels)
        vector<int> prev;
        
        // Loop until all levels of the tree are traversed
        while (!q.empty()) {
            // Count the number of nodes at the current level
            int n = q.size();
            // Process each node at the current level
            while (n--) {
                // Get the node at the front of the queue
                auto node = q.front();
                // Pop the processed node from the queue
                q.pop();
                
                // If a left child exists, push it to the queue
                if (node->left) {
                    q.push(node->left);
                    // If current level is even, the child level is odd; record its value
                    if (level % 2 == 0)
                        prev.push_back(node->left->val);
                }
                // If a right child exists, push it to the queue
                if (node->right) {
                    q.push(node->right);
                    // If current level is even, the child level is odd; record its value
                    if (level % 2 == 0)
                        prev.push_back(node->right->val);
                }
                // If current level is odd, overwrite the node's value from the end of prev
                if (level % 2 == 1) {
                    // Overwrite the current node's value with the corresponding reversed value
                    node->val = prev.back();
                    // Remove the used value from the back of the vector
                    prev.pop_back();
                }
            }
            // Move to the next level
            level++;
        }
    }

    // Main function to reverse values at odd levels of the perfect binary tree
    TreeNode* reverseOddLevels(TreeNode* root) {
        // Call the BFS helper function to reverse values in-place
        oddLevelReverse(root);
        // Return the modified root node
        return root;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Since the tree is guaranteed to be a perfect binary tree, we can perform a simultaneous Depth-First Search (DFS) 
// on two symmetric subtrees. We can traverse the tree using two pointers: one starting at the left child and 
// one starting at the right child of the root. At each level of recursion, if the level is odd, we swap the 
// values of the two symmetric nodes. Then, we recursively call the traversal function on the outer and inner 
// children. This approach avoids the queue overhead and runs with O(log N) stack space.

// Algorithm:
// 1. If root is null, return nullptr.
// 2. Call the helper function `traverse(root->left, root->right, 1)`.
// 3. In `traverse(leftNode, rightNode, level)`:
//    - If leftNode or rightNode is null, return (base case).
//    - If the level is odd, swap the values of leftNode and rightNode.
//    - Recursively call `traverse(leftNode->left, rightNode->right, level + 1)`.
//    - Recursively call `traverse(leftNode->right, rightNode->left, level + 1)`.
// 4. Return the root.

// Time Complexity:
// O(N) since we visit every node in the tree once.

// Space Complexity:
// O(log N) auxiliary space for the recursion stack (since the tree is perfect, height is O(log N)).

namespace Optimal {
class Solution {
private:
    // Helper function to traverse the tree symmetrically and swap values at odd levels
    void traverse(TreeNode* leftNode, TreeNode* rightNode, int level) {
        // Base case: if either node is null, we have reached beyond the leaf level
        if (!leftNode || !rightNode) {
            return;
        }
        
        // If the current level is odd, swap the values of the symmetric nodes
        if (level % 2 == 1) {
            // Store the value of the left node temporarily
            int temp = leftNode->val;
            // Copy the right node's value to the left node
            leftNode->val = rightNode->val;
            // Assign the temporary value to the right node
            rightNode->val = temp;
        }
        
        // Recursively traverse the outer children (left's left and right's right)
        traverse(leftNode->left, rightNode->right, level + 1);
        // Recursively traverse the inner children (left's right and right's left)
        traverse(leftNode->right, rightNode->left, level + 1);
    }

public:
    // Main function to reverse values at odd levels of the perfect binary tree
    TreeNode* reverseOddLevels(TreeNode* root) {
        // Check if root is not null before starting the symmetric traversal
        if (root) {
            // Start traversal with the left and right children of root at level 1 (odd level)
            traverse(root->left, root->right, 1);
        }
        // Return the root of the modified binary tree
        return root;
    }
};
}