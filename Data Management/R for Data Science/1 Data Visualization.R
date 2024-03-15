#SECTIONS 1.1-1.2
install.packages("tidyverse")
library(tidyverse)
library(palmerpenguins)
library(ggthemes)
penguins
glipse(penguins)
glimpse(penguins)
?penguins
ggplot(data = penguins,
       mapping = aes(x = flipper_length_mm, y = body_mass_g)) + geom_point(mapping = aes(color = species, shape = species)) + geom_smooth(method = "lm") + labs(
         title = "Body mass and flipper length",
         subtitle = "Dimensions for Adelie, Chinstrap, and Gentoo Penguins",
         x = "Flipper length (mm)", y = "Body mass (g)",
         color = "Species", shape = "Species"
       ) + scale_color_colorblind()
#EXERCISES 1.2.5
#Exercise 3
ggplot(data = penguins,
       mapping = aes(x = bill_length_mm, y = bill_depth_mm, color = species, shape = species)) + geom_point() + geom_smooth(method = "lm") + labs(
         title = "Bill depth vs length",
         subtitle = "Dimensions for Adelie, Chinstrap, and Gentoo Penguins",
         x = "Bill length (mm)", y = "Bill depth (mm)",
         color = "Species", shape = "Species"
       ) + scale_color_colorblind()
#Exercise 4
ggplot(data = penguins,
       mapping = aes(x = bill_depth_mm, color = species)) + geom_histogram(binwidth = 0.01)  + labs(
         title = "Bill depth vs species",
         subtitle = "Dimensions for Adelie, Chinstrap, and Gentoo Penguins",
         y = "Bill depth (mm)", x = "Species",
       )
#Exercise 5
ggplot(data = penguins) + geom_point()
#missing variables
#Exercise 6
ggplot(data = penguins,
       mapping = aes(x = flipper_length_mm, y = body_mass_g)) + geom_point(mapping = aes(color = species, shape = species), na.rm = TRUE) + geom_smooth(method = "lm") + labs(
         title = "Body mass and flipper length",
         subtitle = "Dimensions for Adelie, Chinstrap, and Gentoo Penguins",
         x = "Flipper length (mm)", y = "Body mass (g)",
         color = "Species", shape = "Species"
       ) + scale_color_colorblind()
#Exercise 7
ggplot(data = penguins,
       mapping = aes(x = flipper_length_mm, y = body_mass_g)) + geom_point(mapping = aes(color = species, shape = species), na.rm = TRUE) + geom_smooth(method = "lm") + labs(
         title = "Body mass and flipper length",
         subtitle = "Dimensions for Adelie, Chinstrap, and Gentoo Penguins",
         x = "Flipper length (mm)", y = "Body mass (g)",
         color = "Species", shape = "Species",
         caption = "Data come from the palmerpenguins package"
       ) + scale_color_colorblind()
#Exercise 8
ggplot(data = penguins,
       mapping = aes(x = flipper_length_mm, y = body_mass_g)) + geom_point(mapping = aes(color = bill_depth_mm)) + geom_smooth()
#Exercise 9
ggplot(data = penguins,
       mapping = aes(x = flipper_length_mm, y = body_mass_g, color = island)) + geom_point() + geom_smooth(se = FALSE)
#Exercise 10
ggplot(
  data = penguins,
  mapping = aes(x = flipper_length_mm, y = body_mass_g)
) +
  geom_point() +
  geom_smooth()

ggplot() +
  geom_point(
    data = penguins,
    mapping = aes(x = flipper_length_mm, y = body_mass_g)
  ) +
  geom_smooth(
    data = penguins,
    mapping = aes(x = flipper_length_mm, y = body_mass_g)
  )
