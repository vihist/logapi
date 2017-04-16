output_dir = ../bin/
objpath = ./obj/
include_path = -I../src/linux/ -I../src/

flag = -D_LINUX_ -D_GNU_SOURCE -fvisibility=hidden -g -c
cc = g++

compile = $(cc) $(include_path) $(flag) $< -o $@

objects = $(objpath)circlebuff.o\
	$(objpath)loginfo.o\
	$(objpath)logapi.o\
	$(objpath)logapp.o\
	$(objpath)logqueue.o\
	$(objpath)ofstreammanager.o\
	$(objpath)recorder.o\
	$(objpath)directoryop.o\
	$(objpath)luxevent.o\
	$(objpath)luxthread.o\
	$(objpath)inifile.o\
	$(objpath)luxcritical.o\
    $(objpath)sem.o\
	
all : $(objpath)	
$(objpath):
	mkdir -p $(objpath)
all : $(output_dir)
$(output_dir):
	mkdir -p $(output_dir)

all : $(output_dir)liblogapi.a 

$(output_dir)liblogapi.a : $(objects) 
	ar rc $@ $(objects)

$(objpath)circlebuff.o: ../src/circlebuff.cpp
	$(compile)
	
$(objpath)loginfo.o: ../src/loginfo.cpp
	$(compile)
	
$(objpath)logapi.o: ../src/logapi.cpp
	$(compile)
			
$(objpath)ofstreammanager.o: ../src/ofstreammanager.cpp
	$(compile)

$(objpath)logqueue.o: ../src/logqueue.cpp
	$(compile)
		
$(objpath)recorder.o: ../src/recorder.cpp
	$(compile)

$(objpath)directoryop.o: ../src/linux/directoryop.cpp
	$(compile)

$(objpath)luxevent.o: ../src/linux/luxevent.cpp
	$(compile)
		
$(objpath)luxthread.o: ../src/linux/luxthread.cpp
	$(compile)

$(objpath)inifile.o: ../src/linux/inifile.cpp
	$(compile)

$(objpath)luxcritical.o: ../src/linux/luxcritical.cpp
	$(compile)
	
$(objpath)sem.o: ../src/sem.cpp
	$(compile)
	
$(objpath)logapp.o: ../src/logapp.cpp
	$(compile)
	
.PHONY: clean

clean: 
	rm -r -f $(objpath) $(output_dir)liblogapi.a 
