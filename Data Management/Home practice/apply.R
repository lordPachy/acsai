#Taken from https://www.guru99.com/r-apply-sapply-tapply.html

#APPLY
#Sum on matrices: dataframe/matrix -> vector/list/array
m1 = matrix(c(1:10), nrow = 5, ncol = 6)
m1
colsum_m1 = apply(m1, 2, sum)
colsum_m1

#LAPPLY
#Apply a function to each element of a list, retaining the dimension
l1 = c(1, 2, 3, 4)
l1
mode(l1)
l2 = c("francesco", "maria", "giovanni", "diego")
mode(l2)
cube = function(x){
  return(x^3)
}
cube_l1 = lapply(l1, cube)
cube_l1
l2_lower = lapply(l2, toupper)
l2_lower
str(l2_lower)
str(cube_l1)
#SAPPLY
#Same as lapply but returns a vector
dt = cars
lmn_cars = lapply(dt, min)
smn_cars = sapply(dt, min)
lmn_cars
str(lmn_cars)
smn_cars
str(smn_cars)
cars
what = sapply(m1, sum)
what
#SLICE VECTOR EXAMPLE: EQUALITY OF SAPPLY AND LAPPLY
below_ave = function(x){
  ave = median(x)
  return(x[x == ave])
}
dt_s = sapply(dt, below_ave)
dt_l = lapply(dt, below_ave)
dt_s
dt_l
#TAPPLY
#It computes a function for each factor (categorical variable) of a vector
data(iris)
tapply(iris$Sepal.Width, iris$Species, median)
mtcars
tapply(mtcars$hp, mtcars$cyl, mean)

#MAPPLY
#It applies a certain function to all the first elements, then the seconds, then the thirds...
v1 = c(1:3)
v2 = c(3:5)
v3 = c(100:102)
mapply(function(x, y, z) z*x*y, v1, v2, v3)
mapply(paste,c("Chelsea", "Tony", "Sam"),
       c("Parlett", "Pelleriti","Jones"))
