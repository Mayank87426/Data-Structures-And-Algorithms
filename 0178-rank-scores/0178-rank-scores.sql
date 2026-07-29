-- ===============================
-- Brute Force Approach
-- ===============================

-- Approach:
-- For each score in the Scores table, calculate its rank by scanning the table
-- and counting how many unique scores are greater than or equal to it.
-- This correlated subquery executes for every single row in the outer query,
-- making it computationally expensive but straightforward.

-- Algorithm:
-- 1. Select the score from the outer Scores table (aliased as s1).
-- 2. Use a correlated subquery to scan the inner Scores table (aliased as s2).
-- 3. Filter for s2.score values that are greater than or equal to s1.score.
-- 4. Count the distinct scores in that subset to handle duplicate scores (ties) properly.
-- 5. Order the final results in descending order by score.

-- Time Complexity:
-- O(N^2) where N is the number of rows in the Scores table, as we perform a table scan for each row.

-- Space Complexity:
-- O(N) auxiliary space needed for sorting the final result set.

SELECT
    s1.score, -- Select the current score to be displayed in the output
    (
        SELECT COUNT(DISTINCT s2.score) -- Count unique higher/equal scores to determine the dense rank
        FROM Scores s2 -- Scan the Scores table to compare with the outer score
        WHERE s2.score >= s1.score -- Filter for scores that are greater than or equal to the current score
    ) AS `rank` -- Alias the calculated count as 'rank' to match the output schema
FROM Scores s1 -- Use the outer Scores table to drive the rank calculations
ORDER BY s1.score DESC; -- Order results from highest score to lowest as required by the problem

-- ===============================
-- Better Approach
-- ===============================

-- Approach:
-- Perform a non-equi join (theta join) between the Scores table and a subquery of unique scores.
-- For each score in the first table, we join it with all unique scores from the subquery that are
-- greater than or equal to it. Grouping by score and taking the count of these matched unique scores
-- yields the rank. This avoids repeating the scan for duplicates.

-- Algorithm:
-- 1. Create a derived table of unique scores to count only unique values.
-- 2. Join the main Scores table s1 with the unique scores s2 such that s1.score <= s2.score.
-- 3. Group the joined rows by the primary key (id) and score of s1.
-- 4. Count the matched scores from s2 to compute the dense rank.
-- 5. Order the result set by score in descending order.

-- Time Complexity:
-- O(N * U) where N is the total number of scores and U is the number of unique scores.
-- In the worst case where all scores are unique, it is O(N^2), but practically better with duplicates.

-- Space Complexity:
-- O(U + N) to store the intermediate unique scores set and the final output.

SELECT
    s1.score, -- Select the score from the original table to output
    COUNT(s2.score) AS `rank` -- Count the number of qualifying unique scores to find the dense rank
FROM Scores s1 -- Base table containing all the original scores and ids
INNER JOIN (
    SELECT DISTINCT score -- Retrieve the unique set of scores from the table
    FROM Scores
) s2 -- Derived table representing the distinct scores reference
    ON s1.score <= s2.score -- Join condition where outer score is less than or equal to the unique score
GROUP BY s1.id, s1.score -- Group by id (primary key) and score to compute count per row
ORDER BY s1.score DESC; -- Sort the ranked scores from highest to lowest

-- ===============================
-- Optimal Approach
-- ===============================

-- Approach:
-- Use the SQL standard window function DENSE_RANK(). Window functions are optimized by the database engine,
-- avoiding manual self-joins or correlated subqueries. DENSE_RANK() assigns ranks without gaps (e.g., 1, 2, 2, 3),
-- which matches the problem requirements.

-- Algorithm:
-- 1. Apply DENSE_RANK() over the Scores table partition ordered by score in descending order.
-- 2. Retrieve the score and the computed rank value for each row.
-- 3. Sort the final result set by score descending.

-- Time Complexity:
-- O(N log N) where N is the number of rows in the Scores table, dominated by the sorting step inside the window function and ORDER BY.

-- Space Complexity:
-- O(N) to store the partition state and output the ordered rows.

SELECT
    score, -- Select the score column to be displayed in the output
    DENSE_RANK() OVER (ORDER BY score DESC) AS `rank` -- Compute the rank consecutively without gaps using the window function
FROM Scores -- Retrieve rows from the main Scores table
ORDER BY score DESC; -- Order the final output records by score from highest to lowest