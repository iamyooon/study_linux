@echo off
setlocal enabledelayedexpansion

set i=0
for %%A in (%*) do (
	set /a i+=1
	echo %%A
	set var!i!=%%A
)
echo method 1 %var1%
echo method 1 %var2%

set xxx=%1
set yyy=%2
echo method 2 %xxx%
echo method 2 %yyy%

