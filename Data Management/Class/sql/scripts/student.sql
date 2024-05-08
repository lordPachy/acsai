-- Student (Student_ID*, First Name, Last Name, Email)
-- Even though email is a unique constraint, it cannot be used as a key since a key needs to be UNIQUE and NOT NULL
--Moreover, the primary key automatically creates the index file and cannot be changed, while the email can
CREATE TABLE Student(
  Student_id INT PRIMARY KEY,
  First_Name VARCHAR(30), 
  Last_Name VARCHAR(50),
  Email VARCHAR(100) UNIQUE,
  Age INT
);

INSERT INTO Student(Student_id, First_Name, Last_Name, Email, Age) VALUES
(10, 'Francesco', 'Calzona', 'calzona.2046920@studenti.uniroma1.it', 20),
(2, 'Anna', 'Verdi', 'ciao@studenti.uniroma1.it', 22),
(4, 'Lorenzo', 'Trapasso', 'lorenzotrapasso@gmail.com', 23);

SELECT * FROM Student;

-- Class (Class_ID*, Name, ClassYear)

CREATE TABLE Class(
	
  Class_id INT PRIMARY KEY,
  Name VARCHAR (100),
  ClassYear INT
);

INSERT INTO Class(Class_id, Name, ClassYear) VALUES
(1, 'Algorithms', 1),
(2, 'DBMS', 3),
(3, 'Physics', 2);

SELECT * FROM Class;

--Attendings((Student_ID, Course_ID)*, Year)

CREATE TABLE Attends(

  Student_id INT REFERENCES Student(Student_id),
  Class_id INT REFERENCES Class(Class_id),
  AttendingYear INT,
  
  PRIMARY KEY(Student_id, Class_id)
);

--Joining tables into the attends table

INSERT INTO Attends(Student_id, Class_id, AttendingYear) VALUES
(2, 1, 1),
(2, 2, 3),
(4, 2, 2),
(10, 2, 2);
SELECT * FROM Attends;

--INNER JOIN

SELECT S.Student_id, S.First_Name, S.Last_Name, A.Class_id
FROM Student AS S
INNER JOIN Attends AS A
ON S.Student_id = A.Student_id;

SELECT S.Student_id, S.First_Name, S.Last_Name, C.Class_id, C.Name
FROM Student AS S
INNER JOIN Attends AS A ON S.Student_id = A.Student_id
INNER JOIN Class AS C ON A.Class_id = C.Class_id;

--WHERE
SELECT S.Student_id, S.First_Name, S.Last_Name, C.Class_id, C.Name
FROM Student AS S
INNER JOIN Attends AS A
ON S.Student_id = A.Student_id
INNER JOIN Class AS C
ON A.Class_id = C.Class_id
WHERE C.class_id = 2;

SELECT S.Student_id, S.First_Name, S.Last_Name, S.Age, A.Class_id, C.Name
FROM Student AS S
INNER JOIN Attends AS A
ON S.Student_id = A.Student_id
INNER JOIN Class AS C
ON A.Class_id = C.Class_id
WHERE S.First_Name LIKE '%cesco%';

--GROUP BY (usually done on the primary key of a table). Note that select applies a group function on the groups obtained by GROUP BY
SELECT C.Name, AVG(S.Age)
FROM Student AS S
INNER JOIN Attends AS A
ON S.Student_id = A.Student_id
INNER JOIN Class AS C
ON A.Class_id = C.Class_id
GROUP BY C.Class_id;