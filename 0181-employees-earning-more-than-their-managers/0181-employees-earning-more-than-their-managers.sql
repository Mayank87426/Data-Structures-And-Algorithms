SELECT E.name AS Employee
FROM Employee AS E
LEFT JOIN Employee AS E1 
    ON E.managerId = E1.id
WHERE E.salary > E1.salary;
