class LRUCache {
public:
    list<int> dll;

    unordered_map<int, pair<int, list<int>::iterator>> keyToValAddress;

    int n;

    LRUCache(int capacity) { n = capacity; }

    int get(int key) {
        if (!keyToValAddress.count(key))
            return -1;

        int value = keyToValAddress[key].first;

        dll.erase(keyToValAddress[key].second);
        dll.push_front(key);
        keyToValAddress[key].second = dll.begin();

        return value;
    }

    void put(int key, int value) {
        if (keyToValAddress.count(key)) {
            dll.erase(keyToValAddress[key].second);
        } else if (keyToValAddress.size() == n) {
            int oldKey = dll.back();
            dll.pop_back();
            keyToValAddress.erase(oldKey);
        }

        dll.push_front(key);
        keyToValAddress[key] = {value, dll.begin()};
    }
};
/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */