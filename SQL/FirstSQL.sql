-- Tells which scheme to look at:
use northwind;
-- TO set safe mode to 0 - let us do unsafety things
SET SQL_SAFE_UPDATES = 0; -- ... = 1 - will switch it up again

-- select employeeID, Firstname, Lastname from employees;

-- select EmployeeID, Firstname, Lastname from employees
-- where EmployeeID > 3;

-- Using alias:
-- select EmployeeID, EmployeeID * 100 as EidMultyBy10, Firstname, lastname from employees;

-- select employeeid from employeeterritories;

-- Use distinct to see only 1 instance for a object (without seeing it more then once):
-- select distinct EmployeeId, territoryID from employeeterritories;

-- Use "is" or "is not" only while working with NULL:
-- select * from employees
-- where region is null;

-- select * from products;
-- select * from suppliers;

-- top 1 unitprice
/*
this is
a 
comment
block
*/

-- select * from products 
-- order by unitprice desc
-- limit 0, 1;
/*
 -------------------------- Class Work: --------------------------
*/
-- SELECT * FROM employees
-- WHERE EmployeeID >= 5 AND EmployeeID <= 9
/*
>>> or:
-- SELECT * FROM employees
-- WHERE EmployeeID between 5 AND 9
*/

-- >>> LIKE: {_ == placeholder}, {% == 0 or more instances of any letter}
-- SELECT * FROM employees
-- WHERE Lastname LIKE '%E%'; -- last name has E in it;

-- SELECT * FROM employees
-- WHERE Lastname = "Peacock" or TitleOfCourtesy = 'Mr.' or TitleOfCourtesy = 'Ms.';
/*
>>> or:
-- SELECT * FROM employees
-- WHERE Lastname = "Peacock" or TitleOfCourtesy in ('Mr.', 'Ms.');
*/

-- SELECT * FROM employees
-- WHERE Region IS NULL;

-- SELECT * FROM employees
-- WHERE lastname LIKE 'a%' 
-- OR
-- lastname LIKE 'b%' 
-- OR
-- lastname LIKE 'c%';

/*
>>> or:
SELECT * FROM employees
WHERE left(lastname, 1) IN ('a%', 'b%', '%c');
*/

/* --------------------- MORE CLASS EXERCISES - SqlExercises pdf: -----------------------*/

/* (1) Queries */
-- 1) No (SQL is not case sensitive)
-- 2) (a) Didn't specify from which table to grab the data | (c) no comma between amount and name fields
-- 3) a, b and d (and it will just will show the string)
-- 4) answer:
-- select Firstname, Title from employees;
-- 5) answer:
-- select Title, Firstname from employees;
-- 6) answer:
-- select distinct Title from employees;
-- 7) answer:
-- select * from orders;
-- 8) answer:
-- select FIrstname, HireDate, Region, Country from employees;

/* (2) Operators */
-- 1) answer:
-- select * from employees where Firstname like 'M%';
-- 2) answer:
-- select * from employees where firstname = 'robert' and city = 'london';
-- 3) answer:
-- select ProductName, UnitPrice, UnitsInStock, UnitsInStock % 2 as Modulo from products;
-- 4) BETWEEN keyword
-- 5) answer:
-- select ProductID as ID, productNAME as Name, unitprice as Price from products;
-- 6) answer:
-- select customerid, concat(address, ' ', city) as FullAddress from customers;
-- 7) answer:
-- select * from customers where country = 'uk';
-- 8) answer:
-- select * from products where discontinued = 0 and unitsInStock = 0
-- 9) (a) None (Empty group) | (b) 3 instances: AL, AL, FERRIS
-- 10) answer (with concat and space function):
/* >>> friends table is not available:
select concat(firstname, space(9), 'FROM') as NAME, state as st from friends
where firstname = 'AL' and state = 'IL';*/
-- select concat(firstname , space(9) , 'FROM') as Name, country as st from employees
-- where firstname = 'Steven' and country = 'UK';
-- 11) answer:
/* >>> friends table is not available:
select concat(lastname , space(4) , firstname) as Name, concat(areacode ,'-',phone) as PHONE
from friends
where areacode like '3%'*/
-- select concat(firstname, ',', space(5), lastname) as NAME, concat(extension, '-', homephone) as PHONE
-- from employees
-- where extension like '3%';

/* (4) */
-- Having Keyword is a unique keyword that can be used only with group by

