#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    
    	system("chmod 600 file.txt");
    
    	system("id -u");
    	system("id -ur");
    	
    	char * filename = "file.txt";
	FILE *f;
    
    	f = fopen(filename, "w");
    	if (f == NULL) {
        	perror("Error opening file for writing");
    	}
    	else
    	{
    		fprintf(f,"Test");
    	    	fclose(f);
    	}

	
	int  setuid (uid_t uid);

      	int  setgid (gid_t gid);
	
	system("id -u");
    	system("id -ur");
    	
    	f = fopen(filename, "r");
    	if (f == NULL) {
        	perror("Error opening file for writing");
    	}
    	else
    	{
    	    	fclose(f);
    	}
    	
    	return 0;
}
