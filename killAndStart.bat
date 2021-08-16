@echo off  
 
echo 准备关闭程序。。。 
 
taskkill /f /im Setex.exe
 
echo 关闭成功
 
D: 
 
echo 准备启动程序1。。。
 
cd D:\DataToSqlServer\
 
start D:\DataToSqlServer\Setex.exe
 
echo 启动程序成功.
 
echo 退出本界面
exit