LFLAG = -ljsoncpp -lmuduo_net -lmuduo_base -pthread
##绑定库
.PHONY : all
all : test test_server
test : test.cpp
	g++ -std=c++11 $^ -o $@ $(LFLAG)
test_server : test_server.cpp
	g++ -std=c++11 $^ -o $@ $(LFLAG)
clean:
	rm  test test_server

