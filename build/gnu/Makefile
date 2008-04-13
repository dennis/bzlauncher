# requirex wxformbuilder, imagemagick, ccache, g++

CXX=ccache g++
CC=gcc
WXFLAGS=`wx-config --cxxflags` -ggdb -Wall -W 
WXLIBS=`wx-config --libs`
ROOTDIR=../..
SRCDIR=$(ROOTDIR)/src
CONTRIBDIR=$(ROOTDIR)/contrib
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
	serverparser.o \
	serverping.o \
	updatingdlg_impl.o \
	query.o
ICONS=\
	$(SRCDIR)/about32.h \
	$(SRCDIR)/bzflag32.h \
	$(SRCDIR)/favorite32.h \
	$(SRCDIR)/ping32.h \
	$(SRCDIR)/refresh32.h \
	$(SRCDIR)/search32.h \
	$(SRCDIR)/stop22.h 

all: $(ROOTDIR)/bzlauncher

$(ROOTDIR)/bzlauncher: $(OBJS)
	$(CXX) *.o $(WXLIBS) -o $(ROOTDIR)/bzlauncher

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) -c $(WXFLAGS) $< -o $@

gui.o: $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h
	$(CXX) -c $(WXFLAGS) $< -o $@

# Pre compilation steps
pre: gui_files $(ICONS)
	cp -a $(SRCDIR)/gui_raw.cpp $(SRCDIR)/gui.cpp
	cp -a $(SRCDIR)/gui_raw.h $(SRCDIR)/gui.h

# Generate cpp from wxformbuilder
$(SRCDIR)/gui_raw.cpp: $(ROOTDIR)/bzlauncher.fbp
	-wxformbuilder -g $(ROOTDIR)/bzlauncher.fbp 

# wxformbuilder postprocessing: Add some code to top of gui_raw.cpp
gui_files: $(SRCDIR)/gui_raw.cpp
	# Change code so we can use the embedded PNG for the toolbar
	sed '/#include "gui_raw.h"/ a #include <wx/mstream.h>\n#include "version.h"\n\ninline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) { wxMemoryInputStream is(data, length); return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1); }\n #define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))' <$(SRCDIR)/gui_raw.cpp >$(SRCDIR)/gui_raw.cpp.tmp && \
		mv $(SRCDIR)/gui_raw.cpp.tmp $(SRCDIR)/gui_raw.cpp
	# Correct "gui_raw.h" to "gui.h"
	sed 's/#include "gui_raw.h"/#include "gui.h"/' <$(SRCDIR)/gui_raw.cpp >$(SRCDIR)/gui_raw.cpp.tmp && \
		mv $(SRCDIR)/gui_raw.cpp.tmp $(SRCDIR)/gui_raw.cpp
	# Correct __gui_raw__ to __gui__
	sed 's/__gui_raw__/__gui__/' <$(SRCDIR)/gui_raw.h >$(SRCDIR)/gui_raw.h.tmp \
		&& mv $(SRCDIR)/gui_raw.h.tmp $(SRCDIR)/gui_raw.h
	# Include "version.h", so we can use the VERSION define in next step
	sed '/#define __gui__/ a #include "version.h"\n' <$(SRCDIR)/gui_raw.h >$(SRCDIR)/gui_raw.h.tmp \
		&& mv $(SRCDIR)/gui_raw.h.tmp $(SRCDIR)/gui_raw.h
	# Change #VERSION# to the VERSION define
	sed 's/#VERSION#/" VERSION "/' <$(SRCDIR)/gui_raw.cpp >$(SRCDIR)/gui_raw.cpp.tmp \
		&& mv $(SRCDIR)/gui_raw.cpp.tmp $(SRCDIR)/gui_raw.cpp
	sed 's/#VERSION#/" VERSION "/' <$(SRCDIR)/gui_raw.h >$(SRCDIR)/gui_raw.h.tmp \
		&& mv $(SRCDIR)/gui_raw.h.tmp $(SRCDIR)/gui_raw.h
	# Remove: tabs = new wxNotebook...
	sed 's/tabs = new wxNotebook.+//' <$(SRCDIR)/gui_raw.cpp >$(SRCDIR)/gui_raw.cpp.tmp \
		&& mv $(SRCDIR)/gui_raw.cpp.tmp $(SRCDIR)/gui_raw.cpp

# wxformbuilder postprocessing. 
# - Convert the png icons to c-code
# - modify gui_raw.h, to include the c-code
# - modify gui_raw.cpp, to use the embedded png icons, instead of files
$(SRCDIR)/%.h: $(ICONDIR)/%.png $(ROOTDIR)/bin2c
	$(ROOTDIR)/bin2c -c $< $@
	sed '/#define __gui__/ a #include "$(notdir $@)"' <$(SRCDIR)/gui_raw.h >$(SRCDIR)/gui_raw.h.tmp && mv $(SRCDIR)/gui_raw.h.tmp $(SRCDIR)/gui_raw.h
	sed 's|wxBitmap( wxT("../icons/$(notdir $<)"), wxBITMAP_TYPE_ANY )|wxGetBitmapFromMemory($(notdir $(basename $@)))|g' <$(SRCDIR)/gui_raw.cpp >$(SRCDIR)/gui_raw.cpp.tmp && mv $(SRCDIR)/gui_raw.cpp.tmp $(SRCDIR)/gui_raw.cpp

# build helper-tool bin2c - converts a png to c code for embedding them
$(ROOTDIR)/bin2c: $(CONTRIBDIR)/bin2c.c
	gcc $(CONTRIBDIR)/bin2c.c -o $(ROOTDIR)/bin2c
# CLEANUP

clean:
	-rm *.o
	-rm $(ROOTDIR)/bzlauncher

cleanall: clean
	-rm $(SRCDIR)/gui.cpp $(SRCDIR)/gui.h
	-rm $(SRCDIR)/gui_raw.cpp $(SRCDIR)/gui_raw.h
	-rm $(ROOTDIR)/bin2c
	-rm $(SRCDIR)/*32.h $(SRCDIR)/*22.h

.PHONY: cleanall clean pre all
