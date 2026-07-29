// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach implements the LRU Cache using a simple dynamic array (std::vector) 
// of key-value pairs. Since we do not use hash maps or doubly linked lists, we must search 
// through the vector linearly to find or update any element. 
// When an element is accessed via get() or put(), it is removed from its current position 
// and pushed to the back of the vector to mark it as the most recently used (MRU). 
// If the cache exceeds capacity, the element at the front of the vector (which is the oldest 
// or least recently used) is evicted.

// Algorithm:
// 1. Initialize the capacity cap.
// 2. get(key):
//    - Linearly scan the cache vector.
//    - If the key is found, retrieve its value, remove it from its position, append it to the end of the vector, and return the value.
//    - If not found, return -1.
// 3. put(key, value):
//    - Linearly scan the cache vector.
//    - If the key is found, update its value, remove it from its current position, and append it to the end.
//    - If the key is not found, check if the cache is full (cache.size() == cap).
//      - If full, evict the first element of the vector (least recently used).
//    - Append the new key-value pair to the end of the vector.

// Time Complexity:
// - get(key): O(N) per operation, where N is the current size of the cache, due to linear scan and element deletion/shifting.
// - put(key, value): O(N) per operation, due to linear scan and element deletion/shifting.

// Space Complexity:
// O(N) to store up to capacity elements in the vector.

namespace BruteForce {
class LRUCache {
private:
    // A dynamic array of pairs to store the key-value entries in cache
    vector<pair<int, int>> cache;
    // Capacity of the cache
    int cap;

public:
    // Constructor to initialize cache capacity
    LRUCache(int capacity) {
        cap = capacity;
    }

    // Retrieve value for a key, moving it to the back to represent it as recently used
    int get(int key) {
        // Linearly scan the cache to locate the key
        for (int i = 0; i < cache.size(); ++i) {
            // If key is found in cache
            if (cache[i].first == key) {
                // Store the value to return later
                int value = cache[i].second;
                // Keep a temporary copy of the key-value pair
                pair<int, int> temp = cache[i];
                // Remove the element from its current position (requires shifting elements)
                cache.erase(cache.begin() + i);
                // Push it to the back of the vector to mark it as the most recently used
                cache.push_back(temp);
                // Return the found value
                return value;
            }
        }
        // Return -1 if key is not found in the cache
        return -1;
    }

