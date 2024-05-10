setwd('/home/pachy/Desktop/ACSAI/acsai/Data Management/Class/networks_igraph/scripts')

load("data/posts_disneypixar.rdata")
load("data/likes_disneypixar.rdata")
load("data/comments_disneypixar.rdata")


head(posts)
head(likes)
head(comments)


library(igraph)

edges = data.frame("from" = likes$post_id, "to" = likes$from_id, stringsAsFactors = F )
g <- graph.data.frame(edges, directed = F)
V(g)$type <- V(g)$name %in% edges[,2]
#313077 nodes are LIKERS - node marked as TRUE - length(V(g)[type == "TRUE"])
#576 nodes are POSTS - node marked as FALSE  length(V(g)[type == "FALSE"])
g_like = g  #from now on g_like is the bipartite for likes - posts

edges = data.frame("from" = comments$post_id[1:5000], "to" = comments$from_id[1:5000], stringsAsFactors = F )
g <- graph.data.frame(edges, directed = F)
V(g)$type <- V(g)$name %in% edges[,2]
#40918 nodes are COMMENTS - node marked as TRUE - length(V(g)[type == "TRUE"])
#573 nodes are POSTS - node marked as FALSE  length(V(g)[type == "FALSE"])
g_comm = g #from now on g_comm is the bipartite for comments - posts


g = g_comm

#USE TO SEE THE BIPARTITE (TIME CONSUMING)
V(g)$color <- V(g)$type
V(g)$color=gsub("FALSE","red",V(g)$color)
V(g)$color=gsub("TRUE","blue",V(g)$color)
pdf("data/TEST_BIPARTITE.pdf")
V(g)$color <- ifelse(V(g)$type, "lightblue", "salmon")
V(g)$shape <- ifelse(V(g)$type, "circle", "square")
E(g)$color <- "lightgray"
plot(g, vertex.label.cex = 0.2, 
     vertex.label.color = "black",
     vertex.label = "NA",
     edge.width=0.5,
     edges.size = 0.1,
     vertex.size = 0.1,
     edge.lty = 2,
     edge.curved = TRUE,
     layout=layout_as_bipartite)
#plot(g, edge.color="gray30",edge.width=E(g)$weight, layout=layout_as_bipartite,
     #vertex.label = "NA")
#grid(col = "dodgerblue4")
dev.off()

#ANALYZING THE BIPARTITE: some metrics
types <- V(g)$type                 ## getting each vertex `type` let's us sort easily
deg <- degree(g)
bet <- betweenness(g)
clos <- closeness(g)
eig <- eigen_centrality(g)$vector
cent_df <- data.frame(types, deg, bet, clos, eig)
cent_df[order(cent_df$type, decreasing = TRUE),] ## sort w/ `order` by `type`


#PLOT PROBABILITY DENSITY FUNCTION OF BET OR CLOS OR EIG
plot(density(bet), log = "xy",lty = 6)
plot(density(clos), log = "xy",lty = 6)
plot(density(eig), log = "xy",lty = 6)

#JUST IN CASE YOU WANT TO PLOT IT (WITH NODE SIZE PROPORTIONAL TO THE DEGREE)
pdf("data/TEST_BIPARTITE_1.pdf")
V(g)$color <- ifelse(V(g)$type, "lightblue", "salmon")
V(g)$shape <- ifelse(V(g)$type, "circle", "square")
V(g)$size <- degree(g)
V(g)$label.cex <- degree(g) * 0.2
E(g)$color <- "lightgray"
plot(g, vertex.label.cex = 0.2, 
     vertex.label.color = "black",
     vertex.label = "NA",
     edge.width=0.5,
     edges.size = 0.1,
     edge.lty = 2,
     edge.curved = TRUE,
     layout=layout_as_bipartite)
#plot(g, edge.color="gray30",edge.width=E(g)$weight, layout=layout_as_bipartite,
#vertex.label = "NA")
#grid(col = "dodgerblue4")
dev.off()


#PROJECTING ON PAGES/USERS
# ON ROWS Arow <- A %*% t(A)
# ON COLUNMS Acol <- t(A) %*% A
bipartite_matrix <- as_incidence_matrix(g_comm) #use g_like for the like-post network

post_matrix = bipartite_matrix %*% t(bipartite_matrix)
comments_matrix_prod = t(bipartite_matrix) %*% bipartite_matrix
diag(post_matrix) <- 0
diag(comments_matrix_prod) <- 0

g_post_c = graph_from_adjacency_matrix(post_matrix, 
                            mode = "undirected", 
                            weighted = TRUE)

g_post_c = simplify(g_post_c)
pdf("POST_PROJECTION_COMMENTS.pdf")
V(g_post_c)$size <- degree(g_post_c)
V(g_post_c)$label.cex <- degree(g_post_c) * 0.2
E(g_post_c)$color <- "lightgray"
plot(g_post_c, vertex.label.cex = 0.2, 
     vertex.label.color = "black",
     vertex.label = NA,
     edge.width=0.5,
     edges.size = 0.1,
     edge.lty = 2,
     vertex.size = 0.8,
     edge.curved = TRUE,
     layout=layout.circle(g_post_c)
     )
dev.off()



library("poweRlaw")
library("ggplot2")

g_post_c.degrees <- degree(g_post_c)
# Let's count the frequencies of each degree
g_post_c.degree.histogram <- as.data.frame(table(g_post_c.degrees))
# Need to convert the first column to numbers, otherwise
# the log-log thing will not work (that's fair...)
g_post_c.degree.histogram[,1] <- as.numeric(g_post_c.degree.histogram[,1])
# Now, plot it!
ggplot(g_post_c.degree.histogram, aes(x = g_post_c.degrees, y = Freq)) +
  geom_point() +
  scale_x_continuous("Degree\n(nodes with this amount of connections)",
                     breaks = c(1, 3, 10, 30, 100, 300),
                     trans = "log10") +
  scale_y_continuous("Frequency\n(how many of them)",
                     breaks = c(1, 3, 10, 30, 100, 300, 1000),
                     trans = "log10") +
  ggtitle("Degree Distribution (log-log)") +
  theme_bw()


#COMMUNITIES
cl_gc = cluster_fast_greedy(g_post_c)

plot(cl_gc, g_post_c, 
     vertex.label = NA,
     vertex.size = 6,
     edge.width=0.1,
     edges.size = 0.1,
     edge.lty = 1,
     edge.curved = TRUE,
     layout = layout_with_fr(g_post_c)
     #layout=layout.circle(g_post_c)
     )


#########
cluster.distribution(g_post_c, cumulative = F)
V(g_post_c)$degree = degree(g_post_c)

filter_gc = subgraph(g_post_c, V(g_post_c)$degree>2)
filter_gc = induced.subgraph(g_post_c, V(g_post_c)$degree>40)
length(V(filter_gc))
