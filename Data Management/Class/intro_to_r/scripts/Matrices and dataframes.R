#Let's create an identity matrix
ident = matrix(c(1, 0, 0, 0, 1, 0, 0, 0, 1), ncol = 3, byrow = T)
idetn
ident
#Summing a matrix with a scalar shifts the entire matrix
ident +1 
ident
#Accessing elements
ident[1, 1]
ident[1, 2] = ident[1, 2] + 1
ident[1, 2]
#Remember the adjacency matrix representation?
#This is for the traspose:
ident
t(ident)
#Let's perform vector and matrix multiplication:
v = c(1, 2, 3)
ident %*% v
v %*% ident
ident %*% t(ident)
#Going back to datasets
library(datasets)
summary(iris)
#Binning is the operation of dividing the data into chunks, given by quantiles
head(iris)
#We can cast matrices to dataframes:
d = data.frame(ident)
d
names(d) = c("a", "b", "c")   #adds names to the columns
d
set.seed(42)
height = rnorm(10, mean = 170, sd = 20)
weight = rnorm(10, mean = 70, sd = 5)
head(height)
#Note that standard precision is to the fourth digit;
#it can be changed:
height = round(height, 2)
head(height)
hw = data.frame(h = height, w = weight)
summary(hw)
head(hw)
hw$h
hw$w
#To add a new row to the table:
newline = c(169, 60)
hw = rbind(hw, newline)
hw
#There are two ways of adding a column:
age = rnorm(11, 25, 2)
hw$age = age
hw
income = rnorm(11, 40000, 3000)
income
hw$income = income
summary(hw)
