# 森林火警系统数据收集程序  
Author : zhangxinhui02 [SHIINASAMA](https://github.com/SHIINASAMA)

这是一套能够自行收集温度数据至数据库的程序。展示数据并产生警报的前端页面由其他人着手，不在此仓库中。

## ForestFireWarningSystem  
Arduino端的数据收集装置的程序。主板会每隔5秒向串口输出温度数据（单位：摄氏度，两位小数）。  
数据格式为（为了方便分割出数据）：  
"#度数$"

例如：  
"#29.00$"

## 森林火警系统数据收集程序  
上位机的数据收集程序。将Arduino传来的串口数据读取出来，并将数据存入数据库，项目是C# WinForm项目。  

主要由[SHIINASAMA](https://github.com/SHIINASAMA)编写。

## Docs
Arduino的接线图。