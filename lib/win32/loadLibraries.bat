REM boost
if not exist boost/ (
git clone --recursive https://github.com/boostorg/boost.git
cd boost
git checkout develop
)

REM WXWIDGETS
if not exist wxWidgets/ (
git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git
)