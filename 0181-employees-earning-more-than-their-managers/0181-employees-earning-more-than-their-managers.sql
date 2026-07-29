-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- We can use a correlated subquery. For each employee in the outer query,
-- we execute a subquery to find their manager's salary (where E2.id = E1.managerId).
-- We then filter the outer query to return only employees whose salary is greater than their manager's salary.

-- Algorithm:
-- 1. Select the name of the employee from the outer Employee table E1.
-- 2. Use a WHERE clause with a subquery to find the manager's salary.
-- 3. In the subquery, filter the Employee table E2 where the manager's ID matches the current employee's managerId.
-- 4. Compare the current employee's salary to the value returned by the subquery.

-- Time Complexity:
-- O(N^2) in the worst case (without indexes), as the database may perform a full table scan for each row.

-- Space Complexity:
-- O(1) auxiliary space as we do not use temporary tables.

SELECT E1.name AS Employee -- Select the employee name
FROM Employee E1 -- Iterate through each employee
WHERE E1.salary > ( -- Check if employee's salary is greater than manager's salary
    SELECT E2.salary -- Select manager's salary
    FROM Employee E2 -- Scan the Employee table for the manager
    WHERE E2.id = E1.managerId -- Match manager's id with the employee's managerId
);

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- We can use an implicit join (comma join) to join the Employee table with itself.
-- We pair every employee with their manager in the WHERE clause and filter those
-- where the employee's salary is greater than the manager's salary.

-- Algorithm:
-- 1. Reference the Employee table twice in the FROM clause: E1 for employee, E2 for manager.
-- 2. Filter rows where E1.managerId equals E2.id to link employees to their managers.
-- 3. Further filter to ensure E1.salary is greater than E2.salary.
-- 4. Select the name of E1.

-- Time Complexity:
-- O(N^2) conceptually due to the Cartesian product, though modern optimizers can convert this to an inner join.

-- Space Complexity:
-- O(1) auxiliary space or O(N) depending on execution plan.

SELECT E1.name AS Employee -- Select the employee name
FROM Employee E1, Employee E2 -- Perform an implicit cross product of the Employee table with itself
WHERE E1.managerId = E2.id -- Filter to match employee's managerId with the manager's id
  AND E1.salary > E2.salary; -- Filter to only keep employees earning more than their manager

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- We use an explicit INNER JOIN to match employees with their managers.
-- This allows the query optimizer to choose the most efficient join path (such as a Hash Join or Merge Join),
-- and utilize index lookups on the primary key column (id).

-- Algorithm:
-- 1. Perform an INNER JOIN on Employee AS E (manager) and Employee AS M (employee).
-- 2. Match the manager's ID (E.id) to the employee's managerId (M.managerId).
-- 3. Filter rows where the employee's salary (M.salary) is greater than the manager's salary (E.salary).
-- 4. Select the employee's name (M.name).

-- Time Complexity:
-- O(N) since index lookups on the primary key (id) run in O(1) time per employee.

-- Space Complexity:
-- O(N) for join execution structures if a hash table or sorting is required by the engine.

SELECT M.name as Employee FROM Employee AS E -- Select the employee's name (aliased as Employee) from Employee table (as manager E)
JOIN Employee AS M -- Join with Employee table (as employee M)
ON
E.id=M.managerId -- On condition: match the manager's id with the employee's managerId
WHERE M.salary>E.salary; -- Filter condition: keep only employees whose salary is greater than their manager's salary