#-------------------------------------------------------------------------
# S T A T I S T I C A    M U L T I V A R I A T A 
#-------------------------------------------------------------------------
# R :   L E Z I O N E   1 :  I N T R O D U Z I O N E   A   R          
#-------------------------------------------------------------------------

# scaricare ed installare R e Rstudio                                           
#-------------------------------------------------------------------------
#
# R. 
#    http://www.r-project.org/
#    selezionate mirror e sistema operativo 
# Rstudio. 
#    https://www.rstudio.com/
#    selezionate sistema operativo 

# impostare la directory di lavoro
#-------------------------------------------------------------------------
#
# dal menu File-->Cambia Directory --> selezionare X:\Giordani
# oppure...
setwd('/home/pachy/Desktop/ACSAI/acsai/Statistica Multivariata/Class/Introduzione_SM')
getwd()
dir()

# aspetti introduttivi
#-------------------------------------------------------------------------

# Calcolatrice
2+2
# Risultato in un oggetto
ls()                                     
a
a = 2+2
a
# al posto di = 
# <- O -> (assegnazione)
# [equivalenti]
aa <- 3*2
8/3 -> aaa

# Altre operazioni numeriche
help(Arithmetic)
?Arithmetic
help(Sqrt)
help.search('Sqrt')
??Sqrt
?abs

# Le entita' piu' semplici che possono costituire un oggetto di R
# (a cui assegnare un nome) sono elementi scalari 
# (numerici {[interi, reali] o complessi})                              
a <- 10
b <- 15.53
c <- 3.51+1.2i
# - elementi stringhe di caratteri (usare le virgolette)
d <- "Ciao"
e <- "Paolo"
# - elementi logici
f <- true
f <- TRUE
g <- FALSE
h <- F
i <- T

# per controllare gli oggetti creati
ls()
# per cancellare oggetti
rm(aa,aaa)
# per cancellare tutti gli oggetti: rm(list=ls())

# terminiamo qui la prima sessione e vediamo come lasciare traccia 
# permanente dei comandi impartiti e degli oggetti ottenuti                 
# rispondendo SI alla richiesta di salvataggio dell'area di
# lavoro (workspace)

# Ripartiamo dalla sessione precedente andando a pescare
# il file .RData dalla directory opportuna oppure       
# eseguendo tutti i comandi precedenti memorizzati nel file .Rhistory            

# ovviamente e' possibile salvare il workspace o la history in qualunque momento
savehistory("prima-interruzione.txt")
save.image("prima-interruzione.RData")
# oppure salvare solo alcuni elementi...
save(a,file = "solo-a.RData")

# tutto questo viene salvato nella directory di lavoro;
# se chiudiamo Rstudio e vogliamo caricare il workspace precedente
load("prima-interruzione.RData")

# oggetti VECTOR
#-------------------------------------------------------------------------

c(1,2,3)
vett1 <- c("a","b","c")
vett2 <- c(f,g,h,i)

# vediamo di che TIPO (mode) sono i vettori precedenti
mode(vett1)
mode(vett2)
# possiamo anche chiedere se sono di un particolare tipo
is.logical(vett1)
is.character(vett1)
is.numeric(vett2)
is.logical(vett2)
# possiamo poi cambiare il tipo di vettore
vett3 <- vett2
mode(vett3)
vett3 <- as.numeric(vett2)
mode(vett3)
# o analogamente
vett3 <- vett2
mode(vett3)
mode(vett3) <- "numeric"
mode(vett3)

# vettori vuoti
vuoto1 <- c()
vuoto1
vuoto2 <- vector(length=0,mode="numeric")
vuoto2
vuoto3 <- vector(length=0,mode="character")
vuoto3

# vettori di tipo 'misto' (non e' pero' possibile)
vett4 <- c(3,"4")
mode(vett4)
vett5 <- c(3,T)
mode(vett5)
vett6 <- c("T",T)
mode(vett6)

