#This exercise was taken from https://www.datacamp.com/tutorial/linear-regression-R?authuser=0

#LINEAR AND MULTIPLE LINEAR REGRESSION (height and age of babies)
#This is used to read Microsoft Excel files
install.packages("readxl")
library(readxl)
#Let's import data from Microsoft Excel
getwd()
setwd("~/Desktop/ACSAI/acsai/Data Management/Home practice/")
ageandheight = read_excel("ageandheight.xls", sheet = "Hoja2")
head(ageandheight)
summary(ageandheight)
ageandheight  #there is an error at observation 7: let's fix it
ageandheight[7, 2] = 79.9
ageandheight

#Linear regression
lmHeight = lm(height ~ age, data = ageandheight)
summary(lmHeight)

library(ggplot2)
ggplot(data = ageandheight, mapping = aes(x=age, y = height)) + geom_point() + geom_smooth(method = lm)

#Multiple linear regression
lmHeightmul = lm(height ~ age + no_siblings, data = ageandheight)
summary(lmHeightmul)

#NONLINEAR REGRESSION AND RESIDUALS (pressure and temperature)
pressure = read_excel("pressure.xlsx")
head(pressure)
summary(pressure)
pressure

lmTemp = lm(Pressure ~ Temperature, data = pressure)
summary(lmTemp)
plot(pressure, pch=16, col="blue")
abline(lmTemp)
plot(lmTemp$residuals, pch = 16, col = "red")
#There is a precise pattern in the residuals.
#It is the second order function missing from the linear fit.
#If the problem is actually linear, the residuals are random.
#This can be fixed:
lm_temp_quadratic = lm(Pressure ~ Temperature + I(Temperature^2), data = pressure)
summary(lm_temp_quadratic)
plot(lm_temp_quadratic$residuals, pch=16, col="green")

#INFLUENTIAL POINTS
#It is possible to detect outliers by using the Cook's distance, and by plotting it.
#Altering the data
ageandheight[2, 2] = 7.7
head(ageandheight)

#Doing a (bad) lm
bad_height_lm = lm(height ~ age, data = ageandheight)
summary(bad_height_lm)
plot(cooks.distance(bad_height_lm), pch = 16, col = "blue")
