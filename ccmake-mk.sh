mkdir obj
mkdir obj/mk
cd obj/mk
ccmake -Wdev -G "Unix Makefiles" ../..
make all
cd ../..
