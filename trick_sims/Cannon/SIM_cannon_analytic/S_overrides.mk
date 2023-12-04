TRICK_CFLAGS += -I../models -I/home/fjolnirr/trick/trick_source -g 
TRICK_CXXFLAGS += -I../models -I/home/fjolnirr/trick/trick_source -g 			

TRICK_EXEC_LINK_LIBS += -L/usr/local/lib -lpthread -lcrypto \
-lssl -lmongocxx -lbsoncxx

# add the line below to the .bashrc for permanent solution
# export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

