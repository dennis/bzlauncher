all: bzlauncher

CXX=ccache g++
CXXFLAGS=`wx-config --cxxflags` -Wall -ggdb 

bzlauncher: listserver.o gui.o main.o server.o
	$(CXX) *.o `wx-config --libs` -o bzlauncher

listserver.o: src/listserver.cpp src/listserver.h
	$(CXX) src/listserver.cpp -c $(CXXFLAGS)
gui.o: src/gui.cpp src/gui.h
	$(CXX) src/gui.cpp -c $(CXXFLAGS)
main.o: src/main.cpp src/main.h
	$(CXX) src/main.cpp -c $(CXXFLAGS)
server.o: src/server.cpp src/server.h
	$(CXX) src/server.cpp -c $(CXXFLAGS)

src/gui.cpp:
	-wxformbuilder -g bzlauncher.fbp 

	
clean:
	-rm *.o
	-rm bzlauncher

cleanall: clean
	-rm src/gui.cpp src/gui.h
