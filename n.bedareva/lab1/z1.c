#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ulimit.h>

 int main(int argc, char *argv[])
 {
     char options[ ] = "ispu:U:cC:dvV:";  /* valid options */
     int c, invalid = 0;
     
     printf("argc equals %d\n", argc);
     
     while ((c = getopt(argc, argv, options)) != -1) {
     switch (c) {
      case 'i':
          system("id -u");
    	  system("id -ur");
          break;
      case 's':
          setpgid(0, 0);
          break;
      case 'p':
       	  pid_t pid = getpid();
    	  pid_t ppid = getppid();
    	  pid_t pgid = getpgid(pid);

    	  printf("Process ID: %d\nParent Process ID: %d\nProcess Group ID: %d\n", pid, ppid, pgid);
          break;
      case 'u':
      	  long limit = ulimit(UL_GETFSIZE, 0);
    	  printf("Ulimit: %ld\n", limit);
          break;
          
      case 'U':
      	  atol(optarg);
          break;
      case 'c':
      	  long core_size = ulimit(UL_GETFSIZE, 0);
    	  printf("Core file size limit: %ld bytes\n", core_size);
          break;
          
      case 'C':
      	  long res = ulimit(UL_SETFSIZE,10);
    	  printf("Core file size limit: %ld bytes\n", res);
          break;
         
      case 'd':
      	  char cwd[1000];
    	  getcwd(cwd, sizeof(cwd));
          printf("Working directory: %s\n", cwd);
    	
          break;
          
      case 'v':
      	  extern char **environ;
      	  for (char **env = environ; *env != 0; env++) {
        	char *thisEnv = *env;
        	printf("%s\n", thisEnv);
        	}
        break;
      case 'V':
      
      	  char *name = strtok(optarg, "=");
          char *value = strtok(NULL, "=");
          if (value != NULL) {
                 setenv(name, value, 1);
           } else 
           {
                 printf("Invalid argument for -V\n");
           }
           break;
      
      case '?':
          printf("invalid option is %c\n", optopt);
          invalid++;
          break;
            
     }

      }

	return 0;
 }