    // Insert or update key-value pair
    void put(int key, int value) {
        // Linearly scan the cache to see if key already exists
        for (int i = 0; i < cache.size(); ++i) {
            // If key is already present in cache
            if (cache[i].first == key) {
                // Erase the old occurrence of this key
                cache.erase(cache.begin() + i);
                // Insert the updated key-value pair at the back (most recently used)
                cache.push_back({key, value});
                // Return early since update is complete
                return;
            }
        }

        // If cache capacity is reached, evict the least recently used element
        if (cache.size() == cap) {
            // Erase the first element of the vector, which is the least recently used
            cache.erase(cache.begin());
        }

        // Insert the new key-value pair at the back of the vector
        cache.push_back({key, value});
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The better approach reduces the time complexity of lookup to O(1) and eviction/update to O(log N) 
// by using a combination of hash maps and a balanced binary search tree (std::map).
// We use a timestamp counter to keep track of the logical time each operation is performed.
// - keyToVal: maps key -> value to allow O(1) retrieval.
// - keyToTime: maps key -> timestamp of its last access.
// - timeToKey: a sorted map (std::map) that maps timestamp -> key.
// The key with the minimum timestamp in timeToKey represents the least recently used (LRU) element.
// When an element is accessed or updated, its old timestamp is removed from timeToKey and it is assigned 
// a new, higher timestamp.

// Algorithm:
// 1. Initialize capacity cap and time_counter to 0.
// 2. get(key):
//    - Check keyToVal for key existence. If absent, return -1.
//    - Retrieve the old timestamp for key from keyToTime, and erase it from timeToKey.
//    - Assign a new timestamp using time_counter++, update keyToTime[key], and insert the new timestamp-key pair into timeToKey.
//    - Return the value from keyToVal.
// 3. put(key, value):
//    - If the key exists, update its value in keyToVal, and refresh its timestamp in keyToTime and timeToKey.
//    - If key does not exist:
//      - If cache size is at capacity:
//        - Get the minimum timestamp from timeToKey.begin(), and retrieve its corresponding oldestKey.
//        - Erase oldestKey from keyToVal, keyToTime, and timeToKey.
//      - Insert key-value into keyToVal, assign a new timestamp, and update keyToTime and timeToKey.

// Time Complexity:
// - get(key): O(log N) per operation, due to insertion and deletion in the sorted map.
// - put(key, value): O(log N) per operation, due to insertion and deletion in the sorted map.

// Space Complexity:
// O(N) to store capacity elements across the maps.

namespace Better {
class LRUCache {
private:
    // Maximum capacity of the cache
    int cap;
    // Monotonically increasing counter to simulate system timestamps
    int time_counter;
    // Hash map to store key to value for O(1) value retrieval
    unordered_map<int, int> keyToVal;
    // Hash map to associate each key with its last accessed timestamp
    unordered_map<int, int> keyToTime;
    // Balanced binary search tree (std::map) mapping timestamp to key for O(log N) eviction lookup
    map<int, int> timeToKey;

public:
    // Constructor to initialize capacity and timestamp counter
    LRUCache(int capacity) {
        cap = capacity;
        time_counter = 0;
    }

    // Retrieve value for a key, updating its access timestamp
    int get(int key) {
        // If the key does not exist in the hash map, return -1
        if (keyToVal.find(key) == keyToVal.end()) {
            return -1;
        }

        // Retrieve the old timestamp of the key
        int oldTime = keyToTime[key];
        // Remove the old timestamp entry from the sorted map to prevent stale lookups
        timeToKey.erase(oldTime);

        // Generate a new unique timestamp
        int newTime = time_counter++;
        // Update the timestamp mapping for the key
        keyToTime[key] = newTime;
        // Insert the new timestamp and key pair into the sorted map
        timeToKey[newTime] = key;

        // Return the cached value
        return keyToVal[key];
    }

    // Insert or update key-value pair
    void put(int key, int value) {
        // If key already exists in the cache
        if (keyToVal.find(key) != keyToVal.end()) {
            // Update the key's value in the map
            keyToVal[key] = value;
            // Retrieve its old timestamp
            int oldTime = keyToTime[key];
            // Remove the old timestamp from the sorted map
            timeToKey.erase(oldTime);
            
            // Assign a new timestamp to mark it as recently used
            int newTime = time_counter++;
            keyToTime[key] = newTime;
            timeToKey[newTime] = key;
            return;
        }

        // If capacity is reached, evict the least recently used key
        if (keyToVal.size() == cap) {
            // The first element in the sorted map has the smallest timestamp (LRU)
            auto it = timeToKey.begin();
            // Get the key corresponding to this minimum timestamp
            int oldestKey = it->second;
            
            // Remove the evicted key from all three structures
            keyToVal.erase(oldestKey);
            keyToTime.erase(oldestKey);
            timeToKey.erase(it);
        }

        // Insert the new key-value pair
        keyToVal[key] = value;
        // Assign a new timestamp
        int newTime = time_counter++;
        keyToTime[key] = newTime;
        timeToKey[newTime] = key;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach achieves O(1) average time complexity for both get and put operations 
// by using a doubly linked list (std::list) combined with a hash map (std::unordered_map).
// The doubly linked list stores the keys of the cache in order of their usage (most recently used at the front, 
// least recently used at the back).
// The hash map maps each key to a pair containing its value and an iterator pointing to its node 
// in the doubly linked list. This allows us to find any element in O(1) time and splice/erase it 
// from the doubly linked list in O(1) time.

// Algorithm:
// 1. Initialize capacity n.
// 2. get(key):
//    - Check if the key exists in keyToValAddress. If not, return -1.
//    - Retrieve the value, erase the key from its current position in dll, push it to the front of dll, and update the iterator in the map.
//    - Return the retrieved value.
// 3. put(key, value):
//    - If the key already exists, erase it from its current position in dll.
//    - If the key does not exist and the cache is full (keyToValAddress.size() == n):
//      - Evict the least recently used key from the back of dll (dll.back()), and erase its entry from keyToValAddress.
//    - Push the key to the front of dll and map it to {value, dll.begin()} in keyToValAddress.

// Time Complexity:
// - get(key): O(1) average time complexity.
// - put(key, value): O(1) average time complexity.

// Space Complexity:
// O(N) to store capacity elements in the list and hash map.

namespace Optimal {
class LRUCache {
public:
    // Doubly linked list to maintain the order of keys, with most recently used at the front
    list<int> dll;

    // Hash map to store key to pair of value and iterator pointing to its position in dll
    unordered_map<int, pair<int, list<int>::iterator>> keyToValAddress;

    // Cache capacity
    int n;

    // Constructor to initialize the cache capacity
    LRUCache(int capacity) { n = capacity; }

    // Retrieve value for a given key
    int get(int key) {
        // If the key is not present in the cache map, return -1
        if (!keyToValAddress.count(key))
            return -1;

        // Retrieve the stored value from the map
        int value = keyToValAddress[key].first;

        // Erase the key from its current position in the doubly linked list using the stored iterator
        dll.erase(keyToValAddress[key].second);
        // Move the key to the front of the list to mark it as most recently used
        dll.push_front(key);
        // Update the iterator in the map to point to the new front position
        keyToValAddress[key].second = dll.begin();

        // Return the retrieved value
        return value;
    }

    // Insert or update key-value pair in the cache
    void put(int key, int value) {
        // If the key already exists in the cache
        if (keyToValAddress.count(key)) {
            // Remove the old position of the key from the doubly linked list
            dll.erase(keyToValAddress[key].second);
        } else if (keyToValAddress.size() == n) {
            // If cache capacity is reached, retrieve the least recently used key from the back of the list
            int oldKey = dll.back();
            // Evict the least recently used key from the list
            dll.pop_back();
            // Erase the evicted key's mapping from the hash map
            keyToValAddress.erase(oldKey);
        }

        // Insert the key at the front of the doubly linked list
        dll.push_front(key);
        // Store the value and the new list iterator in the hash map
        keyToValAddress[key] = {value, dll.begin()};
    }
};
}

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */