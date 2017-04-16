include_path = -I../../src/

flag = -D_LINUX_ -g -O2 -c
cc = g++


logtest: testmain.o
	g++  -lpthread -lrt testmain.o -L../../bin -o logtest -llogapi

testmain.o: ../testmain.cpp
	g++ $(flag) $(include_path) $< -o $@

.PHONY: clean
clean: 
	@rm -f testmain.o logtest
