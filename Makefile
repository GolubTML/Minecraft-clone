CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lGL -ldl -lpthread -lX11 -lGLEW

compile: src/*.cpp
	clang++ $(CFLAGS) -o obj/Minecraft src/*.cpp $(LDFLAGS)

.PHONY: test clean

run: compile
	./obj/Minecraft

clean:
	rm /obj/Minecraft