# vediamo alcune operazioni con i vettori (numeric)
vett <- c(1,2,3,4,5)
length(vett)
sum(vett)
prod(vett)
min(vett)
max(vett)
cumsum(vett)
cumprod(vett)
diff(cumprod(vett))
mean(vett)
median(vett)
sd(vett)
var(vett)
vett+vett
vett*vett
vett/vett
vett*5
vett*c(5)
vett*c(5,5,5,5,5)
vett*c(1,2)
vett*c(1,2,1,2,1)

# vediamo alcune operazioni con i vettori (logical)
# NOT (Negazione) [usando il punto esclamativo !]
!TRUE
# AND (Intersezione) [usando la E-commerciale &]
TRUE & FALSE
TRUE & TRUE
c(T,T,F,F)&c(T,F,T,F)
# OR (Unione) [usando la barra verticale |]
TRUE | FALSE
c(T,T,F,F)|c(T,F,T,F)
vettore.logico <- c(T,T,F,F,T)
vettore.logico
any(vettore.logico)
all(vettore.logico)
which(vettore.logico)
# inseriamo una condizione
any(vett>mean(vett)+sd(vett))
all(vett>mean(vett)+sd(vett))
which(vett>mean(vett)+sd(vett))
# per altre informazioni sugli operatori logici
help(Logic)

# Operazioni su stringhe
?paste
paste("Ro","ma")
paste("Ro","ma",sep="")
paste("Roma","Milano","Torino",sep="-")
paste(c("Roma","Milano","Torino"),"Palermo",sep="-")

# altre operazioni con vettori
# Funzione seq()
b0 <- seq(from=1,to=10)
b0
b1 <- seq(from=1,to=10,by=1)
b1
b2 <- seq(from=0,to=9.9,by=0.1)
b2
# ...alternativamente...
b3 <- 1:10
b3
# attenzione...
b3bis <- 1:10+1
b3bis
b3ter <- 1:(10+1)
b3ter
# Funzione rep()
b4 <- rep(x=10,times=100)
b4
b5 <- rep(x=c(1,2),times=10)
b5

# altre info:
# - 0 e forme indeterminate
zero
as.numeric("zero")
0/4
4/0
0/0
# NaN vuol dire Not a Number (forme indeterminate)
# - dati mancanti
# in R rappresentati da NA (Not Available)
# per scovare dove sono i dati mancanti
is.na(c(1,2,3,NA,4))
!is.na(c(1,2,3,NA,4))
vett7 <- c(1,2,3,NA,4)
any(is.na(vett7))
all(is.na(vett7))
which(is.na(vett7))

# Esercizio 1.1
#-------------------------------------------------------------------------
# - Creare un vettore "v" di lunghezza 1000 generato casualmente da N(0,1)
# (Suggerimento: vedi funzione "rnorm")
# - Calcolare media, mediana e sd
# - Quali sono le posizioni dei valori maggiori di 1? E maggiori della media?
# - Esiste almeno un valore maggiore 2.5?
# - Tutti i valori sono minori di 3 in valore assoluto?

# oggetti MATRIX
#-------------------------------------------------------------------------

mat1 <- matrix(data=0,nrow=6,ncol=4)
mat1
mode(mat1)
class(mat1)
# pi? semplicemente
mat1bis <- matrix(0,nrow=6,ncol=4)
mat1bis
mat2 <- matrix(1:24,nrow=6,ncol=4)
mat2
mat3 <- matrix(1:24,nrow=6,byrow=T)
# attenzione...
mat3bis <- matrix(1:24,6,T)
mat4 <- matrix(c(1,2,3,4,5,6),nrow=6,ncol=4)
dim(mat4)
nrow(mat4)
ncol(mat4)
mat5 <- matrix(c(TRUE,TRUE,FALSE),nrow=3,ncol=5,byrow=T)
mat5
mode(mat5)
class(mat5)
mat2==1:6

