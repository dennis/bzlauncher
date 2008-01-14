all:
	$(CXX) src/gui.cpp -c `wx-config --cxxflags`
	$(CXX) src/main.cpp -c `wx-config --cxxflags`
	$(CXX) *.o `wx-config --libs` -o bzlauncher
clean:
	rm *.o
	rm bzlauncher
