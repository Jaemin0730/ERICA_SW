\echo '======'
\echo 'Task1'
\echo '======'


\echo '--------------------------------------------'
\echo 'Submission: Week 5 Hands-on Assignment'
\echo 'name: Kim Jaemin'						   
\echo 'student number: 2021050300'			  
\echo 'section: 23360'						 
\echo '--------------------------------------------'

\echo '======'
\echo 'Task2'
\echo '======'

\set ECHO all
drop table phonebook;

CREATE TABLE phonebook (
	id numeric PRIMARY KEY,
	name varchar(50) NOT NULL,
	phone varchar(100) DEFAULT '010-0000-0000',
	email varchar(100),
	age numeric DEFAULT 21,
	memo bytea,
	regDate date
);

\set ECHO none
drop table phonebook;

CREATE TABLE phonebook (
	id numeric PRIMARY KEY,
	name varchar(50) NOT NULL,
	phone varchar(100) DEFAULT '010-0000-0000',
	email varchar(100),
	age numeric DEFAULT 21,
	memo bytea,
	regDate date
);

\echo '======'
\echo 'Task3'
\echo '======'

insert into phonebook values (12,'bob','010-1111-2222','database@test.com',28,'','2018-08-02');

select * from phonebook;

insert into phonebook (id,name,phone,age,regDate) values (15,'alice','010-2222-1111',21,'2021-03-12');

select * from phonebook;

\echo '======'
\echo 'Task4'
\echo '======'

insert into phonebook values (12,'bob','010-2222-3333','database@test.com',28,'','2018-08-02');

insert into phonebook (id, name) values (12, 'bob') ON CONFLICT (id) DO NOTHING;

insert into phonebook values (12,'bob','010-2222-3333','database@test.com',28,'','2018-08-02') ON CONFLICT (id) DO update set phone = EXCLUDED.phone;

select * from phonebook;

\echo '======'
\echo 'Task5'
\echo '======'

update phonebook set (phone,age) = ('010-1111-2222',34) where id = 12;

select * from phonebook;