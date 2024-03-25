install.packages("RSelenium")
install.packages("rvest")
install.packages("tidyverse")
library(RSelenium)
library(rvest)
library(tidyverse)
library(wdman)
binman::rm_platform("phantomjs")
selenium(retcommand = T)

rd = rsDriver(browser = "firefox", port = 4545L, verbose = T)
remdr = rd[["client"]]
