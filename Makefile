.SILENT:

CXX=g++
OUT=gof

run: build
	./$(OUT)

build:
	$(CXX) -o $(OUT) main.cpp