#Histograms
library(datasets)
histo = hist(iris$Sepal.Length)
str(histo)
class(histo)
#Linear model of length w.r.t. width
?lm
a = lm(iris$Sepal.Length ~ iris$Sepal.Width)
str(a)
summary(a)
plot(a)
#Apply functions on columns with the method apply():
mat <- matrix(c(1,2,3,4,7,4,2,5,7), nrow = 3)
apply(mat,2,mean)
