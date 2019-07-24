cd ..\premake
if not exist ..\solutions\vc2013\SKIP_PREMAKE_HERE premake5 vs2013
if not exist ..\solutions\vc2015\SKIP_PREMAKE_HERE premake5 vs2015
cd ..\solutions