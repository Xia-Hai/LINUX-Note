all: read write
	
read: read_fifo.cpp
	g++ $< -o $@
write: write_fifo.cpp
	g++ $< -o $@

.PHONY: clean
clean:
	rm -rf read write