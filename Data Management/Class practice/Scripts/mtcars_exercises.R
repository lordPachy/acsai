library(datasets)
library(MASS)
library(ggplot2)

summary(mtcars)
head(mtcars)
mode(mtcars$am)

#Exercise 1
ggplot(data = mtcars, mapping = aes(x = wt, y = mpg)) + geom_point() + geom_smooth(method = "lm")

#Exercise 2
ggplot(data = mtcars, mapping = aes(x = hp, y = qsec)) + geom_point(mapping = aes(color = vs)) + geom_smooth(method = "lm")

#Exercise 3
