print("hello world!")
getwd()
setwd("/home/pachy/Desktop")
setwd("/home/pachy/Desktop/ACSAI/acsai/Data Management/Class practice")
getwd()
library(ggplot2)
install.packages("ggplot2")
library("ggplot2")
library("datasets")
summary(iris)
View(iris)
plot(iris$Sepal.Length)
hist(iris$Sepal.Length)
plot(iris$Sepal.Length,iris$Sepal.Width)
iris[1:10,"Sepal.Length"]
num = 7
class(num)
v = c(1, 2,3, 4)
v[3]
v[2]
v = c(1, "Francesco")
class(v)
a = list(name="Chelsea", heigth=42)
mat = matrix(c(1,0,0,1,1,1,1, 1, 1), ncol =3)
mat
mat[1, 2]
seq(1,10, length=10)
seq(1,10,length=6)
seq(1,10,by=2)
seqs = rep(1:10, 10)
seqs
seqs > 4
our_names <- c("Chelsea", "John", "Jim", "Bianca", "Amy", "John", "Jake", "Tony", "Sam")
ifelse(our_names == "John", "J-Money", our_names)
which(our_names == "John")
library("maps")
1 +2 
