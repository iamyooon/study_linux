@echo off
:: FOR /F "tokens=* USEBACKQ" %%F IN (`date /t`) DO ( <------ what is diffrent?
:: SET currentdate=%%F
:: )
for /F "tokens=* " %%F in ('date /t') do (set currentdate=%%F)
echo Original date's output ::::::::::::::::::::::::::::::::::::::::: %currentdate%
For /f "tokens=1-3 delims=/-" %%a in ('date /t') do (set currentdate_1=%%a%%b%%c)
echo Using delimiter '-' and select 1-3(1,2,3) field :::::::::::::::: %currentdate_1%
For /f "tokens=1,2,3 delims=/-" %%a in ('date /t') do (set currentdate_1=%%a%%b%%c)
echo Using delimiter '-' and select 1,2,3 field ::::::::::::::::::::: %currentdate_1%
For /f "tokens=1-3 delims=/-" %%a in ('date /t') do (set currentdate_1=%%a_%%b_%%c)
echo Using delimiter '-' and select 1-3(1,2,3) field add '-' :::::::: %currentdate_1%

echo =================================================================================
For /f "tokens=*" %%a in ('time /t') do (set currenttime_2=%%a)
echo Original time's output is :::::::::::::::::::::::::::::::::::::: %currenttime_2%
For /f "tokens=1,2 delims=/ " %%a in ('time /t') do (set currenttime_2=%%a and %%b)
echo Using delimeter ' ' and select 1,2 field add "and" ::::::::::::: %currenttime_2%

echo =================================================================================
echo Original date's output ::::::::::::::::::::::::::::::::::::::::: %currentdate%
set currentdate_2=%currentdate:~0,4%
echo Get filed 0~3(year) :::::::::::::::::::::::::::::::::::::::::::: %currentdate_2%
set currentdate_2=%currentdate:~5,2%
echo Get filed 5~6(month) ::::::::::::::::::::::::::::::::::::::::::: %currentdate_2%
set currentdate_2=%currentdate:~8,2%
echo Get filed 8~9(date) :::::::::::::::::::::::::::::::::::::::::::: %currentdate_2%
