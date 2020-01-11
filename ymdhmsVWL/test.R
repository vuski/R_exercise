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

df=tibble(v1=rep("20191231",180*10),v6=rep("042355",180*10))
dt=as.data.table(df)

res=microbenchmark::microbenchmark(
  df1=
    df%>%mutate(et=as.numeric(as.POSIXct(paste0(v1,v6),format="%Y%m%d%H%M%S"))),
  
  df2=
    df%>%mutate(et=as.numeric(ymd_hms(paste0(v1,v6)))),
  
  df5=
    df%>%mutate(et=ymd_hms(paste0(df$v1,df$v6))),
  
  dt1=
    dt[,et:=as.numeric(ymd_hms(paste0(v1,v6)))],
  
  dt2=
    dt[,et:=as.numeric(anytime(paste0(v1,v6)))],
  
  dt9=
    df%>% mutate(et = ymdhmsVWL(v1, v6))
  
)

ggplot2::autoplot(res)

