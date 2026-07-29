-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- We can use a NOT IN subquery to find all customer IDs that do not exist in the Orders table.
-- For each customer, we check if their ID is absent from the subquery's result set of ordered customer IDs.
-- Note that we must filter out NULLs in the subquery because if the subquery returns any NULL,
-- the NOT IN operator will always evaluate to false or NULL, returning no rows at all.

-- Algorithm:
-- 1. Select the name of the customer and alias it as 'Customers'.
-- 2. Use a WHERE clause to check that the customer's id is NOT IN the subquery.
-- 3. In the subquery, select customerId from Orders and filter out NULLs to ensure correctness.

-- Time Complexity:
-- O(N * M) in the worst case without indexes, as it may perform a full table scan of Orders for each customer. With indexes, this can be optimized to O(N log M).

-- Space Complexity:
-- O(M) to store the customer IDs from the Orders table in a hash set or temporary table during execution.

SELECT name AS Customers -- Select the name column and rename it to 'Customers' for the output.
FROM Customers -- Query from the Customers table to check all customers.
WHERE id NOT IN ( -- Filter for customers whose ID is not present in the set of ordering customers.
    SELECT customerId -- Select customerId from the Orders table.
    FROM Orders -- Scan the Orders table.
    WHERE customerId IS NOT NULL -- Exclude NULL customer IDs to prevent the NOT IN query from returning empty results.
);

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- We perform a LEFT JOIN between the Customers table and the Orders table on the customer ID.
-- For customers who have never placed an order, the LEFT JOIN will still include them in the result set,
-- but all columns from the Orders table (such as customerId) will be NULL.
-- We then filter the rows to keep only those where the Orders.customerId is NULL.

-- Algorithm:
-- 1. Left join Customers C with Orders O on C.id = O.customerId.
-- 2. Select C.name and alias it as 'Customers'.
-- 3. Use WHERE O.customerId IS NULL to filter out customers who have placed at least one order.

-- Time Complexity:
-- O(N + M) using a hash join or merge join, where N is the number of customers and M is the number of orders.

-- Space Complexity:
-- O(N + M) or O(1) auxiliary space, depending on whether the optimizer builds temporary join structures.

SELECT C.name AS Customers -- Select the customer name and alias it as 'Customers' for output.
FROM Customers AS C -- Scan the Customers table, aliasing it as C.
LEFT JOIN Orders AS O -- Left join with the Orders table, aliasing it as O, to keep all customers.
ON C.id = O.customerId  -- Match customers to their orders by matching C.id with O.customerId.
WHERE O.customerId   IS NULL; -- Filter for rows where customerId is NULL, indicating they never ordered.

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- We use a NOT EXISTS correlated subquery to check for each customer if they do not have any matching records in the Orders table.
-- The NOT EXISTS clause is generally optimal in SQL because it can short-circuit: the database engine stops scanning the Orders table for a specific customer as soon as it finds the first matching order.

-- Algorithm:
-- 1. Select the name of the customer as 'Customers' from the Customers table.
-- 2. Use a WHERE NOT EXISTS subquery to check if there is no record in Orders where the customerId equals the customer's id.

-- Time Complexity:
-- O(N + M) or O(N log M) depending on the presence of indexes. Since it short-circuits, it is highly efficient in practice.

-- Space Complexity:
-- O(1) auxiliary space, as no intermediate subquery result set needs to be materialized in memory.

SELECT name AS Customers -- Select the customer name and alias it to 'Customers' for output.
FROM Customers AS C -- Scan the Customers table, aliasing it as C.
WHERE NOT EXISTS ( -- Check that no matching row exists in the subquery.
    SELECT 1 -- Return a constant 1 since we only care about existence, not actual column values.
    FROM Orders AS O -- Scan the Orders table, aliasing it as O.
    WHERE O.customerId = C.id -- Correlate the subquery by matching the order's customerId with the customer's id.
);