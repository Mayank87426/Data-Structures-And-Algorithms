class LRUCache {
public:
    int n;
    int timer = 0;

    set<pair<int, int>> st;            
    unordered_map<int, int> val;        
    unordered_map<int, int> last;       

    LRUCache(int capacity) {
        n = capacity;
    }

    int get(int key) {
        if (!val.count(key))
            return -1;

        st.erase({last[key], key});

        timer++;
        last[key] = timer;
        st.insert({timer, key});

        return val[key];
    }

    void put(int key, int value) {
        timer++;

        if (val.count(key)) {
            st.erase({last[key], key});
        } else if ((int)val.size() == n) {
            auto it = st.begin();
            int oldKey = it->second;

            st.erase(it);
            val.erase(oldKey);
            last.erase(oldKey);
        }

        val[key] = value;
        last[key] = timer;
        st.insert({timer, key});
    }
};
/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */