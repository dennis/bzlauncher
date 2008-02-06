# requirex wxformbuilder, imagemagick, ccache, g++

CXX=ccache g++
CXXFLAGS=`wx-config --cxxflags` -ggdb -Wall -W 
ROOTDIR=../..
SRCDIR=$(ROOTDIR)/src
ICONDIR=$(ROOTDIR)/icons
OBJS=\
	aboutdlg_impl.o \
	gui.o \
	listserverhandler.o \
	main.o \
	mainframe_impl.o \
	server.o \
	serverdlg_impl.o \
	updatingdlg_impl.o
XPMS=\
	$(SRCDIR)/about32.xpm \
	$(SRCDIR)/bzflag32.xpm \
	$(SRCDIR)/down22.xpm \
	$(SRCDIR)/favorite32.xpm \
	$(SRCDIR)/ping32.xpm \
	$(SRCDIR)/refresh32.xpm \
	$(SRCDIR)/stop22.xpm \
	$(SRCDIR)/up22.xpm

all: dist bzlauncher

dist: xpm $(SRCDIR)/gui.cpp

bzlauncher: $(ROOTDIR)/bzlauncher

xpm: $(XPMS)

$(ROOTDIR)/bzlauncher: $(OBJS)
	$(CXX) *.o `wx-config --libs` -o $(ROOTDIR)/bzlauncher

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

gui.o: $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

%_xpm: $(ICONDIR)/%.png
	convert $< xpm:$@

$(SRCDIR)/%.xpm: %_xpm
	mv $< $@

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
	-rm $(SRCDIR)/*.xpm

.PHONY: clean cleanall all xpm dist bzlauncher
