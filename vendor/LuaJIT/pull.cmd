@echo off
if exist "luajit\" (
  cd luajit && git pull    
) else (
  git clone https://luajit.org/git/luajit.git
)