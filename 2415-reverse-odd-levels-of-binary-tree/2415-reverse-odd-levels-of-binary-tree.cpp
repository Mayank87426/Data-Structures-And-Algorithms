class Solution {
public:
    void oddLevelReverse(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        int level = 0;
        vector<int> prev;
        while (!q.empty()) {
            int n = q.size();
            while (n--) {
                auto node = q.front();
                q.pop();
                if (node->left) {
                    q.push(node->left);
                    if (level % 2 == 0)
                        prev.push_back(node->left->val);
                }
                if (node->right) {
                    q.push(node->right);
                    if (level % 2 == 0)
                        prev.push_back(node->right->val);
                }
                if (level % 2 == 1) {
                    node->val = prev.back();
                    prev.pop_back();
                }
            }
            level++;
        }
    }

    TreeNode* reverseOddLevels(TreeNode* root) {
        oddLevelReverse(root);
        return root;
    }
};