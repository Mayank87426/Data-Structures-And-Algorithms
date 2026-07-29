// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach simulates the collision process step-by-step by repeatedly scanning the array 
// from left to right. In each pass, we look for adjacent pairs where the left asteroid is moving right (positive) 
// and the right asteroid is moving left (negative). When we find such a pair, we resolve their collision according 
// to the rules (larger size survives, or both destroy if equal size) and build a new state. We repeat this process 
// until a full pass completes with zero collisions, indicating the state is stable.

// Algorithm:
// 1. Initialize a flag `collisionOccurred` to `true`.
// 2. Loop while `collisionOccurred` is true:
//    - Set `collisionOccurred = false`.
//    - Create a temporary vector `nextState`.
//    - Traverse the current `asteroids` list:
//      - If the current asteroid at index `i` is positive and `i+1` is valid and negative, a collision occurs:
//        - Set `collisionOccurred = true`.
//        - If `abs(asteroids[i]) > abs(asteroids[i+1])`, push `asteroids[i]` to `nextState`.
//        - Else if `abs(asteroids[i]) < abs(asteroids[i+1])`, push `asteroids[i+1]` to `nextState`.
//        - If equal, do not push either (both explode).
//        - Increment index `i` to skip the collided partner.
//      - Otherwise, push `asteroids[i]` to `nextState`.
//    - Set `asteroids = nextState`.
// 3. Return `asteroids`.

// Time Complexity:
// O(N^2) in the worst case, where N is the number of asteroids. Each pass can resolve at least one collision, 
// leading to at most O(N) passes, and each pass takes O(N) time to copy and build the next state.

// Space Complexity:
// O(N) extra space to store the state of the asteroids during each simulation pass.

namespace BruteForce {
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        // Flag to track if any collision occurred during a pass, determining if further passes are needed.
        bool collisionOccurred = true;
        
        // Continue simulation passes as long as at least one collision happens in the previous pass.
        while (collisionOccurred) {
            // Reset the flag to false before starting the current simulation pass.
            collisionOccurred = false;
            
            // Temporary container to accumulate the state of asteroids surviving this pass.
            vector<int> nextState;
            int n = asteroids.size();
            
            // Traverse the current list of asteroids to resolve any adjacent collisions.
            for (int i = 0; i < n; i++) {
                // A collision occurs only when a right-moving asteroid (> 0) is followed by a left-moving one (< 0).
                if (i < n - 1 && asteroids[i] > 0 && asteroids[i + 1] < 0) {
                    // Mark that a collision is detected, meaning the state is not yet stable.
                    collisionOccurred = true;
                    
                    // Compare the absolute sizes to determine which asteroid survives the collision.
                    if (abs(asteroids[i]) > abs(asteroids[i + 1])) {
                        // The right-moving asteroid is larger and survives, so we add it to the next state.
                        nextState.push_back(asteroids[i]);
                    } else if (abs(asteroids[i]) < abs(asteroids[i + 1])) {
                        // The left-moving asteroid is larger and survives, so we add it to the next state.
                        nextState.push_back(asteroids[i + 1]);
                    }
                    // If sizes are equal, both destroy each other and neither is added to the next state.
                    
                    // Increment index to bypass the next asteroid since it was resolved in the collision.
                    i++;
                } else {
                    // If no collision condition is met, the current asteroid remains unaffected in this pass.
                    nextState.push_back(asteroids[i]);
                }
            }
            // Update the list of asteroids to reflect the state after resolving this round of collisions.
            asteroids = nextState;
        }
        // Return the stable configuration where no further collisions are possible.
        return asteroids;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of scanning the entire array repeatedly, we can process the asteroids from left to right using a stack 
// to keep track of the stable asteroids. If the current asteroid is moving right, it will not collide with any 
// previous ones, so we push it onto the stack. If it is moving left, it can collide with the right-moving asteroids 
// on the top of the stack. We pop all smaller right-moving asteroids until we encounter a larger one or one moving 
// left, resolving the collisions.

// Algorithm:
// 1. Initialize a stack `st`.
// 2. For each asteroid `x` in the input:
//    - If `x > 0`, push `x` to `st`.
//    - If `x < 0`:
//      - If stack is not empty and `st.top() < 0` (moving left), push `x` and continue.
//      - Loop while stack is not empty, top is positive, and top's size is smaller than `abs(x)`: pop from `st`.
//      - If stack becomes empty or top is negative, push `x` and continue.
//      - If top is positive and equal to `abs(x)`, pop from `st` and continue (both explode).
//      - Otherwise (top is positive and larger than `abs(x)`), do nothing (`x` explodes).
// 3. Populate a vector from stack elements, reverse it, and return.

// Time Complexity:
// O(N) since each asteroid is pushed onto and popped from the stack at most once.

// Space Complexity:
// O(N) auxiliary space for the stack to store the surviving asteroids.

namespace Better {
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        // Use a stack to track asteroids that have survived all collisions so far.
        stack<int> st;
        
