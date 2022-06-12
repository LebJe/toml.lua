param(
	# Where LuaRocks should be installed
	[string]$installDir="$($pwd.Path)\LuaRocks"
)

if (-not (Test-Path -Path $installDir)) {
    mkdir $installDir
}

$installDir = Resolve-Path $installDir

Invoke-WebRequest -Uri "http://luarocks.github.io/luarocks/releases/luarocks-3.8.0-windows-32.zip" -OutFile "luarocks.zip"
Expand-Archive -Path "luarocks.zip" -DestinationPath "."
Move-Item -Path "luarocks-3.8.0-windows-32\luarocks.exe" -Destination "$($installDir)"
Move-Item -Path "luarocks-3.8.0-windows-32\luarocks-admin.exe" -Destination "$($installDir)"

Remove-Item "luarocks.zip"
Remove-Item -Recurse -Force "luarocks-3.8.0-windows-32"