#SECTIONS 1.3-1.4
#Stripping down ggplot syntax
ggplot(penguins, aes(x= flipper_length_mm, y=body_mass_g)) + geom_point()
penguins |> ggplot(aes(x = flipper_length_mm, y = body_mass_g)) + geom_point()
#Bar charts
ggplot(penguins, aes(x=species)) + geom_bar()
#To have it ordered on the base of frequency, it is possible to transform to a factor:
ggplot(penguins, aes(x=fct_infreq(species))) + geom_bar()
#Histograms
ggplot(penguins, aes(x = body_mass_g)) + geom_histogram(binwidth = 200) #binwidth reunites the domain of body_mass_g into strips
#Density plots
ggplot(penguins, aes(x = body_mass_g)) + geom_density()
#EXERCISES 1.4.3
#Exercise 1
ggplot(penguins, aes(y = species)) + geom_bar()
#Exercise 2
ggplot(penguins, aes(x = species)) +
  geom_bar(color = "red")

ggplot(penguins, aes(x = species)) +
  geom_bar(fill = "red")
#Exercise 3
ggplot(penguins, aes(x = body_mass_g)) + geom_histogram(bins = 1)
#Exercise 4
library(tidyverse)
summary(diamonds)
ggplot(diamonds, aes(x = carat)) + geom_histogram(binwidth = 0.5)
#SECTIONS 1.5-
#Boxplots
ggplot(penguins, aes(x = species, y = body_mass_g)) + geom_boxplot()
#Density plots with colours
ggplot(penguins, aes(x = body_mass_g, color = species)) + geom_density(linewidth = 0.75)
#Density plots filled and with transparency
ggplot(penguins, aes(x = body_mass_g, color = species, fill=species)) + geom_density(alpha = 0.5)
#Stacked bar plots
ggplot(penguins, aes(x = island, fill = species)) + geom_bar()
ggplot(penguins, aes(x = island, fill = species)) + geom_bar(position = "fill")
#Three variables in a graph
ggplot(penguins, aes(x = flipper_length_mm, y = body_mass_g)) +
  geom_point(aes(color = species, shape = island))
ggplot(penguins, aes(x = flipper_length_mm, y = body_mass_g, color = species, shape = island)) +
  geom_point()
#Facets
ggplot(penguins, aes(x = flipper_length_mm, y = body_mass_g)) + geom_point(aes(color = species, shape = species)) + facet_wrap(~island)   #the argument of facet_wrap must be a formula, and in particular it needs to be categorical

#EXERCISES 1.5.5
#Exercise 1
library(ggplot2)
head(mpg)
#Exercise 2
ggplot(mpg, aes(x = displ, y = hwy)) + geom_point()
ggplot(mpg, aes(x = displ, y = hwy)) + geom_point(aes(color = cty))
ggplot(mpg, aes(x = displ, y = hwy)) + geom_point(aes(size = cty))
ggplot(mpg, aes(x = displ, y = hwy)) + geom_point(aes(size = cty, color = cty))
ggplot(mpg, aes(x = displ, y = hwy)) + geom_point(aes(shape = drv))
#Exercise 3
ggplot(mpg, aes(x = displ, y = hwy, linewidth = cty)) + geom_point() + geom_smooth(method = "lm")
#Exercise 5
ggplot(penguins, aes(y = bill_depth_mm, x = bill_length_mm)) + geom_point(aes(color = species))
ggplot(penguins, aes(y = bill_depth_mm, x = bill_length_mm)) + geom_point(aes(color = species)) + facet_wrap(~species)
#Exercise 6
ggplot(
  data = penguins,
  mapping = aes(
    x = bill_length_mm, y = bill_depth_mm, 
    color = species, shape = species
  )
) +
  geom_point() +
  labs(color = "Species", shape = "Species")
#Exercise 7
ggplot(penguins, aes(x = island, fill = species)) +
  geom_bar(position = "fill")
ggplot(penguins, aes(x = species, fill = island)) +
  geom_bar(position = "fill")
#SECTION 1.6
ggsave(filename = "penguin-plot.png")
getwd()
setwd("home/pachy/Desktop/ACSAI/acsai/Data Management")
getwd()
setwd("Desktop/ACSAI/acsai/Data Management")
setwd("R for Data Science")
ggplot(penguins, aes(x = island, fill = species)) +
  geom_bar(position = "fill")
ggsave(filename = "penguing-plot.pdf", )
