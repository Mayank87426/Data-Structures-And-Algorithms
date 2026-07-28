SELECT M.name as Employee FROM Employee AS E
JOIN Employee AS M
ON
E.id=M.managerId
WHERE M.salary>E.salary;