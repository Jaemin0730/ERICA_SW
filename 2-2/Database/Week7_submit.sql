/* 
=========================
Week7 Assignment

CSE3010 Database 
Fall, 2022
=========================

-----------
Description
-----------

This is a skeleton code for the week7 assignment.

-------
Grading
-------

Execute sql files submitted by students by using 
\i meta-command in psql.

For example, do
	\i wk7_submit.sql

Each task's output must be consistent with those
screenshots in the assignment PPT slide on LMS.

Total: 2 points
1 point for each task (no partial mark)

*/

\echo '==========='
\echo 'Preparation'
\echo '==========='

\set ECHO all
INSERT INTO section VALUES ('CS-101', '1', 'Fall', 2022, 'Watson', '120', 'A');
INSERT INTO section VALUES ('CS-315', '2', 'Fall', 2022, 'Watson', '100', 'B');
SELECT * FROM section;
\set ECHO none

-- Task 1. counting enrollment across all sections including sections 
--         with no student taking them
\echo '======'
\echo 'Task 1'
\echo '======'

select course_id, sec_id,semester,year,count(id) as num_enrollment
from section natural left outer join takes
group by course_id, sec_id,semester,year;

-- Task 2. finding the maximum and minimum enrollment across all sections
\echo '======'
\echo 'Task 2'
\echo '======'

select max(num) as max_enrollment,min(num) as min_enrollment
from (select count(id) from section natural left outer join takes group by course_id, sec_id,semester,year) as ci(num);

\echo '========'
\echo 'CLEANING'
\echo '========'

\set ECHO all
DELETE FROM section WHERE course_id = 'CS-101' AND 
	sec_id = '1' AND semester = 'Fall' AND year = 2022;
DELETE FROM section WHERE course_id = 'CS-315' AND 
	sec_id = '2' AND semester = 'Fall' AND year = 2022;
SELECT * FROM section;
\set ECHO none




