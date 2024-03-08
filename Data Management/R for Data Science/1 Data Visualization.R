#Sections 1.1-1.2
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
#Exercises 1.2.5
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