/* (5) Join */
-- 1) 50,000 * 100 lines
-- 2) inner join
-- 3) (a) No (conflict) | (b) Yes but not joining by a particular key (close to cartesian)
--    (c) Yes (best practice by Oracle) | (d) Yes | (e) Yes
-- 4) No limitation about crossing columns
-- 5) answer:
-- select p.productName, c.categoryID
-- from products as p
-- join categories as c
-- on p.categoryID = c.categoryID
-- 6) answer:
-- select p.productName, p.unitPrice, c.categoryName
-- from products as p
-- join categories as c
-- on p.categoryID = c.categoryID
-- where p.unitPrice > 50;
-- 7) answer:
-- select e.firstName, e.lastName, ifnull(t.territoryID, '0'), ifnull(t.TerritoryDescription, 'Unknown')
-- from employees as e
-- left join employeeterritories as et
-- on e.employeeID = et.employeeID
-- left join territories as t
-- on et.territoryID = t.territoryID
-- 8) answer:
-- select c.CustomerID, o.OrderID, o.ShippedDate, c.ContactName, c.City, c.Phone
-- from customers as c
-- join orders as o
-- on c.CustomerID = o.CustomerID and o.OrderID > 10700
-- order by c.CustomerID desc /* Descending order */;
-- 9) answer:
-- select c.CustomerID, c.City, od.UnitPrice, od.UnitPrice * 1.17
-- from customers as c
-- join orders as o
-- on c.CustomerID = o.CustomerID and o.ShipCity = 'London'
-- join orderdetails as od
-- on o.OrderID = od.OrderID

/* (7) Tables */
-- 1) Yes - forcing to stand in some conditions before inserting new information to the table
-- 2) No
-- 3) Checking if the inserted info is stand in some conditions before the insertion to the table
-- 4) To the father first, then to the son (with the foreign key)
-- 5) The opposite from above - first the son and then the father
-- 6) (a) No "where" condition was spesified to delete a given row
--    (b) "set" should be used ONLY with "update" while updating an existing row
--    (c) Using "update" without "set" syntax
-- 7) (a) the * will give a syntax error
--    (b) Nothing would be returned - so nothing would happen (zero rows would be deleted)
--    (c) 2 sets - no valid
-- 8) (a) No
--    (b) Yes
-- 9)
-- (a) answer:
-- insert into territories values('23234', 'Tel Aviv', 5);
-- Failed because tried to add a new region "code" (5) while its a foreign key that DOES NOT EXIST in its original table (region table - there regionID is a PRIMARY KEY), we must ADD this "5" regionID to the region table first (father), and then to the territories table (son)
-- Error Code: 1452. Cannot add or update a child row: a foreign key constraint fails (`northwind`.`territories`, CONSTRAINT `FK_Territories_Region` FOREIGN KEY (`RegionID`) REFERENCES `region` (`RegionID`))
-- (b) answer:
-- insert into region values(5, 'Gush Dan');
-- insert into territories values('23234', 'Tel Aviv', 5); -- works now!
-- 10) answer:
-- insert into employees values(50, 'Meirov', 'Natan', 'Software Engineer', 'Mr.', '1995-10-16 08:55:00.00', '2021-04-01 10:00:00.00', 'Hadar 13', 'Holon', 'Gush Dan', NULL, 'Israel', '544482549', '+972', NULL, NULL, NULL, NULL);
-- 11) answer:
-- insert into employees values(100, 'Fake', 'Mister', 'Tourist', 'Brother', '2050-01-01 00:00:01.00', '2021-04-01 10:00:00.00', 'Nowhere', 'Known', 'World', NULL, 'Galaxy', '0000000', '000', NULL, NULL, NULL, NULL);
-- Should fail, because the server validates (uses a constraint) that the BirthDate must be valid (below the current Now() DateTime)
-- 12) answer:
-- insert into employees(FirstName, LastName, City, Title, TitleOfCourtesy, EmployeeID) values('Larry', 'King', 'Los Angeles', 'Announcer', NULL, 20);
-- 13) answer:
-- update employees
-- set Address = '56 Hatikva Str.', City = 'Ramat HaSharon', Region = 'TA', PostalCode = '34533', Country = 'IL'
-- where EmployeeID = 20; -- if we does NOT specify this where - it would update ALL the rows in the table with the same things
-- 14) answer:
-- delete from employees where EmployeeID = 20;

/* (8) */
-- 1) answer:
-- create table US_Customers(
-- 	CustomerID char(5) Primary Key,
-- 	CompanyName	varchar(40),
-- 	ContactName	varchar(30),
-- 	ContactTitle varchar(30),
-- 	Address	varchar(60),
-- 	City varchar(15),
-- 	Region varchar(15),
-- 	PostalCode varchar(10),
-- 	Country	varchar(15),
-- 	Phone varchar(24),
-- 	Fax	varchar(24)
-- )
-- 2) answer:
-- insert into US_Customers
-- (select * from customers where country = 'USA');
-- 3) answer:
-- alter table US_Customers
-- add column CustomerType char(1) not null;
-- 4) answer:
-- update US_Customers
-- set CustomerType = 'W' 
-- where region in ('CA', 'OR', 'WA');
-- 5) answer:
-- alter table US_Customers
-- add column CompanyStartDate DateTime
-- 6) answer:
-- alter table US_Customers
-- add constraint check(CompanyStartDate < Now()); -- raise an error...
-- 7) answer:


-- select * from US_Customers


