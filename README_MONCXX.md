# How to make mongocxx lib work 

paste this to the .bashrc file located at home dir.

```
export PATH=${PATH}:${HOME}/trick/bin

export CFLAGS="$CFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export CXXFLAGS="$CXXFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export LDFLAGS="$LDFLAGS $1 -O0 -g -Wl -V -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_CFLAGS="$TRICK_CFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_CXXFLAGS="$TRICK_CXXFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_LDFLAGS="$TRICK_LDFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_SYSTEM_CFLAGS="$TRICK_SYSTEM_CFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_SYSTEM_CXXFLAGS="$TRICK_SYSTEM_CXXFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export TRICK_SYSTEM_LDFLAGS="$TRICK_SYSTEM_LDFLAGS $1 -O0 -g -Iinclude -Ilib -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx"
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```
