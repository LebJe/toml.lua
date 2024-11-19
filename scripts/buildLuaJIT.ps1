# MinGW must be installed: `choco install mingw`

param(
	# Where LuaJIT should be installed
	[string]$installDir="$($pwd.Path)\LuaJIT"
)

if (-not (Test-Path -Path $installDir)) {
    mkdir $installDir
}

$installDir = Resolve-Path $installDir

New-Item -Path $installDir -ItemType "directory" -Name "bin"
New-Item -Path $installDir -ItemType "directory" -Name "lib"
New-Item -Path $installDir -ItemType "directory" -Name "include"
New-Item -Path $installDir -ItemType "directory" -Name "share"
New-Item -Path $installDir -ItemType "directory" -Name "jit"

Invoke-WebRequest -Uri "https://github.com/LuaJIT/LuaJIT/archive/refs/tags/v2.1.ROLLING.tar.gz" -OutFile "LuaJIT.tar.gz"
tar -xzvf "LuaJIT.tar.gz"
Remove-Item "LuaJIT.tar.gz"
Set-Location "LuaJIT-2.1.ROLLING"

mingw32-make.exe

Copy-Item "src\luaconf.h" -Destination "$($installDir)\include"
Copy-Item "src\lua.h" -Destination "$($installDir)\include"
Copy-Item "src\lualib.h" -Destination "$($installDir)\include"
Copy-Item "src\lauxlib.h" -Destination "$($installDir)\include"
Copy-Item "src\lua.hpp" -Destination "$($installDir)\include"
Copy-Item "src\luajit.h" -Destination "$($installDir)\include"
Copy-Item "src\jit" -Destination "$($installDir)\jit\"
Copy-Item "src\luajit.exe" -Destination "$($installDir)\bin"
Copy-Item "src\lua51.dll" -Destination "$($installDir)\bin"

Set-Location "$($pwd.Path)\.."
Remove-Item -Recurse -Force "LuaJIT-2.1.ROLLING"