LFLAG = -lmuduo_net -lmuduo_base -pthread
all: Server Client
Client: muduoClient.cpp
	g++   $^ -o $@ $(LFLAG)
Server: muduoServer.cpp
	g++   $^ -o $@ $(LFLAG)
clean:
	rm -r Server Client