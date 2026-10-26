@echo off
if exist "luajit\" (
  cd luajit
  git restore src/msvcbuild.bat
  git pull
  git apply ../msvcbuild-md-runtime.patch    
) else (
  git clone https://luajit.org/git/luajit.git
  cd luajit
  git apply ../msvcbuild-md-runtime.patch  
)