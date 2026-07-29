// ===============================
// Brute Force Approach
// ===============================

// Approach:
// We calculate the time needed for each car to reach the target. To determine fleet merges, we need to process cars from closest to furthest from the target. In this brute force method, we sort the cars by their starting positions in descending order using an O(N^2) Selection Sort. Then, we iterate through the sorted cars and count how many times the arrival time of a trailing car exceeds the arrival time of the current fleet leader ahead.

// Algorithm:
// 1. Store the starting position and calculated time to target for each car.
// 2. Sort the cars in descending order of their starting position using Selection Sort.
// 3. Initialize the fleet count to 1 and set the initial fleet leader's arrival time to the first car's time.
// 4. Iterate through the remaining cars; if a car takes longer than the fleet leader, increment the fleet count and update the fleet leader's time.
// 5. Return the total count of fleets.

// Time Complexity:
// O(N^2)

// Space Complexity:
// O(N)

namespace BruteForce {
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        // Store the total number of cars to manage loops and check bounds
        int n = position.size();
        // Return 0 if there are no cars, as they cannot form any fleets
        if (n == 0) return 0;

        // Vector of pairs to associate starting positions with arrival times to target
        vector<pair<int, double>> posAndTime;

        // Loop through all cars to calculate the time required to reach the target
        for (int i = 0; i < n; i++) {
            // Compute arrival time using floating-point arithmetic to prevent precision loss
            double time = (double)(target - position[i]) / speed[i];
            // Pair each car's position with its time to target for sorting
            posAndTime.push_back({position[i], time});
        }

        // Selection sort to sort cars by starting position in descending order
        for (int i = 0; i < n - 1; i++) {
            // Assume the current index i has the largest starting position in the unsorted portion
            int maxIdx = i;
            // Inner loop to find the actual maximum starting position in the remaining part
            for (int j = i + 1; j < n; j++) {
                // Compare starting positions to sort in descending order (closest to target first)
                if (posAndTime[j].first > posAndTime[maxIdx].first) {
                    // Update the index of the largest starting position found so far
                    maxIdx = j;
                }
            }
            // Swap the elements to place the next closest car to the target in its sorted position
            swap(posAndTime[i], posAndTime[maxIdx]);
        }

        // Start with 1 fleet since the car closest to the target always forms a fleet
        int count = 1;
        // Track the arrival time of the current fleet leader to evaluate trailing cars
        double fleetTime = posAndTime[0].second;

        // Scan the rest of the cars from closest to furthest starting position
        for (int i = 1; i < n; i++) {
            // If the trailing car takes longer than the fleet leader, a new fleet is formed
            if (posAndTime[i].second > fleetTime) {
                // Increment the fleet count as this car cannot catch up to the one in front
                count++;
                // Update fleetTime to this car's arrival time, making it the new fleet leader
                fleetTime = posAndTime[i].second;
            }
        }

        // Return the final count of fleets arriving at the target
        return count;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We improve the time complexity to O(N log N) by using standard library sort to order the cars. We use a monotonic stack to explicitly manage the arrival times of the fleets. As we process cars from closest to furthest, a trailing car can only form a new fleet if its arrival time is strictly greater than the top of the stack (the current slowest fleet leader). Otherwise, it merges into the existing fleet. The final size of the stack corresponds to the total number of fleets.

// Algorithm:
// 1. Calculate each car's arrival time to target and store it alongside its position.
// 2. Sort the cars in descending order of starting positions using std::sort.
// 3. Initialize a stack to store arrival times of the fleet leaders.
// 4. For each car, if the stack is empty or its arrival time is greater than the stack's top, push the time onto the stack.
// 5. Return the size of the stack as the fleet count.

// Time Complexity:
// O(N log N)

// Space Complexity:
// O(N)

namespace Better {
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        // Store the size of the input arrays to determine boundary conditions
        int n = position.size();
        // If there are no cars, they cannot form any fleets, so return 0
        if (n == 0) return 0;

        // Container to hold the position and time pairs for sorting
        vector<pair<int, double>> posAndTime;

        // Iterate through all cars to calculate their arrival times at the target
        for (int i = 0; i < n; i++) {
            // Compute arrival time using floating-point division to avoid truncation
            double time = (double)(target - position[i]) / speed[i];
            // Associate each car's starting position with its travel time
            posAndTime.push_back({position[i], time});
        }

        // Sort cars by starting position descending to process from closest to furthest
        sort(posAndTime.begin(), posAndTime.end(), greater<>());

        // Stack to keep track of the bottleneck times of active fleets
        stack<double> fleetTimes;

        // Process each sorted car sequentially
        for (int i = 0; i < n; i++) {
            // Retrieve the time to target for the current car
            double time = posAndTime[i].second;
            // If stack is empty or the car is slower than the fleet ahead, it forms a new fleet
            if (fleetTimes.empty() || time > fleetTimes.top()) {
                // Push the new fleet's bottleneck time onto the stack
                fleetTimes.push(time);
            }
        }

        // Return the total number of distinct fleets stored in the stack
        return fleetTimes.size();
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach reduces the auxiliary space complexity from the O(N) stack in the Better approach to O(1) by only tracking the largest arrival time seen so far. Since a car can only merge with the fleet directly in front of it (which is already sorted ahead of it), we only need to maintain a single tracking variable `fleetTime` representing the arrival time of the current fleet leader. This avoids stack allocations while preserving the O(N log N) sorting logic.

// Algorithm:
// 1. Calculate each car's arrival time to target and store it alongside its position.
// 2. Sort the cars in descending order of starting positions using std::sort.
// 3. Track the number of fleets (initially 1) and the current fleet leader's arrival time.
// 4. Iterate from the second closest car to the furthest: if its time is greater than the tracked fleet time, increment the fleet count and update the tracked fleet time.
// 5. Return the total count of fleets.

// Time Complexity:
// O(N log N)

// Space Complexity:
// O(N)

namespace Optimal {
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        // Store the total number of cars to manage loops and check bounds
        int n = position.size();
        // Return 0 if there are no cars, as they cannot form any fleets
        if (n == 0) return 0;

        // Vector of pairs to associate starting positions with arrival times to target
        vector<pair<int, double>> posAndTime;

        // Loop through all cars to calculate the time required to reach the target
        for (int i = 0; i < n; i++) {
            // Compute arrival time using floating-point arithmetic to prevent precision loss
            double time = (double)(target - position[i]) / speed[i];
            // Pair each car's position with its time to target for sorting
            posAndTime.push_back({position[i], time});
        }

        // Sort cars in descending order of starting positions using standard sort
        sort(posAndTime.begin(), posAndTime.end(), greater<>());

        // Start with 1 fleet since the car closest to the target always forms a fleet
        int count = 1;
        // Track the arrival time of the current fleet leader to evaluate trailing cars
        double fleetTime = posAndTime[0].second;

        // Scan the rest of the cars from closest to furthest starting position
        for (int i = 1; i < n; i++) {
            // If the trailing car takes longer than the fleet leader, a new fleet is formed
            if (posAndTime[i].second > fleetTime) {
                // Increment the fleet count as this car cannot catch up to the one in front
                count++;
                // Update fleetTime to this car's arrival time, making it the new fleet leader
                fleetTime = posAndTime[i].second;
            }
        }

        // Return the final count of fleets arriving at the target
        return count;
    }
};
}