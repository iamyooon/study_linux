@echo off

:: date command's result is like below.
:: 3:04:35.31
set currenttime=%TIME%
echo Original value is %currenttime%
set currenttime_1=%currenttime::=%
:: echo Replace ': -> $' %currenttime% <----- nor work with '>'
echo Remove ':' is %currenttime_1%

set currenttime_2=%currenttime::=-%
echo Replace ':' to '-' is %currenttime_2%

set currenttime_3=%currenttime: =%
echo Remove ' ' is %currenttime_3%
