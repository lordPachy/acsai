#Functions
my_function = function(arg){
  #something
  arg + 1
}
my_function(2)
declaration = function(){
  print(paste("I hereby declare that"))
}
declaration()
#Loops
if (1 == 1){
  a <- TRUE
} else {
  a <- FALSE
}

#else if
b <- 11
if (b%%2 == 0){
  print("even")
} else if (b%%3 == 0){
  print("divisible by 3")
} else {
  print("crappy number")
}
#That being said, try not to use loops when you can use
#VECTORIZATION

i <- 1:20
j <- rep(NA,length(i))

#instead of...
for (n in i){
  j[i] <- i*11
}

#try...
j <- i*11

#instead of...
letters <- c("a","b", "c","d","e","f","g","h","i","j","k")
vowLets <- c()
vowels <- c("a","e","i","o","u")


for (let in letters){
  if (let %in% vowels){
    vowLets <- c(vowLets,TRUE) #WARNING: memory inefficient
  } else {
    vowLets <- c(vowLets, FALSE) #WARNING: memory inefficient
  }
}

#try...
vowLets2 <- ifelse(letters %in% vowels, TRUE, FALSE)
vowLets2
#lapply()-----------------------------------
#QUICK REVIEW: normal distribution
#for lists (and btw dfs are lists)
age <- round(rnorm(10,40,10)) #years
height <- round(rnorm(10,140,20)) #cm?
weight <- round(rnorm(10,170,35)) #lbs

people <- data.frame(age = age, height = height, weight = weight)
head(people)
lapply(people,mean)
peopleMAT <- as.matrix(people)
peopleMAT
apply(peopleMAT,2,mean)  #not a list
