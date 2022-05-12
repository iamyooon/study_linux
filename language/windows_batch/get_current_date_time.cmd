@echo off
set currentdate=%date%
set currenttime=%time%
set currenttime=%currenttime: =%
echo %currentdate%_%currenttime%
:: below works!!
:: echo %date%_%time%