# vediamo alcune operazioni con le matrici
mat2
apply(X=mat2,MARGIN=1,FUN=sum)
apply(mat2,2,sum)
apply(mat2,2,prod)
?apply
apply(mat2,2,mean)
apply(mat2,2,sd)
apply(mat2,2,var)
mean(mat2)
sd(mat2)
var(mat2)
mat7 <- mat2[c(1,4),1:4]
mat7
mat8 <- mat2[1:4,]
mat8
mat9 <- mat4%*%mat8
mat4*mat8
mat4bis <- mat4[1:4,]
mat4bis*mat8
mat4bis%*%mat8
t(mat2)
mat10 <- diag(c(1,2,3,4))
mat10
mat10 <- diag(1:4)
mat10
# Attenzione...
v <- c(1,4)
mat10 <- diag(v)
mat10
v <- c(4)
mat10 <- diag(v)
mat10
mat10 <- diag(mat8)
mat10
# matrice identita'
mat.ident <- diag(1,nrow=4)
mat.ident
mat11 <- matrix(2:5,ncol=2)
mat11
det(mat11)
mat11^2
mat11^-1
solve(mat11)
solve(mat11)%*%mat11
#traccia
sum(diag(mat11))
#norma (al quadrato)
sum((mat11)^2)

# Esercizio 1.2
#-------------------------------------------------------------------------
# Usando mat11, calcoliamo la differenza tra il prodotto 
# della somma per riga e il prodotto della somma per colonna

# Esercizio 1.3
#-------------------------------------------------------------------------
# Come possiamo verificare se esiste una riga
# uguale a c(3,9,15,21) in mat2?

# oggetti LIST
#-------------------------------------------------------------------------

# gli oggetti di *mode* "list"
vettore1 <- 3
vettore2 <- c(T,F,T,F)
vettore3 <- c("Ciao","Paolo")
lista <- list(vettore3,vettore1,vettore2)
mode(lista)
# Nella lista si possono mettere assieme oggetti, 
# anche complicati, di tipo diverso
# non ? cos? per le matrici
help(cbind) # rbind()
matrice <- cbind(vettore3,vettore1,vettore2)
mode(matrice)

# oggetti DATA.FRAME
#-------------------------------------------------------------------------

# servono per combinare dati di natura diversa in  
# una struttura simile a quella di una matrice 
# (trattazione pi? semplice rispetto ad una lista)

nome <- c("Paolo","Sara","Matteo","Sofia")
eta <- c(41,41,9,6)
sesso <- c("M","F","M","F")
vaccino <- c(F,F,T,T)
dati <- data.frame(nome,eta,sesso,vaccino)
dati
str(dati)
dim(dati)
rownames(dati)
colnames(dati)
dimnames(dati)

# Ordinamenti (vettori, matrici, data.frame)
# iniziamo da <nome>
nome        
mode(nome)
sort(x=nome)
sort(x=nome,decreasing=T)
# proseguiamo con <eta>
eta
mode(eta)
sort(eta)
sort(eta,decreasing=T)
# chiudiamo con <vaccino>
vaccino
mode(vaccino)
sort(vaccino)
# differenza tra <sort> e <order>
sort(nome)
order(nome)
sesso
order(sesso)        
# per ordinare *prima* rispetto a <sesso>, *poi* a <eta>
order(sesso,eta)    

# <order> e' molto utile per riordinare anche le matrici o i data.frame
dati[order(sesso),]
dati[order(sesso,eta),]
# ancora un'altra funzione che riorganizza *invertendo* l'ordine degli elementi
nome
rev(nome)

# Operazioni su nomi di righe e colonne (vettori, matrici, data.frame)
#-------------------------------------------------------------------------

# modifichiamo i dati in modo tale che la variabile nome fornisca 
# il nome delle righe di dati e non ne sia pi? una colonna. Poi
# 'miglioriamo' i nomi delle colonne di dati. 
dati <- dati[,2:4]
rownames(dati) <- nome
colnames(dati) <- c("Et?","Sesso","Vaccino")
dati
nrow(dati)
ncol(dati)
dim(dati)
dimnames(dati)

