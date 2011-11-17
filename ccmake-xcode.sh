mkdir obj
mkdir obj/xcode
cd obj/xcode
ccmake -Wdev -G "Xcode" ../..
open *.xcodeproj
cd ../..
