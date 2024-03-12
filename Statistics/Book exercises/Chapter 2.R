#Exercise 16
array = c(112, 121, 126, 108, 141, 104, 136, 134, 121, 118, 143, 116, 108, 122, 127, 140, 113, 117, 126, 130, 134, 120, 131, 133, 118, 125, 151, 147, 137, 140, 132, 119, 110, 124, 132, 152, 135, 130, 136, 128)
array.mean()
array.mean
mode(array)
library(modeest)
install.packages("modeest")
install.packages("gss")
install.packages("-llapack")
modeest::mfv(array)
mfv(array)
modeest::mfv1(array)
getmode <- function(v) {
  uniqv <- unique(v)
  uniqv[which.max(tabulate(match(v, uniqv)))]
}
getmode(array)
median(array)
hist(array, freq = FALSE)
#Exercise 21
array = c(114, 114, 114, 113, 111, 97, 94, 66, 63, 52, 48, 33)
mean(array)
?var()
var(array)
#Exercise 26
array = c(3.46, 3.72, 3.95, 3.55, 3.62, 3.80, 3.86, 3.71, 3.56, 3.49, 3.96, 3.90, 3.70, 3.61,
          3.72, 3.65, 3.48, 3.87, 3.82, 3.91, 3.69, 3.67, 3.72, 3.66, 3.79, 3.75, 3.93, 3.74,
          3.50, 3.83)
mean(array)
sqrt(var(array))
sqrt(var(c(26, 28, 22, 23, 18, 31, 30)))
