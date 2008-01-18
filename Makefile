all: bzlauncher

CXX=ccache g++
CXXFLAGS=`wx-config --cxxflags` -Wall -ggdb 

bzlauncher: gui.o listserverhandler.o main.o server.o mainframe_impl.o aboutdlg_impl.o serverdlg_impl.o
	$(CXX) *.o `wx-config --libs` -o bzlauncher

listserverhandler.o: src/listserverhandler.cpp src/listserverhandler.h
	$(CXX) src/listserverhandler.cpp -c $(CXXFLAGS)
gui.o: src/gui.cpp src/gui.h
	$(CXX) src/gui.cpp -c $(CXXFLAGS)
main.o: src/main.cpp src/main.h
	$(CXX) src/main.cpp -c $(CXXFLAGS)
server.o: src/server.cpp src/server.h
	$(CXX) src/server.cpp -c $(CXXFLAGS)
mainframe_impl.o: src/mainframe_impl.cpp src/mainframe_impl.h
	$(CXX) src/mainframe_impl.cpp -c $(CXXFLAGS)
aboutdlg_impl.o: src/aboutdlg_impl.cpp src/aboutdlg_impl.h
	$(CXX) src/aboutdlg_impl.cpp -c $(CXXFLAGS)
serverdlg_impl.o: src/serverdlg_impl.cpp src/serverdlg_impl.h
	$(CXX) src/serverdlg_impl.cpp -c $(CXXFLAGS)

src/gui.cpp:
	-wxformbuilder -g bzlauncher.fbp 
	-vim -c "%s/\.\.\/icons\//\icons\//g" -c "wq" src/gui.cpp

	
clean:
	-rm *.o
	-rm bzlauncher

cleanall: clean
	-rm src/gui.cpp src/gui.h
