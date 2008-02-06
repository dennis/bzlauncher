
CXX=ccache g++
CXXFLAGS=`wx-config --cxxflags` -ggdb -Wall -W 
ROOTDIR=../..
SRCDIR=$(ROOTDIR)/src

all: $(ROOTDIR)/bzlauncher

$(ROOTDIR)/bzlauncher: gui.o listserverhandler.o main.o server.o mainframe_impl.o aboutdlg_impl.o serverdlg_impl.o updatingdlg_impl.o
	$(CXX) *.o `wx-config --libs` -o $(ROOTDIR)/bzlauncher

listserverhandler.o: $(SRCDIR)/listserverhandler.cpp $(SRCDIR)/listserverhandler.h
	$(CXX) $(SRCDIR)/listserverhandler.cpp -c $(CXXFLAGS)
gui.o: $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h
	$(CXX) $(SRCDIR)/gui.cpp -c $(CXXFLAGS)
main.o: $(SRCDIR)/main.cpp $(SRCDIR)/main.h
	$(CXX) $(SRCDIR)/main.cpp -c $(CXXFLAGS)
server.o: $(SRCDIR)/server.cpp $(SRCDIR)/server.h
	$(CXX) $(SRCDIR)/server.cpp -c $(CXXFLAGS)
mainframe_impl.o: $(SRCDIR)/mainframe_impl.cpp $(SRCDIR)/mainframe_impl.h
	$(CXX) $(SRCDIR)/mainframe_impl.cpp -c $(CXXFLAGS)
aboutdlg_impl.o: $(SRCDIR)/aboutdlg_impl.cpp $(SRCDIR)/aboutdlg_impl.h
	$(CXX) $(SRCDIR)/aboutdlg_impl.cpp -c $(CXXFLAGS)
serverdlg_impl.o: $(SRCDIR)/serverdlg_impl.cpp $(SRCDIR)/serverdlg_impl.h
	$(CXX) $(SRCDIR)/serverdlg_impl.cpp -c $(CXXFLAGS)
updatingdlg_impl.o: $(SRCDIR)/updatingdlg_impl.cpp $(SRCDIR)/updatingdlg_impl.h
	$(CXX) $(SRCDIR)/updatingdlg_impl.cpp -c $(CXXFLAGS)

$(SRCDIR)/gui.cpp: $(ROOTDIR)/bzlauncher.fbp
	-wxformbuilder -g $(ROOTDIR)/bzlauncher.fbp 
	-vim -c "%s/\.\.\/icons\//\icons\//g" -c "wq" $(SRCDIR)/gui.cpp

	
clean:
	-rm *.o
	-rm bzlauncher

cleanall: clean
	-rm $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h

.PHONY: clean cleanall all
