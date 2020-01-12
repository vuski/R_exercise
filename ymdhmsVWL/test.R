install.packages("tidyverse")
library(tidyverse)
library(ggplot2)
library(tidyverse)
library(lubridate)
library(data.table)
library(purrr)
library(anytime) # install.packages("anytime",repos="http://cran.rstudio.com/")

library(Rcpp)
sourceCpp("ymdhmsVWL.cpp")

samples1 <- as.character(Sys.time()-sample(1e7, 1e5, TRUE)) #random dates
df <- as.data.frame(samples1)
dt <- as.data.table(samples1)
res=microbenchmark::microbenchmark(
  POSIXct_df =
    df %>% mutate( et=as.numeric(as.POSIXct(samples1,format="%Y-%m-%d %H:%M%S"))),
  
  ymd_hms_df =
    df %>% mutate(et=as.numeric(ymd_hms(samples1))),
  
  ymd_hms_dt=
    dt[,et:=as.numeric(ymd_hms(samples1))],
  
  anytime_dt =
    dt[,et:=as.numeric(anytime(samples1))],
  
  ymdhmsVWL_df =
    df %>% mutate(et = ymdhmsVWL(samples1))
  
)

ggplot2::autoplot(res)
