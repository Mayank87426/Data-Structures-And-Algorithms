-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- We can solve this by querying the Users table and using a correlated subquery for each user.
-- For each user in the outer query, the subquery scans the Orders table to filter and count 
-- the orders that belong to that user and were placed in the year 2019.

-- Algorithm:
-- 1. Select the user_id (aliased as buyer_id) and join_date from the Users table.
-- 2. For each user, perform a subquery on the Orders table.
-- 3. Filter the orders where buyer_id matches the user's user_id and the order_date falls in 2019.
-- 4. Count the number of such orders using COUNT() and return it as orders_in_2019.

-- Time Complexity:
-- O(N * M) where N is the number of users and M is the number of orders. For each of the N users, 
-- we scan all M orders in the subquery if there is no index on buyer_id.

-- Space Complexity:
-- O(N) to store the final output records.

SELECT
    -- Select the user ID and alias it as buyer_id as required by the output schema
    U.user_id AS buyer_id,
    -- Select the join_date to show when the user joined the platform
    U.join_date,
    -- Count the 2019 orders for the current user using a correlated subquery
    (
        SELECT COUNT(O.order_id)
        FROM Orders O
        -- Correlate the subquery with the outer user record to count only their orders
        WHERE O.buyer_id = U.user_id 
          -- Filter for orders placed in the year 2019
          AND YEAR(O.order_date) = 2019
    ) AS orders_in_2019
-- Query from the main Users table to ensure every user is included
FROM Users U;

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- This approach uses a LEFT JOIN to combine the Users and Orders tables, and then uses GROUP BY 
-- with a conditional aggregation (SUM with CASE WHEN) to count the orders in 2019.
-- By grouping by user_id and join_date, we collapse the joined rows back into one row per user.

-- Algorithm:
-- 1. Perform a LEFT JOIN from Users to Orders on user_id = buyer_id.
-- 2. Group the combined rows by user_id and join_date.
-- 3. For each group, check if the order_date year is 2019. If yes, evaluate to 1, otherwise 0.
-- 4. Sum these values to get the total number of orders in 2019 for each user.

-- Time Complexity:
-- O(N + M log(N + M)) where N is the number of users and M is the total number of orders. 
-- We join the entire Users table and Orders table first, which can produce a large intermediate 
-- table, and then we group the result.

-- Space Complexity:
-- O(N + M) to store the intermediate joined representation before grouping.

SELECT
    -- Select the user's ID as buyer_id to match the desired result format
    U.user_id AS buyer_id,
    -- Retrieve the join date of each user
    U.join_date,
    -- Use a conditional sum to count only the orders that occurred in the year 2019
    SUM(CASE WHEN YEAR(O.order_date) = 2019 THEN 1 ELSE 0 END) AS orders_in_2019
-- Start with the Users table to ensure we do not omit users with zero orders
FROM Users U
-- Perform a left join so that users without matching orders are still kept in the result
LEFT JOIN Orders O
-- Match users to orders by linking user_id with buyer_id
ON U.user_id = O.buyer_id
-- Group by user_id and join_date to aggregate the order count for each unique user
GROUP BY U.user_id, U.join_date;

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- The optimal approach aggregates the 2019 orders in a subquery BEFORE performing the LEFT JOIN.
-- We filter the Orders table using a sargable range query (order_date >= '2019-01-01' AND order_date <= '2019-12-31') 
-- which can utilize an index on order_date. We then group and count the orders per buyer in this subquery, 
-- and finally LEFT JOIN the Users table with this small, pre-aggregated subquery.

-- Algorithm:
-- 1. In a subquery, filter the Orders table for dates in 2019 using range conditions.
-- 2. Group by buyer_id and count the order_id for each buyer.
-- 3. In the outer query, LEFT JOIN Users with this subquery on user_id = buyer_id.
-- 4. Use COALESCE(orders_in_2019, 0) to replace NULLs (for users with no 2019 orders) with 0.

-- Time Complexity:
-- O(M_2019 log(M_2019) + N) where N is the number of users, and M_2019 is the number of orders in 2019.
-- The range filter on Orders can utilize an index on order_date, scanning only 2019 records.
-- The subquery aggregates only the 2019 orders, reducing the join size significantly.

-- Space Complexity:
-- O(U_2019) where U_2019 is the number of unique buyers who made an order in 2019. 
-- This stores the pre-aggregated results for the join.

SELECT 
    -- Select the user ID as buyer_id to match the output schema requirements
    U.user_id AS buyer_id,
    -- Retrieve the join date for each user
    U.join_date,
    -- Default the count to 0 if a user has no orders in 2019 (handling nulls from LEFT JOIN)
    COALESCE(O.orders_in_2019, 0) AS orders_in_2019
-- Start with the Users table to guarantee all users are present in the final output
FROM Users U
-- Left join with the pre-aggregated 2019 orders to avoid processing unneeded years or rows
LEFT JOIN (
    SELECT 
        -- Retrieve the buyer ID for grouping and joining
        buyer_id, 
        -- Count the total number of orders placed by this buyer in 2019
        COUNT(order_id) AS orders_in_2019
    -- Source from the Orders table to count the purchases
    FROM Orders
    -- Use a sargable range query to filter 2019 orders, allowing the use of indexes on order_date
    WHERE order_date >= '2019-01-01' AND order_date <= '2019-12-31'
    -- Group by buyer_id to calculate the count per individual buyer
    GROUP BY buyer_id
) O
-- Join on the buyer's identifier to map the aggregated counts to their user profiles
ON U.user_id = O.buyer_id;