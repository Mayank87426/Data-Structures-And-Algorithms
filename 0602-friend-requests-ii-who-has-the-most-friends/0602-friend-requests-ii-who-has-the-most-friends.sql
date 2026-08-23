WITH cte AS (
    SELECT requester_id, COUNT(requester_id) AS cnt
    FROM RequestAccepted
    GROUP BY requester_id

    UNION ALL

    SELECT accepter_id, COUNT(accepter_id) AS cnt
    FROM RequestAccepted
    GROUP BY accepter_id
)
SELECT cte.requester_id as id, SUM(cnt) AS num
FROM cte
GROUP BY cte.requester_id
ORDER BY num DESC
LIMIT 1;
