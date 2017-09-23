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

  while(1) {
    char *str = (char *) malloc(1000);
    read(0, str, 1000);
    
    int checkExit = cmp(str, "exit");

    if(checkExit == 1) {
      // printf("%s\n user exit");
      //check if user want to exit
      return 0;
    }
    else {
      //space constant as delimiter defined in ksh.h file
      //tokenize string and place it in array
      char **array = mytok(str, SPACE);
      printf("%s\n", array[1]);

      if(array != '\0'){
	printf("test");
	int count = 0;
	//	printf("%s\n", array[0]);

	printf("adsf");
	
	if(found(array[0])) {
	  printf("%s\n ??????");
	  cmd = array[0];
	  printf("%s\n",array[0]);
	    count++;
	}
	else {
	  printf("i'm here");
	  //find $PATH
	  char *path = getenv("PATH");
	  
	  //once found $PATH, delimit string with colon
	  //char **dir = mytok(path, COLON);
	  printf("%s\n still here");
	  
	  //for(int i = 0; dir[i] != (char *) 0; i++) {
	    //char *cmd1 = myconcat(myconcat(dir[i], "/"), array[0]);
	    
	    //	    printf("%s\n", myconcat(myconcat(dir[i], "/"), array[0]));
	  
	  //	  }
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

int found(char *arg) {
  struct stat sb;
  if((stat(arg, &sb) == 0) && (sb.st_mode & S_IXUSR))
    return 1;
  else return 0;
}

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

int letters(char *str) {
  int letters;
  char currChar;
  while(*str != '\0') {
    currChar = *str;
    letters++;
  }
  return letters;
}

char* myconcat(char *str1, char *str2) {
  int let1 = letters(str1);
  int let2 = letters(str2);
  char *concat = (char *) malloc(let1 + let2 + 1);
  char *res = concat;
  for(; *str1; *concat++ = *str1++);
  for(; *str2; *concat++ = *str2++);
  *concat = '\0';
  return res;
}
