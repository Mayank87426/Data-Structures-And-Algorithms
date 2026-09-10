SELECT user_id,
      round( SUM(action = 'confirmed') / COUNT(*),2) AS confirmation_rate
FROM Confirmations
GROUP BY user_id

UNION

SELECT user_id,
       0.00 AS confirmation_rate
FROM Signups
WHERE user_id NOT IN (
    SELECT user_id
    FROM Confirmations
);
