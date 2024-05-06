---
title: "IGraph intro"
format: html
editor: visual
---

# Importing stuff
```{r}
install.packages("igraph")
library(igraph)
```
```{r}
library(ggplot2)
ws_cc = numeric()
ws_av = numeric()
#fattori di normalizzazione
normcc <- transitivity(sample_smallworld(1, 100, 4, 0, loops = F, multiple = F))
normav <- average.path.length(sample_smallworld(1, 100, 4, 0, loops = F, multiple = F))
#clustering coefficient & av path length VS rewiring probability
for(p in seq(from = 0.0, to =  1, by =0.0001)){#small step size
  ws = sample_smallworld(1, 100, 4, p, loops = F, multiple = F)
  cc = transitivity(ws)
  av = average.path.length(ws)
  ws_cc = c(ws_cc, cc)
  ws_av = c(ws_av, av)
}

ggplot(data = ws_cc, mapping = aes(cc, ws_cc)) + geom_point() 
```


