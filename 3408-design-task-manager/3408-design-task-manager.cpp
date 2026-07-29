// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach stores the tasks in a hash map mapping each taskId to its userId and priority.
// To add, edit, or remove a task, we simply update the hash map in O(1) time.
// To execute the top task, we scan all active tasks in the hash map to find the one with the highest priority 
// (and the highest taskId in case of a tie). We then remove it from the hash map and return its userId.

// Algorithm:
// 1. Store task details (userId, priority) in an unordered_map using taskId as the key.
// 2. add(userId, taskId, priority): Insert the mapping taskId -> {userId, priority} into the map.
// 3. edit(taskId, newPriority): Look up the taskId in the map and update its priority to newPriority.
// 4. rmv(taskId): Erase the taskId from the map.
// 5. execTop(): Iterate through all entries in the map. Keep track of the task with the maximum priority.
//    If we find a task with a higher priority, or a task with the same priority but a larger taskId, update our candidate.
//    Erase the chosen task from the map and return its userId. Return -1 if the map is empty.

// Time Complexity:
// - TaskManager(): O(N) where N is the number of initial tasks.
// - add(): O(1) average time.
// - edit(): O(1) average time.
// - rmv(): O(1) average time.
// - execTop(): O(T) where T is the current number of active tasks.

// Space Complexity:
// - O(T) to store the active tasks in the hash map.

namespace BruteForce {
class TaskManager {
public:
    // Stores active tasks mapping taskId to a pair of {userId, priority} for constant time lookups and updates.
    unordered_map<int, pair<int, int>> taskToUserPriority;

    TaskManager(vector<vector<int>>& tasks) {
        // Populate the mapping with the initial set of tasks provided during initialization.
        for (auto t : tasks) {
            // Map taskId (t[1]) to its userId (t[0]) and priority (t[2]).
            taskToUserPriority[t[1]] = {t[0], t[2]};
        }
    }

    void add(int userId, int taskId, int priority) {
        // Store the new task's details in the map to enable future lookups, updates, and existence checks.
        taskToUserPriority[taskId] = {userId, priority};
    }

    void edit(int taskId, int newPriority) {
        // Directly update the task's priority in the map using its taskId key since user ID remains unchanged.
        taskToUserPriority[taskId].second = newPriority;
    }

    void rmv(int taskId) {
        // Erase the entry from the map to mark the task as removed from the system.
        taskToUserPriority.erase(taskId);
    }

