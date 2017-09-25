#include "mytok.h"
#include "ksh.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv, char **envp) {
  int pid;
  int waitVal, waitStatus;
  char *cmd;
  int count = 0;

  //infinite loop to keep asking for commands
  while(1) {
    write(1, "$ ", 2);
    //ask user for input and store in string variable
    char *str = (char *) malloc(1000);
    read(0, str, 1000);
    //check if command was exit, if it was, exit
    int checkExit = cmp(str, "exit");
    if(checkExit == 1) {
      //check if user want to exit
      exit(0);
    }

    else {
      //space constant as delimiter defined in ksh.h file
      //tokenize string and place it in array, first argument will be command, next two will be parameters
      char **array = mytok(str, SPACE);

      
      /*
      //  printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
      // printf("hello, I am child (pid:%d)\n", (int) getpid());

        execve(array[0], &array[0], envp);  // runs word count
        printf("this shouldn't print out");
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
	       rc, wc, (int) getpid());
	       }*/



      


      

      //if it is not empty, begin a count to track if the command existed
      if(array != '\0'){



	//found uses stat system call to check if command exists, if it returns true it sets the command to the first argument passed by the user
	if(found(array[0])) {
	  
	  
	  cmd = array[0];

	  //sets command counter to 1 to execute
	    count = 1;
	}
	//if it was not immediately found
	else {
	  
	  //find $PATH variable
	    char *path = getenv("PATH");

	  
	  //once found $PATH, delimit string with colon to traverse through all directories
	   char **dir = mytok(path, COLON);

	  //traverse through all files to find a potential command in all directories
	  int i;
	  
	   for(i = 0; dir[i] != (char *)0; i++) {
	    //once found a file, put together in the syntax is, ex: /usr/local/sbin/cmd
	     char *cmd1 = myconcat(myconcat(dir[i], "/"), array[0]);
	     //printf("\nyour command was %s\n", cmd1);
	     if(found(cmd1)) {
	       
	    
	       count = 1;
	       cmd = cmd1;
	      
	  
	       break;
	       }
	    }
	
      
    
      if(count == 1) {
      //if command was found, create child process
	  pid = fork();
	  if(pid == 0){
	//if child process, exec
	     execve(cmd, array, envp);
	    }
	  else {
	//if parent process, wait until child exits
	      waitVal = waitpid(pid, &waitStatus, 0);
	      if(waitVal == pid) {
	       printf("%s\n child exited with value: %d", waitStatus);
	     }
		  }
		}
	else {
	  //if command was not found, print error message
	  printf("%s command was not found\n", array[0]);
	}
	}
      }
    }
  }
}

//stat system call was made to check if command exists and if is executable, if it is, method returns true
int found(char *arg) {
  struct stat sb;
  if((stat(arg, &sb) == 0) && (sb.st_mode & S_IXUSR))
    return 1;
  else return 0;
}

//string comparison method to check if exit was entered, if both strings match, it returns true
int cmp(char *str, char *str2) {
  int count = 0;
  while(str[count] != '\0' && str2[count] != '\0') {
    if(str[count] != str2[count]) {
      return 0;
    }
    count++;
  }
    return 1;
}

//letter count method
int letters(char *str) {
  int letters = 0;
  char currChar;
  while(*str != '\0') {
    *str++;
    letters++;
  }
  return letters;
}

//concatenation method to concatenate commands
char *myconcat(char *str1, char *str2) {
  int let1 = letters(str1);
  int let2 = letters(str2);
  char *concat = (char *) malloc(let1 + let2 + 1);
  char *res = concat;
  char *tmp;

  for(tmp = str1; *tmp; res++, tmp++) {
    *res = *tmp;
  }
  for(tmp = str2; *tmp; res++ , tmp++) {
    *res = *tmp;
  }
 
  *res = '\0';

  return concat;
  }
