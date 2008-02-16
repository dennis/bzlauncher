# requirex wxformbuilder, imagemagick, ccache, g++

CXX=ccache g++
CXXFLAGS=`wx-config --cxxflags` -ggdb -Wall -W 
ROOTDIR=../..
SRCDIR=$(ROOTDIR)/src
ICONDIR=$(ROOTDIR)/icons
OBJS=\
	aboutdlg_impl.o \
	config.o \
	gui.o \
	listserverhandler.o \
	main.o \
	mainframe_impl.o \
	server.o \
	serverdlg_impl.o \
	updatingdlg_impl.o

all: dist bzlauncher

dist: $(SRCDIR)/gui.cpp

bzlauncher: $(ROOTDIR)/bzlauncher

valgrind: bzlauncher
	cd $(ROOTDIR) && valgrind --leak-check=full --suppressions=build/gnu/valgrind.suppression ./bzlauncher

$(ROOTDIR)/bzlauncher: $(OBJS)
	$(CXX) *.o `wx-config --libs` -o $(ROOTDIR)/bzlauncher

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

gui.o: $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Create src/gui.(cpp|h)
$(SRCDIR)/gui.cpp: $(ROOTDIR)/bzlauncher.fbp
	-wxformbuilder -g $(ROOTDIR)/bzlauncher.fbp 
	-vim -c "%s/\.\.\/icons\//\icons\//g" -c "wq" $(SRCDIR)/gui.cpp

# Utility
clean:
	-rm *.o
	-rm $(ROOTDIR)/bzlauncher

cleanall: clean
	-rm $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h

.PHONY: clean cleanall all xpm dist bzlauncher valgrind
