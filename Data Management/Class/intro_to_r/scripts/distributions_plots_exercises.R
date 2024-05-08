data("iris")
library(ggplot2)

#Exercise 1
ggplot(iris, aes(x = Petal.Length)) + geom_histogram(binwidth = 0.2)

#Exercise 2
library(tidyr)
iris_mod = pivot_longer(data = iris, cols = c("Sepal.Width", "Petal.Width"), names_to = "Specie", values_to = "Values")
iris_mod
summary(iris_mod)
ggplot(iris_mod, aes(x = Values, color = Specie, linetype = Specie)) + geom_density()

#Exercise 3
ggplot(iris, aes(x = Petal.Length)) + stat_ecdf()
ggplot(iris, aes(x = Petal.Length)) + stat_function(fun = pnorm, args = list(mean = mean(iris$Petal.Length, sd = sd(iris$Petal.Length))))

ggplot(iris, aes(x = Petal.Length)) + stat_function(fun = function(x) 1 - pnorm(x, mean = mean(iris$Petal.Length), sd = sd(iris$Petal.Length)))

#Exercise 4
ggplot(iris, aes(x = Species, y = Sepal.Length)) + geom_boxplot()

#Exercise 5
petallength_width = cor(iris[3:4])
heatmap(petallength_width, col = cm.colors(256))

#Exercise 6
ggplot(iris, aes(sample = Sepal.Width)) + geom_qq_line() + geom_qq()
ggplot(iris, aes(sample = Sepal.Length)) + geom_qq_line() + geom_qq()
ggplot(iris, aes(sample = Petal.Width)) + geom_qq_line() + geom_qq()