    int execTop() {
        // If the system has no active tasks, execution cannot proceed, so return -1 as specified.
        if (taskToUserPriority.empty()) return -1;

        int targetTaskId = -1;
        int targetUserId = -1;
        int maxPriority = -1;

        // Iterate through all active tasks to find the task with the highest priority.
        for (auto entry : taskToUserPriority) {
            int taskId = entry.first;
            int userId = entry.second.first;
            int priority = entry.second.second;

            // Select task with larger priority, or resolve tie by choosing the task with the larger taskId.
            if (priority > maxPriority || (priority == maxPriority && taskId > targetTaskId)) {
                maxPriority = priority;
                targetTaskId = taskId;
                targetUserId = userId;
            }
        }

        // Erase the executed task from the system as it has been executed and should not be processed again.
        taskToUserPriority.erase(targetTaskId);

        // Return the userId of the executed task to indicate which user's task was completed.
        return targetUserId;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This approach maintains tasks in a multiset sorted by priority and taskId to quickly find the top task,
// alongside a hash map to map taskId to user and priority.
// Since multiset stores elements in sorted order, the last element (rbegin()) represents the task with the
// highest priority (and highest taskId in case of a tie). When tasks are updated or removed, they are 
// eagerly deleted from the multiset by constructing the exact matching element and locating it.

// Algorithm:
// 1. Use a multiset to store tasks in the format {priority, taskId, userId}.
// 2. Use an unordered_map to map taskId to {userId, priority}.
// 3. TaskManager(): Populate both containers by reversing each input task to {priority, taskId, userId} and inserting.
// 4. add(): Insert the task details into both the multiset and the map.
// 5. edit(): Find the existing task entry from the map, erase the old task from the multiset, insert the new task 
//    with the updated priority into the multiset, and update the map.
// 6. rmv(): Retrieve the task details from the map, find and erase the corresponding element in the multiset.
// 7. execTop(): Retrieve the last element in the multiset (highest priority). Erase it from both the multiset and 
//    return the userId. Return -1 if empty.

// Time Complexity:
// - TaskManager(): O(N log N) to insert N tasks into the multiset.
// - add(): O(log T) to insert into the multiset.
// - edit(): O(log T) to find and erase the old task, and insert the new task.
// - rmv(): O(log T) to find and erase the task from the multiset.
// - execTop(): O(log T) to erase the task from the multiset.

// Space Complexity:
// - O(T) to store task details in the multiset and the hash map.

namespace Better {
class TaskManager {
public:
    // Multiset of vectors to automatically keep active tasks sorted lexicographically by {priority, taskId, userId}.
    multiset<vector<int>> assignedTasks;
    // Map of taskId to {userId, priority} to perform O(1) lookups for editing and removing tasks.
    unordered_map<int,pair<int,int>>taskToUserPriority;

    TaskManager(vector<vector<int>>& tasks) {
        // Initialize the manager by inserting each given task into the sorted multiset and the lookup map.
        for (auto t : tasks) {
            // Copy the current task vector t = {userId, taskId, priority}
            vector<int> currTask = t;
            // Reverse from {userId, taskId, priority} to {priority, taskId, userId} to enforce priority-based sorting.
            reverse(begin(currTask), end(currTask));
            // Insert into the multiset which maintains sorted order in O(log N) time.
            assignedTasks.insert(currTask);
            // Store the taskId mapping to its userId and priority for O(1) lookup.
            taskToUserPriority[t[1]]={t[0],t[2]};
        }
    }

    void add(int userId, int taskId, int priority) {
        // Insert the task into the sorted multiset to keep track of its relative priority.
        assignedTasks.insert({priority, taskId, userId});
        // Register the task in the map to enable quick retrieval during updates and deletions.
        taskToUserPriority[taskId] = { userId, priority };
    }

    void edit(int taskId, int newPriority) {
        // Look up current userId and priority from the map to construct the old task representation.
        auto p = taskToUserPriority[taskId];
        // Construct the vector representing the old task in the multiset: {priority, taskId, userId}.
        vector<int> oldTaskEntry = {p.second, taskId, p.first};
        // Find and erase the old entry from the multiset to prevent stale data.
        assignedTasks.erase(assignedTasks.find(oldTaskEntry));
        // Insert the new task state into the multiset to maintain correct priority sorting.
        assignedTasks.insert({newPriority, taskId, p.first});
        // Update the map with the new priority value.
        taskToUserPriority[taskId] = {p.first, newPriority};
    }

    void rmv(int taskId) {
        // Retrieve current task details from the map to construct its multiset entry.
        auto p = taskToUserPriority[taskId];
        // Form the {priority, taskId, userId} entry for the task to be deleted.
        vector<int> taskToBeRemoved = {p.second, taskId, p.first};
        // Find and remove the task entry from the multiset.
        assignedTasks.erase(assignedTasks.find(taskToBeRemoved));
    }

    int execTop() {
        // If there are no tasks, return -1 as there is nothing to execute.
        if(assignedTasks.empty())return -1;
        // Since multiset is sorted ascending, rbegin() yields the task with the highest priority and taskId.
        auto topPriorityTask = *assignedTasks.rbegin();
        // Extract the userId from the executed task (stored at index 2).
        int userId=topPriorityTask[2];
        // Erase the executed task from the multiset using its exact iterator to avoid deleting duplicates.
        assignedTasks.erase(assignedTasks.find(topPriorityTask));
        // Return the userId of the executed task.
        return userId;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a max-heap (priority_queue) for fast retrieval of the highest priority task 
// combined with lazy deletion. Instead of immediately erasing tasks from the heap during edit or rmv operations, 
// we only update the hash map. When execTop is called, we lazily discard stale task entries from the top of 
// the heap by comparing their details with the current valid task details in the hash map.
// This is optimal because priority_queue is a contiguous array-based binary heap, which has much better cache 
// locality and lower constant factors than a node-based std::multiset. Furthermore, edit and rmv operations 
// avoid the costly O(log T) search and deletion from a balanced BST.

// Algorithm:
// 1. Declare a priority_queue of vector<int> to store tasks in the format {priority, taskId, userId}.
// 2. Declare an unordered_map to map taskId to {userId, priority}.
// 3. TaskManager(): Populate both containers by inserting task elements.
// 4. add(): Push task details {priority, taskId, userId} to the heap and store details in the map.
// 5. edit(): Look up the taskId's userId from the map, update the priority in the map, and push the new 
//    {newPriority, taskId, userId} state into the heap.
// 6. rmv(): Remove the taskId from the map.
// 7. execTop(): While the heap is not empty, check the top element. If its taskId is not present in the map 
//    or if its priority/userId do not match the current values in the map, pop the element as it is stale.
//    Once a valid element is found, erase it from the map, pop it from the heap, and return its userId.
//    If the heap becomes empty, return -1.

// Time Complexity:
// - TaskManager(): O(N log N) to insert initial tasks.
// - add(): O(log M) to push onto the heap, where M is the total number of heap operations.
// - edit(): O(log M) to push the new state onto the heap.
// - rmv(): O(1) average time to erase from the hash map.
// - execTop(): O(log M) amortized time to pop stale entries and retrieve the top task.

// Space Complexity:
// - O(M) where M is the total number of tasks and operations stored in the heap and the map.

namespace Optimal {
class TaskManager {
public:
    // Max-heap to store tasks in the format {priority, taskId, userId} for O(1) access to the top priority task.
    priority_queue<vector<int>> pq;
    // Map to keep track of active tasks and their current priority/userId, enabling O(1) updates and lazy deletion checks.
    unordered_map<int, pair<int, int>> taskToUserPriority;

    TaskManager(vector<vector<int>>& tasks) {
        // Populate the heap and map with the initial set of tasks.
        for (auto t : tasks) {
            // Push {priority, taskId, userId} to heap to order by priority (and taskId on tie).
            pq.push({t[2], t[1], t[0]});
            // Record the taskId to {userId, priority} mapping for lookup.
            taskToUserPriority[t[1]] = {t[0], t[2]};
        }
    }

    void add(int userId, int taskId, int priority) {
        // Push the new task onto the max-heap.
        pq.push({priority, taskId, userId});
        // Register the task in our map to track its current state.
        taskToUserPriority[taskId] = {userId, priority};
    }

    void edit(int taskId, int newPriority) {
        // Retrieve the userId associated with the taskId (stored in the map).
        int userId = taskToUserPriority[taskId].first;
        // Update the priority in the map to the new value.
        taskToUserPriority[taskId].second = newPriority;
        // Push the new priority state of the task to the heap; the old heap entry will be lazily bypassed.
        pq.push({newPriority, taskId, userId});
    }

    void rmv(int taskId) {
        // Erase the task from the map, marking it as removed from the system.
        taskToUserPriority.erase(taskId);
    }

    int execTop() {
        // Process the max-heap to find the highest priority valid task.
        while (!pq.empty()) {
            // Access the task at the top of the heap.
            auto topTask = pq.top();
            int priority = topTask[0];
            int taskId = topTask[1];
            int userId = topTask[2];

            // Look up the taskId in the map of active tasks.
            auto it = taskToUserPriority.find(taskId);
            // Check if the task is still active and matches the current priority/userId in the map.
            if (it != taskToUserPriority.end() && it->second.first == userId && it->second.second == priority) {
                // Remove the executed task from the map of active tasks.
                taskToUserPriority.erase(it);
                // Pop the executed task from the heap.
                pq.pop();
                // Return the userId of the executed task.
                return userId;
            }
            // If the entry is stale (mismatched priority or deleted), pop it from the heap and continue.
            pq.pop();
        }
        // Return -1 if all tasks in the heap were stale/deleted or if the heap is empty.
        return -1;
    }
};
}

/**
 * Your TaskManager object will be instantiated and called as such:
 * TaskManager* obj = new TaskManager(tasks);
 * obj->add(userId,taskId,priority);
 * obj->edit(taskId,newPriority);
 * obj->rmv(taskId);
 * int param_4 = obj->execTop();
 */