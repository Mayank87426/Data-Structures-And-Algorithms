SELECT employee_id, department_id
FROM Employee
WHERE primary_flag = 'Y'
GROUP BY employee_id, department_id
union
select employee_id,department_id from employee
group by employee_id
having count(department_id)=1
