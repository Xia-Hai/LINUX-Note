#test: add.cpp sub.cpp div.cpp mul.cpp test.cpp
#	g++ add.cpp sub.cpp div.cpp mul.cpp test.cpp -o test

#test: add.o sub.o div.o mul.o test.o
#OBJS = add.o sub.o div.o mul.o test.o
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRC))

TARGET = test

$(TARGET): $(OBJS)
	@g++ $^ -o $@

#add.o: add.cpp
#	g++ -c $< -o $@
#sub.o: sub.cpp
#	g++ -c $< -o $@
#div.o: div.cpp
#	g++ -c $< -o $@
#mul.o: mul.cpp
#	g++ -c $< -o $@
#test.o: test.cpp
#	g++ -c $< -o $@
%.o: %.cpp
	@g++ -c $< -o $@

.PHONY: clean
clean:
	@-rm -rf $(OBJS) $(TARGET)
