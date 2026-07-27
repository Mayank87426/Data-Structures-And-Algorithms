class Solution {
public:
    vector<vector<TreeNode*>> levelOrder(TreeNode* root) {
        vector<vector<TreeNode*>> traversal;
        if (!root)
            return traversal;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int n = q.size();
            vector<TreeNode*> currLevel;

            while (n--) {
                TreeNode* node = q.front();
                q.pop();

                currLevel.push_back(node);

                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }

            traversal.push_back(currLevel);
        }

        return traversal;
    }

    void oddLevelReverse(TreeNode* root) {
        auto levelOrderTraversal = levelOrder(root);
        queue<TreeNode*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int n = q.size();

            vector<int> vals;
            if (level % 2) {
                for (auto node : levelOrderTraversal[level])
                    vals.push_back(node->val);
            }

            int i = vals.size() - 1;

            while (n--) {
                TreeNode* node = q.front();
                q.pop();

                if (level % 2) {
                    node->val = vals[i--];
                }

                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }

            level++;
        }
    }

    TreeNode* reverseOddLevels(TreeNode* root) {
        oddLevelReverse(root);
        return root;
    }
};