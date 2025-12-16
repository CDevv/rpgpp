git submodule update --init --recursive

make -C ./libs/luajit/
cp ./libs/luajit/src/luajit ./execs/luajit
cp ./libs/luajit/src/libluajit.so ./libs/libluajit.so
