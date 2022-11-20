-- select
select * from Student;

-- 중복 제거
select distinct dept_name from Student;

-- where
select course_id,title from course where dept_name = 'Biology';

-- and/or
select course_id,title,dept_name,credits from course where dept_name = 'Biology' or credits = 4;

-- like
select id,name,dept_name,tot_cred from Student where name Like 'S%';

-- between
select id,name,dept_name,salary from instructor where salary between 70000 and 80000;

-- order by
select id,name,dept_name,salary from instructor order by salary desc;
