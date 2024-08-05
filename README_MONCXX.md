# How to make mongocxx lib work 

Run the `setup-mongocxx` file in `$HOME` to install mongocxx driver to your system:
- `sudo chmod +x setup-mongocxx`
- `./setup-mongocxx`

1- Add the line below to the ***Makefile.common (${TRICK_HOME}/share/trick/makefiles/Makefile.common)***

```
# This is required for 3rd party MongoDB library
TRICK_EXEC_LINK_LIBS += -L/usr/local/lib -lpthread -lcrypto -lssl -lmongocxx -lbsoncxx
```

2- Add the line below to the ***Makefile (${TRICK_HOME}/Makefile)***

```
TRICK_CXXFLAGS += -O0 -g -I${TRICK_HOME}/third_party \
			-I/usr/local/include/mongocxx/v_noabi \
 			-I/usr/local/include/bsoncxx/v_noabi \
			-I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc
```