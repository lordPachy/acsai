library(datasets)
library(MASS)
library(ggplot2)
#Let's open a dataset of cars and see the car space in dataset
head(mtcars)
summary(mtcars)
length(mtcars$mpg)
#This shows the pdf of the value in density
plot(density(mtcars$mpg))
#This gives a scatterplot of the data, with labels etc
plot(mtcars$mpg, mtcars$wt,
     main = "MPG vs Weight", xlab = "Miles per Gallon", ylab = "Weight")
#It is possible to see the distribution with histograms
hist(mtcars$mpg,
     main = "Histogram of Miles per Gallon",
     xlab = "Miles per Gallon", ylab = "Count")
#Let's perform a T-Test, a statistical method for comparison likeness
#of distributions:
t.test(mtcars$mpg ~ mtcars$am, var.equal = TRUE)
#This method is useful to see if a distribution is basically noise
#Let's try a simulation function:
t.sim <- function(v1, v2, ns, lim) {
  n <- 50000  # The number of iterations to simulate the distribution of sample means
  sds <- sd(c(v1, v2))  # Calculate the standard deviation of the combined samples
  mu <- mean(c(v1, v2))  # Calculate the mean of the combined samples
  meanDIFF <- rep(NA, n)  # Initialize an empty vector to store the differences in sample means
  # Perform the simulation of sample means
  for (i in 1:n) {
    g <- rnorm(ns, mu, sds)  # Generate a random sample 'g' from a normal distribution with mean 'mu' and standard deviation 'sds'
    k <- rnorm(ns, mu, sds)  # Generate another random sample 'k' from the same distribution
    meanDIFF[i] <- mean(g) - mean(k)  # Calculate the difference in sample means and store it in the vector
  }
  # Plotting the histogram of the differences in sample means
  hist(meanDIFF, xlim = c(-lim, lim))  # Create a histogram to visualize the distribution of sample means
  ourDIFF <- mean(v1) - mean(v2)  # Calculate the difference in means for the original samples
  segments(ourDIFF, 0, ourDIFF, 10000, col = "red")  # Add a red vertical line to the histogram representing the difference in means of the original samples
}
#Let's call the function:
t.sim(
  mtcars[mtcars$am == 0, "mpg"],  # Select 'mpg' values for the group with automatic transmission
  mtcars[mtcars$am == 1, "mpg"],  # Select 'mpg' values for the group with manual transmission
  20,  # Set the sample size for each iteration to 20
  10   # Set the range of the x-axis in the histogram to -10 to 10
)
#We can try to plot all of this with ggplot:
gg <- ggplot(mtcars, aes(x = mpg, y = wt, color = factor(cyl)))
# Add geom_point layer to the plot
# This line adds the 'geom_point' layer to the 'gg' plot object.
# It represents the data points as individual points on the scatterplot.
gg + geom_point() +
  # Customize plot labels
  # 'labs' function is used to customize the plot labels.
  # 'title = "MPG vs Weight"' sets the title of the plot as "MPG vs Weight",
  # 'x = "Miles per Gallon"' sets the label for the x-axis as "Miles per Gallon",
  # 'y = "Weight"' sets the label for the y-axis as "Weight".
  labs(title = "MPG vs Weight", x = "Miles per Gallon", y = "Weight") + theme_classic()
