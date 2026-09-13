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
   typedef long long ll;
    bool check(TreeNode* root, ll l, ll r) {
        if(!root)return true;
        if (root->val <= l || root->val >= r)
            return false;
        return check(root->left,l,root->val)&&check(root->right,root->val,r);
    }
    bool isValidBST(TreeNode* root) {
        if(!root->left&&!root->right)return true;
        return check(root,LLONG_MIN,LLONG_MAX);
    }
};