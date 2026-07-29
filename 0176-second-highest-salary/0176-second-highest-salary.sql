-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- The brute force approach finds the maximum salary from the Employee table, and then
-- finds the maximum salary among all salaries that are strictly less than that maximum.
-- This effectively finds the second highest salary. If there is no such salary,
-- the MAX() function naturally returns NULL.

-- Algorithm:
-- 1. Use a subquery to find the overall maximum salary: SELECT MAX(salary) FROM Employee.
-- 2. Use an outer query to find the maximum salary from the Employee table where the
--    salary is strictly less than the result of the subquery.
-- 3. Return the result under the alias 'SecondHighestSalary'.

-- Time Complexity:
-- O(N) where N is the number of rows in the Employee table. This is because we perform
-- two full table scans (one for the inner MAX and one for the outer MAX).

-- Space Complexity:
-- O(1) auxiliary space as no additional data structures are created.

SELECT MAX(salary) AS SecondHighestSalary -- Select the maximum salary among the filtered set to get the second highest, renaming the column as required
FROM Employee -- Query the Employee table
WHERE salary < ( -- Filter out any salary that is equal to or greater than the absolute maximum salary
    SELECT MAX(salary) -- Subquery to find the absolute maximum salary in the entire table
    FROM Employee -- Scan the Employee table to locate this maximum value
);

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- The better approach utilizes the DENSE_RANK() window function to rank all distinct salaries
-- in descending order. By using DENSE_RANK() instead of RANK() or ROW_NUMBER(), duplicate salaries
-- receive the same rank, ensuring that the second highest distinct salary is ranked exactly 2.
-- We wrap this in a Common Table Expression (CTE) and then select the salary with rank 2.
-- To handle the case where a second highest salary does not exist and ensure we return NULL,
-- we use the MAX() aggregation function on the filtered result.

-- Algorithm:
-- 1. Create a CTE called 'RankedSalaries' that computes the DENSE_RANK() of salaries in descending order.
-- 2. Select the MAX(salary) from 'RankedSalaries' where the rank is equal to 2.
-- 3. The MAX() aggregation returns the salary if found, or NULL if no record has a rank of 2.

-- Time Complexity:
-- O(N log N) where N is the number of rows in the Employee table. Ranking all salaries
-- requires sorting the table.

-- Space Complexity:
-- O(N) to store the intermediate ranked results in the Common Table Expression.

WITH RankedSalaries AS (
    SELECT 
        salary, -- Select the salary column to be ranked
        DENSE_RANK() OVER (ORDER BY salary DESC) AS rank_num -- Rank distinct salaries in descending order so the highest is 1, second highest is 2, etc.
    FROM Employee -- Query from the Employee table
)
SELECT MAX(salary) AS SecondHighestSalary -- Use MAX() to select the salary with rank 2, ensuring NULL is returned if no such rank exists
FROM RankedSalaries -- Query the ranked CTE
WHERE rank_num = 2; -- Filter for the second highest distinct salary

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- The optimal approach sorts the distinct salaries in descending order and uses LIMIT 1 OFFSET 1
-- to retrieve the second highest salary directly. By wrapping this subquery in an outer SELECT statement,
-- SQL automatically returns NULL if the subquery returns an empty set (i.e., if there is no second highest salary).
-- If an index exists on the 'salary' column, the database engine can retrieve the result in O(log N) or O(1)
-- time by traversing the index instead of performing a full table scan and sort.

-- Algorithm:
-- 1. Run a subquery that retrieves unique salaries using SELECT DISTINCT.
-- 2. Order the distinct salaries in descending order (highest first).
-- 3. Skip the first salary (the highest one) using OFFSET 1, and retrieve the next one using LIMIT 1.
-- 4. Wrap the subquery in a SELECT statement to return NULL if the subquery yields no rows.

-- Time Complexity:
-- O(N log N) in the worst case due to sorting the distinct salaries. However, if there is an
-- index on the 'salary' column, the complexity is optimized to O(log N) or O(1) because the engine
-- can directly scan the index.

-- Space Complexity:
-- O(1) as sorting can be avoided or optimized if an index is utilized.

SELECT ( -- Outer select to wrap the subquery and automatically return NULL if the subquery is empty
    SELECT DISTINCT salary -- Retrieve unique salary values to avoid duplicate ranks
    FROM Employee -- Query the Employee table
    ORDER BY salary DESC -- Sort the salaries in descending order to place the highest salaries at the top
    LIMIT 1 OFFSET 1 -- Skip the highest salary (OFFSET 1) and retrieve only the second highest (LIMIT 1)
) AS SecondHighestSalary; -- Alias the resulting column as SecondHighestSalary
