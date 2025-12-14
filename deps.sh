git submodule update --init --recursive

make -C ./libs/lua/ MYCFLAGS="-fPIC"
cp ./libs/lua/lua ./execs/lua
cp ./libs/lua/liblua.a ./libs/liblua.a