# nota: assegnare nomi in maniera automatica (vettori, matrici, data.frame)
#-------------------------------------------------------------------------

#vettore
vettore <- c(2,5,3,8,4,6)
vettore
names(vettore) <- paste("E",1:6)
vettore
#matrice
matrice <- matrix(c(2,5,3,8,4,6),nrow=2)
matrice
rownames(matrice) <- c("R 1","R 2")
colnames(matrice) <- paste("C",1:3,sep="")
matrice
#data.frame
dati
rownames(dati) <- paste("unit? n.",1:nrow(dati),sep="")
colnames(dati) <- paste("Var",1:dim(dati)[2])
dati

# nota: conversione di data.frame in matrix (e viceversa)
#-------------------------------------------------------------------------

mat2 <- matrix(1:24,nrow=6,ncol=4)
mat2
mat3 <- matrix(1:24,nrow=6,byrow=T)
mat3
prod <- mat2%*%t(mat3)
prod
is.matrix(mat2)
is.matrix(mat3)
is.data.frame(mat2)
is.data.frame(mat3)
mat2.df <- as.data.frame(mat2)
mat3.df <- as.data.frame(mat3)
is.matrix(mat2.df)
is.matrix(mat3.df)
is.data.frame(mat2.df)
is.data.frame(mat3.df)
prod.df <- mat2.df%*%t(mat3.df)
mat2.df <- as.matrix(mat2)
mat3.df <- as.matrix(mat3)
prod.df <- mat2.df%*%t(mat3.df)
prod.df

# estrazioni di parti di un oggetto
#-------------------------------------------------------------------------

# Carichiamo dei dataset
ls()
load("Introduzione.RData")
ls()
str(dati.apat)
vett <- dati.apat[277:326,"temp"]
vett
vett[]
vett[1]
vett[c(1,2,3,4,5,6,7,8,9,10)]
vett[c(1,50,30,1,1,50,50)]
# per l'estrazione di parti sono molto utili le seguenti istruzioni
vett
vett>18
vett[vett>18]
vett[-1]
vett[-(1:25)]
vett[-length(vett)]
vett[!is.na(vett)]
vett[complete.cases(vett)]
# dati.apat
dim(dati.apat)
dati.apat[5,2]
dati.apat[5,1:2]
dati.apat[5,]
dati.apat[dati.apat[,1]=="Gen-2000",]
which(dati.apat[,1]=="Gen-2000")

# Esercizio 1.3
# Troviamo quale ? la massima temperatura registrata
# e il giorno in cui si ? verificata

# istruzione attach 
#-------------------------------------------------------------------------

str(dati.apat)
names(dati.apat)
Data[1:4]
# non restituisce nulla
attach(dati.apat)
Data[1:4]
temp[1:10]
detach(dati.apat)
Data[1:4]

# oggetti di classe factor
#-------------------------------------------------------------------------

str(telefonate)
telefonate$Tipologia
tip.fact <- factor(telefonate$Tipologia)
tip.fact
mode(tip.fact)
tip.fact*2
class(tip.fact)
levels(tip.fact)
c(tip.fact)
# vediamo come ricodificare un dato categoriale (o meglio un "factor"), 
# cio? un vettore di dati a cui e' stata assegnata la classe "factor"
table(tip.fact)
tip.fact.ricodificato <- tip.fact
levels(tip.fact.ricodificato)
levels(tip.fact.ricodificato) <- c("Fonia Nazionale","Altro","Altro","Altro","Altro","Altro","Altro")
levels(tip.fact.ricodificato)
tip.fact.ricodificato
c(tip.fact.ricodificato)
table(tip.fact.ricodificato)

