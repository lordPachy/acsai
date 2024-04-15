####################

#                  #

#    Exercise 1    #

#                  #

####################

# Load the data.table package

library(data.table) 
# First install.packages("data.table")

# Load data with fread
getwd()
dir = "/Users/walter/Desktop/DATA/"
setwd(dir)
tc <- fread("toy_cor.csv")

head(tc)
####################

#                  #

#    Exercise 2    #

#                  #

####################
#Using one line of code print out the most common car model in the data, and the number of times it appears.
tc[, .N, by = Model][order(N)][.N]
##                                   Model   N
## 1: 1.6 16V HATCHB LINEA TERRA 2/3-Doors 109
# Or

tc[, .N, by = Model][which.max(N)]
##                                   Model   N
## 1: 1.6 16V HATCHB LINEA TERRA 2/3-Doors 109
####################

#                  #

#    Exercise 3    #

#                  #

####################
#Print out the mean and median price of the 10 most common models.

tc[, .(.N, medianPrice = median(Price), meanPrice = mean(Price)),
   by = Model][order(-N)][1:10]
##                                      Model   N medianPrice meanPrice
##  1:   1.6 16V HATCHB LINEA TERRA 2/3-Doors 109        8750  8578.440
##  2:   1.3 16V HATCHB LINEA TERRA 2/3-Doors  84        7950  8079.167
##  3:     1.6 16V LIFTB LINEA LUNA 4/5-Doors  80        9500  9454.312
##  4:    1.6 16V LIFTB LINEA TERRA 4/5-Doors  71        8750  8624.775
##  5:   1.4 16V VVT I HATCHB TERRA 2/3-Doors  54       10475 10448.704
##  6:    1.6 16V SEDAN LINEA TERRA 4/5-Doors  43        8250  8309.884
##  7:    1.6 16V VVT I LIFTB TERRA 4/5-Doors  37       11695 11514.324
##  8:      1.6 16V VVT I LIFTB SOL 4/5-Doors  35       11950 12131.143
##  9:    1.3 16V LIFTB LINEA TERRA 4/5-Doors  35        8250  8409.143
## 10: 1.6 16V WAGON LINEA TERRA Stationwagen  28        9225  9538.929
####################

#                  #

#    Exercise 4    #

#                  #

####################
#Delete all columns that have Guarantee in its name.

tc[, grep("Guarantee", names(tc)) := NULL]


####################

#                  #

#    Exercise 5    #

#                  #

####################
#Add a new column which is the squared deviation of price from the average price of cars the same color.

tc[, sq_dev_bycol := (Price - mean(Price))^2,  by = Color]


####################

#                  #

#    Exercise 6    #

#                  #

####################
#Use a combintation of .SDcols and lapply to get the mean value of columns 18 through 35

tc[, lapply(.SD, mean), .SDcols = 18:35]
##          ABS  Airbag_1  Airbag_2     Airco Automatic_airco Boardcomputer
## 1: 0.8133705 0.9707521 0.7228412 0.5083565      0.05640669     0.2945682
##    CD_Player Central_Lock Powered_Windows Power_Steering     Radio
## 1:  0.218663    0.5800836       0.5619777      0.9777159 0.1462396
##    Mistlamps Sport_Model Backseat_Divider Metallic_Rim Radio_cassette
## 1: 0.2569638   0.3001393         0.770195    0.2047354      0.1455432
##    Parking_Assistant   Tow_Bar
## 1:       0.002785515 0.2778552
####################

#                  #

#    Exercise 7    #

#                  #

####################
#Print the most common color by age in years?

tc[, .N,
   by = .(floor(Age_08_04/12), Color)][order(floor, N),
                                       .SD[.N],
                                       by = floor]
##    floor Color   N
## 1:     0  Grey  18
## 2:     1  Blue  29
## 3:     2  Grey  37
## 4:     3   Red  39
## 5:     4   Red  60
## 6:     5   Red 103
## 7:     6  Blue  71
####################

#                  #

#    Exercise 8    #

#                  #

####################
#For the dummy variables in columns 18:35 recode 0 to -1. You might want to use the set function here
for (j in 18:35) {
  set(tc,
      i = tc[, which(.SD == 0), .SDcols = j],
      j = j,
      value = -1)
}


####################

#                  #

#    Exercise 9    #

#                  #

####################
#Use the set function to add “yuck!” to the varible Fuel_Type if it is not petrol. Just because…
set(tc,
    i =  tc[, which(Fuel_Type == "Petrol")],
    j = "Fuel_Type",
    value = "Petrol yuck!")


####################

#                  #

#    Exercise 10   #

#                  #

####################
#Using .SDcols and one command create two new variables, log of Weight and Price.
tc[, (c("logWeight", "logPrice")) := lapply(.SD, log),
   .SDcols = c("Weight", "Price")]
