all: talk1 talk2
	
talk1: talk1.cpp
	g++ $< -o $@
talk2: talk2.cpp
	g++ $< -o $@

.PHONY: clean
clean:
	rm -rf talk1 talk2