giudizio <- c("buono","sufficiente","buono","mediocre","ottimo","sufficiente")
giudizio.fact <- factor(giudizio,levels=c("mediocre","sufficiente","buono","ottimo"))
class(giudizio.fact)
giudizio.fact
levels(giudizio.fact)
c(giudizio.fact)
# vediamo i due ordinamenti degli oggetti
# contenenti le stesse informazioni ma
# organizzati diversamente
# 1) vettore di modo character
# 2) vettore di modo "numeric" e di classe "factor"
sort(giudizio)
sort(giudizio.fact)

# Esercizio 1.4
#-------------------------------------------------------------------------
# In quale mese si e' registrato il massimo costo mediano delle chiamate
#(Nota: costo in "Costo..Euro."; chiamate in "Descrizione" modalita' "Chiamata ")
telefonate$Data.e.ora <- as.Date(substr(telefonate$Data.e.ora,1,10),"%d/%m/%Y")
telefonate$mese <- months(telefonate$Data.e.ora)

# (Alcuni) grafici in R
#-------------------------------------------------------------------------

# Grafico a torta
?pie
pie(tip.fact.ricodificato)
pie(table(tip.fact.ricodificato))
pie(table(tip.fact.ricodificato),names(table(tip.fact.ricodificato)))

# Grafico a barre
?barplot
barplot(table(tip.fact))
barplot(table(tip.fact),cex.axis=0.8,cex.names=0.8)

# Istogramma
?hist
costo <- telefonate$Costo..Euro.
hist(x=costo)
hist(costo,freq=FALSE)
hist(costo,breaks="sturges",main='Costo delle telefonate (Euro)')
# anche breaks="scott" o breaks="fd"
hist(costo,breaks=3,col=1:3)
hist(costo,breaks=2)
min(costo)
max(costo)
quantile(costo,probs=c(0,0.01,0.05,0.1, 0.2, 0.3, 0.5,0.7,1))
hist(costo,breaks=c(0,0.10,0.20,0.50,2.0,5.0),main='Costo delle telefonate (Euro)')
hist(costo,breaks=c(0,0.10,0.20,0.50,2.0,5.0),main='Costo delle telefonate (Euro)',col=c("white","gray60","gray45","gray30","gray15"))
hist(costo,breaks=c(0,0.10,0.20,0.50,2.0,5.0), main='Costo delle telefonate (Euro)',freq=TRUE)

# Input/Output di dati
#-------------------------------------------------------------------------

#  leggere dati da immettere in vettori o liste da file esterni oppure
#  dalla 'console' o linea di comando

x <- scan()
1 2 3    4
7 32 1
21    3

# premendo Invio per due volte di seguito
# e' equivalente a scrivere
# x <- c(1,2,3,4,7,32,1,21,3)

# Input
# il caso pi? comune invece consiste nel caricare un file dati
# eventualmente specifando dove dobbiamo prendere i dati
# setwd("...")  NB: nel selezionare la directory usare / 
# sono utili i seguenti comandi 
#     read.table(...)
#     read.csv(...)
#     read.xls(...)  in library(gdata)
#     read.xls(...)  in library(xlsReadWrite)

w1 <- read.table(file="provastringhe1.txt")
w1
colnames(w1) <- c("Nome","Et?","Sesso","Vaccino")
w1
w2 <- read.table("provastringhe2.txt")
w2
# qualcosa non va
?read.table
w2 <- read.table("provastringhe2.txt",sep=",",header=TRUE)
w2

# Output
# per creare un file di tipo .txt
write.table(x=w1,file="w1a.txt")
write.table(w1,"w1b.txt",sep=",")
# da caricare con
read.table("w1a.txt")
read.table("w1b.txt")
# la scelta del separatore crea problemi risolvibili con
read.table("w1b.txt",sep=",",header=TRUE)
# per creare un file di tipo .csv
write.table(w2,"w2.csv",sep=",")
# da caricare con
read.csv("w2.csv")

# se abbiamo dimenticato dove stiamo salvando
getwd()