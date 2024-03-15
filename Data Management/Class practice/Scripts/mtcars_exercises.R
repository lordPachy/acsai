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
ggplot(mtcars, aes(y = mpg, x = wt), color = cyl) + geom_point() + facet_grid(~gear) + geom_smooth(method = "lm") + theme_dark()

#Exercise 4
mode(mtcars)
fourcyl = dplyr::filter(mtcars, cyl == 4)
summary(fourcyl)
head(fourcyl)
sixcyl = dplyr::filter(mtcars, cyl == 6)
summary(mtcars)
ggplot(mtcars, aes(x = mpg)) + geom_boxplot() + facet_wrap(~cyl)
ggplot(mtcars, aes(x = cyl, y = mpg)) + geom_violin() + facet_wrap(~cyl)
mtcars[mtcars$cyl == 4, ]
t.test(mtcars[mtcars$cyl == 4, ]$mpg, mtcars[mtcars$cyl == 6, ]$mpg)
