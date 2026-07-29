-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- Use a correlated subquery in the SELECT clause to fetch the product_name for 
-- each row in the Sales table. This acts as a brute-force lookup since the database 
-- executes a subquery scan for every single record in the outer Sales table.

-- Algorithm:
-- 1. Scan through each record in the Sales table S.
-- 2. For each record, execute a subquery on the Product table P to find the matching product_name where product_id matches.
-- 3. Return the subquery result (product_name) along with the sale's year and price.

-- Time Complexity:
-- O(N * M) in the worst case (where N is the number of rows in Sales and M is the number of rows in Product)
-- if there are no indexes, or O(N log M) with a primary key index on Product.

-- Space Complexity:
-- O(N) to store the result set, or O(1) auxiliary space beyond the output.

SELECT 
    (SELECT P.product_name 
     FROM Product AS P 
     WHERE P.product_id = S.product_id) AS product_name, -- Correlated subquery to look up the product name for each sales record
    S.year, -- Retrieve the sale year from the Sales table
    S.price -- Retrieve the sale price from the Sales table
FROM Sales AS S; -- Iterate through all rows in the Sales table

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- Use an INNER JOIN to combine Sales and Product tables. Since product_id in Sales 
-- is a foreign key referencing Product, every sale will have a matching product. 
-- INNER JOIN explicitly filters for matching rows and avoids any overhead of LEFT JOIN 
-- which preserves unmatched rows from the left table.

-- Algorithm:
-- 1. Perform an inner join between the Sales table and the Product table.
-- 2. Match rows where the product_id columns in both tables are equal.
-- 3. Extract the product_name, year, and price columns from the joined rows.

-- Time Complexity:
-- O(N + M) or O(N log M) depending on the database engine's join algorithm (e.g., Hash Join or Index Nested Loop Join)
-- where N is the number of Sales records and M is the number of Product records.

-- Space Complexity:
-- O(N) to store the result set, with O(1) auxiliary space beyond the output.

SELECT 
    P.product_name, -- Select the name of the product from the joined Product table
    S.year, -- Select the year from the Sales table
    S.price -- Select the price from the Sales table
FROM Sales AS S -- Source from the Sales table S
INNER JOIN Product as P -- Perform an INNER JOIN with the Product table P to match rows
ON S.product_id = P.product_id; -- Match rows based on the shared product_id column

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- Use a LEFT JOIN to join the Sales table S with the Product table P.
-- This ensures that we retrieve all sales records even if some product_ids do not exist in the Product table.
-- Since product_id is a foreign key, the database optimizer can perform index lookups efficiently.

-- Algorithm:
-- 1. Scan the Sales table S.
-- 2. Perform a left join with the Product table P using product_id.
-- 3. Project the product_name from Product table and the year and price from Sales table.

-- Time Complexity:
-- O(N + M) or O(N log M) depending on join execution plan (e.g., Index Nested Loop Join, where N is the number of Sales records).
-- With product_id being the primary key of Product, lookups are O(1) or O(log M).

-- Space Complexity:
-- O(N) to store the resulting rows, or O(1) auxiliary space beyond the output.

SELECT P.product_name,S.year,S.price -- Select the product name, sale year, and sale price for the output
FROM Sales AS S -- Source from the Sales table as S to get sales information
LEFT JOIN -- Perform a LEFT JOIN to preserve all rows in Sales
Product as P -- Join with the Product table as P to fetch product names
ON -- Specify the condition for joining the two tables
S.product_id=P.product_id; -- Match rows where the product IDs are equal