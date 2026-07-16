class TaskManager {
public:
    multiset<vector<int>> assignedTasks;
    unordered_map<int,pair<int,int>>taskToUserPriority;
    TaskManager(vector<vector<int>>& tasks) {
        for (auto t : tasks) {
            vector<int> currTask = t;
            reverse(begin(currTask), end(currTask));
            assignedTasks.insert(currTask);
            taskToUserPriority[t[1]]={t[0],t[2]};
        }
    }

    void add(int userId, int taskId, int priority) {
        assignedTasks.insert({priority, taskId, userId});
        taskToUserPriority[taskId] = { userId, priority };
    }

    void edit(int taskId, int newPriority) {
        auto p = taskToUserPriority[taskId];
        vector<int> oldTaskEntry = {p.second, taskId, p.first};
        assignedTasks.erase(assignedTasks.find(oldTaskEntry));
        assignedTasks.insert({newPriority, taskId, p.first});
        taskToUserPriority[taskId] = {p.first, newPriority};
    }

    void rmv(int taskId) {
        auto p = taskToUserPriority[taskId];
        vector<int> taskToBeRemoved = {p.second, taskId, p.first};
        assignedTasks.erase(assignedTasks.find(taskToBeRemoved));
    }

    int execTop() {
        if(assignedTasks.empty())return -1;
        auto topPriorityTask = *assignedTasks.rbegin();
        int userId=topPriorityTask[2];
        assignedTasks.erase(assignedTasks.find(topPriorityTask));
        return userId;
    }
};

/**
 * Your TaskManager object will be instantiated and called as such:
 * TaskManager* obj = new TaskManager(tasks);
 * obj->add(userId,taskId,priority);
 * obj->edit(taskId,newPriority);
 * obj->rmv(taskId);
 * int param_4 = obj->execTop();
 */