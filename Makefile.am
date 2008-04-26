SUBDIRS = src

EXTRA_DIST=bzlauncher.sln bzlauncher.vcproj README MIT-LICENSE

ICONS=\
	src/about32.h \
	src/bzflag32.h \
	src/favorite32.h \
	src/ping32.h \
	src/refresh32.h \
	src/search32.h \
	src/stop22.h 

pregenerate: gui_files $(ICONS)
	cp -a src/gui_raw.cpp src/gui.cpp
	cp -a src/gui_raw.h src/gui.h

gui_files:
	-wxformbuilder -g bzlauncher.fbp 
	sed '/#include "gui_raw.h"/ a #include <wx/mstream.h>\n#include "../config.h"\n\ninline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) { wxMemoryInputStream is(data, length); return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1); }\n #define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))' <src/gui_raw.cpp >src/gui_raw.cpp.tmp && \
		mv src/gui_raw.cpp.tmp src/gui_raw.cpp
	sed 's/#include "gui_raw.h"/#include "gui.h"/' <src/gui_raw.cpp >src/gui_raw.cpp.tmp && \
		mv src/gui_raw.cpp.tmp src/gui_raw.cpp
	sed 's/__gui_raw__/__gui__/' <src/gui_raw.h >src/gui_raw.h.tmp \
		&& mv src/gui_raw.h.tmp src/gui_raw.h
	sed '/#define __gui__/ a #include "../config.h"\n' <src/gui_raw.h >src/gui_raw.h.tmp \
		&& mv src/gui_raw.h.tmp src/gui_raw.h
	sed 's/#VERSION#/" VERSION "/' <src/gui_raw.cpp >src/gui_raw.cpp.tmp \
		&& mv src/gui_raw.cpp.tmp src/gui_raw.cpp
	sed 's/#VERSION#/" VERSION "/' <src/gui_raw.h >src/gui_raw.h.tmp \
		&& mv src/gui_raw.h.tmp src/gui_raw.h
	sed 's/tabs = new wxNotebook.+//' <src/gui_raw.cpp >src/gui_raw.cpp.tmp \
		&& mv src/gui_raw.cpp.tmp src/gui_raw.cpp

src/%.h: icons/%.png bin2c 
	./bin2c -c $< $@
	sed '/#define __gui__/ a #include "$(notdir $@)"' <src/gui_raw.h >src/gui_raw.h.tmp && mv src/gui_raw.h.tmp src/gui_raw.h
	sed 's|wxBitmap( wxT("../icons/$(notdir $<)"), wxBITMAP_TYPE_ANY )|wxGetBitmapFromMemory($(notdir $(basename $@)))|g' <src/gui_raw.cpp >src/gui_raw.cpp.tmp && mv src/gui_raw.cpp.tmp src/gui_raw.cpp

bin2c:
	$(CC) contrib/bin2c.c -o bin2c

pregenerate_clean:
	-rm gui.cpp gui.h
	-rm gui_raw.cpp gui_raw.h
	-rm bin2c
	-rm src/*22.h src/*32.h
