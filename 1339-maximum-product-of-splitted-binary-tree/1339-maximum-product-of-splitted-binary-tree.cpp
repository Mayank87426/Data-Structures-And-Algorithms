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
class Solution {
public:
    long long maxProd = 0;
    typedef long long ll;
    long long findTotalSumUsingDfs(TreeNode* root) {
        if (!root)
            return 0;
        return findTotalSumUsingDfs(root->left) +
               findTotalSumUsingDfs(root->right) + root->val;
    }

    long long solve(TreeNode* root, int totalSum) {
        if (!root)
            return 0;

        ll sum = solve(root->left, totalSum) + solve(root->right, totalSum) +
                 root->val;
        maxProd = max(maxProd, sum * (totalSum-sum));
        return sum;
    }

    int maxProduct(TreeNode* root) {
        long long totalSum = findTotalSumUsingDfs(root);
        int v = solve(root, totalSum);
        return maxProd % 1000000007;
    }
};