#include <sys/stat.h>
#include <errno.h>

#include "directoryop.h"

bool CreateDirectory(const char* name, void*)
{
	
	return ( mkdir(name, 0777)==0 );
}