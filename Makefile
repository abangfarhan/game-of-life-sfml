CXX = g++
CXXFLAGS = --std=c++11 -g
SFML = -DSFML_STATIC -I "D:/Program Files (x86)/SFML/include" \
	-L "D:/Program Files (x86)/SFML/lib" \
	-l sfml-graphics-s \
	-l sfml-window-s \
	-l sfml-system-s \
	-l opengl32 \
	-l winmm \
	-l gdi32 \
	-l freetype \
	-l jpeg \

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(SFML)
