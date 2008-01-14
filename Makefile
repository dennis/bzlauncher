all:
	$(CXX) src/gui.cpp `wx-config --libs` `wx-config --cxxflags` -o bzlauncher