        // Process each asteroid one by one to determine its survival or collisions.
        for (auto& x : asteroids) {
            // Right-moving asteroids (+ve) cannot collide with previous ones as they move away from them.
            if (x > 0) {
                st.push(x);
            } else {
                // If the stack top is already moving left (-ve), they move in the same direction and won't collide.
                if (!st.empty() && st.top() < 0) {
                    st.push(x);
                    continue;
                }
                
                // Resolve collisions where previous right-moving asteroids are smaller than the current left-moving one.
                while (!st.empty() && st.top() > 0 && abs(st.top()) < abs(x)) {
                    // The smaller right-moving asteroid on top of the stack is destroyed.
                    st.pop();
                }
                
                // If stack is empty or top is moving left, the current left-moving asteroid has no more obstacles.
                if (st.empty() || st.top()<0){
                    st.push(x);
                    continue;
                }
                
                // If the current left-moving asteroid meets a right-moving one of equal size, both explode.
                if (!st.empty() && abs(st.top()) == abs(x)) {
                    // Pop the right-moving asteroid to destroy it, and skip pushing the current asteroid.
                    st.pop();
                    continue;
                }
            }
        }
        
        // Collect surviving asteroids from the stack into a vector.
        vector<int> remainingAsteroids;
        while (!st.empty()) {
            // Push the top element of the stack to the vector.
            remainingAsteroids.push_back(st.top());
            // Remove the processed element to access the next one.
            st.pop();
        }
        
        // Reverse the vector to restore the original left-to-right ordering of the asteroids.
        reverse(begin(remainingAsteroids), end(remainingAsteroids));
        
        // Return the final list of surviving asteroids.
        return remainingAsteroids;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach optimizes the space complexity of the stack-based simulation to O(1) auxiliary space. 
// We use the input `asteroids` vector itself as a stack, maintaining a `write_ptr` that points to the boundary 
// of the processed elements. We perform all push and pop operations in-place by overwriting elements at `write_ptr` 
// and incrementing/decrementing it.

// Algorithm:
// 1. Initialize `write_ptr = 0`.
// 2. Iterate through each asteroid `curr` in `asteroids`:
//    - If `curr > 0`, write it at `write_ptr` and increment `write_ptr`.
//    - If `curr < 0`:
//      - Loop while `write_ptr > 0`, the asteroid at `write_ptr - 1` is positive and smaller than `abs(curr)`: decrement `write_ptr` (popping the smaller right-moving asteroid).
//      - If `write_ptr > 0` and the asteroid at `write_ptr - 1` is equal to `-curr`, decrement `write_ptr` (both explode).
//      - Else if `write_ptr == 0` or the asteroid at `write_ptr - 1` is negative, write `curr` at `write_ptr` and increment `write_ptr`.
//      - Otherwise, `curr` is destroyed (do nothing).
// 3. Resize the vector to `write_ptr` to remove destroyed elements.
// 4. Return the modified `asteroids` vector.

// Time Complexity:
// O(N) since we iterate through the array once and each position is visited/written at most a constant number of times.

// Space Complexity:
// O(1) auxiliary space (excluding the space for the returned vector, since we modify the input in-place).

namespace Optimal {
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        // write_ptr acts as the top of our in-place stack, tracking where the next survivor is placed.
        int write_ptr = 0;
        
        // Iterate through each asteroid to process potential collisions in-place.
        for (int i = 0; i < asteroids.size(); i++) {
            int curr = asteroids[i];
            
            // Right-moving asteroids (+ve) are safe from colliding with elements to their left.
            if (curr > 0) {
                // Place the right-moving asteroid at the current write position and advance the pointer.
                asteroids[write_ptr++] = curr;
            } else {
                // Pop all smaller right-moving asteroids that collide with the current left-moving one.
                while (write_ptr > 0 && asteroids[write_ptr - 1] > 0 && asteroids[write_ptr - 1] < -curr) {
                    // Decrement the pointer to destroy the right-moving asteroid.
                    write_ptr--;
                }
                
                // If the closest surviving asteroid is right-moving and of equal size, both destroy each other.
                if (write_ptr > 0 && asteroids[write_ptr - 1] == -curr) {
                    // Decrement the pointer to destroy the right-moving asteroid, and skip placing the current one.
                    write_ptr--;
                } 
                // If no right-moving asteroid remains to block it, the current left-moving asteroid survives.
                else if (write_ptr == 0 || asteroids[write_ptr - 1] < 0) {
                    // Place the left-moving asteroid at the current write position and advance the pointer.
                    asteroids[write_ptr++] = curr;
                }
                // If the surviving right-moving asteroid is larger than the current one, the current one explodes.
            }
        }
        
        // Truncate the vector to retain only the surviving asteroids up to write_ptr.
        asteroids.resize(write_ptr);
        
        // Return the in-place modified vector containing the survivors.
        return asteroids;
    }
};
}