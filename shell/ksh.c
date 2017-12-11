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
  char *cmd = (char *) malloc(100);
  int count = 0;
  char cwd[200];
  char **array;
  char *str = (char *) malloc(1000);
  int arrlen;

  //find path environment
   char *path = getenv("PATH");
   path = myconcat("trash:", path);
   //once found $PATH, delimit string with colon to traverse through all directories
   char **dir = mytok(path, COLON);

  //infinite loop to keep asking for commands
  while(1) {
    write(1, "$ ", 2);
    //if array has commands still in it, free the array

      /*if(array != NULL) {
			for(int i = 0; array[i] != '\0'; i++) {
        free(array[i]);
        array[i] = NULL;
		}
    free(array);
		array = NULL;
  }*/

    //ask user for input and store in string variable

    int l = read(0, str, 1000);
    //read method adds extra '\n' character, so erase and replace with '\0'
    str[l - 1] = '\0';

    //check if command was exit, if it was, exit
//    int checkExit = cmp(str, "exit");
//    if(checkExit == 1) {
//printf("'%s'\n",str);
	if(cmp(str, "exit")){
      //check if user want to exit
      exit(0);
    }


    else {
      //space constant as delimiter defined in ksh.h file
      //tokenize string and place it in array, first argument will be command, next two will be parameters
      array = mytok(str, SPACE);

      for(int i = 0; array[i]; i++) {
      /*  if(findChar(array[i], '&')) {
        //array[i] = NULL;
          array[i] = '\0';
        }*/
      //  printf("%s\n", array[i]);

    }
    //printf("another test\n"); fflush(stdout);

      //if user wants to change directory, use chdir system call and show current working directory
      if(cmp(array[0], "cd")) {
        getcwd(cwd, sizeof(cwd));
        printf("check cd\n");
        ///printf("current working directory%s\n",cwd);
        //fflush(stdout);
        int success;
        success = checkDir(array[1]);
        if(success) {
          if(!chdir(array[1]))
            continue;
        }
      }

      //if a pipe was found in the string, call piping method
      /* if(cmp(array[1], "|")) {
	piping(str, envp, dir);
	}*/


      //if it array not empty, begin a count to track if the command existed
      if(array != NULL){

	//found uses stat system call to check if command exists, if it returns true it sets
  //the command to the first argument passed by the user
        if(found(array[0]) == 1) {
          printf("found");

	        cmd = array[0];

	  //sets command counter to 1 to execute
          count = 1;
	}
	//if it was not immediately found
	else {
		//traverse through all files to find a potential command in all directories

    for(int i = 0; dir[i] != (char *)0; i++) {
			//once found a file, for every file, put together in the syntax is, ex: /usr/local/sbin/cmd
			char *cmd1 = (char *) malloc(1000);
      cmd1 = myconcat(myconcat(dir[i], "/"), array[0]);

			//call found method and if it is found, set command counter to 1 and set the tmp
      //cmd to the actual command ready to execute
			if(found(cmd1) == 1) {
				count = 1;
				cmd = cmd1;
				break;
			}
		}

		if(count == 1) {
		//if command was found, create process

			pid = fork();
			if(pid == 0){
				//if child process, execute
				execve(cmd, array, envp);
				count = 0;
			}
			else if(pid != 0){
				//if parent process, wait until child exits
				waitVal = waitpid(pid, &waitStatus, 0);
				if(waitVal == pid) {
					printf("\nchild exited with value: %d\n", waitStatus);
					count = 0;
				}
			}
			//if it is not a child process and if there is an & found in the arguments, run on background
			else if(pid != 0) {
				printf("running on bg");
				/*printf("%d\n",findChar(str, '&'));
				printf("is running on background");
				runBackground(cmd, array, envp);*/
				continue;
			}
		}
		else {
			//if command was not found, print error message
		printf("command was not found%s\n\n", array[0]);
  }
}
}
//if the array is not empty and we are done with the commands, free the memory
    if(array != NULL) {
      freeArray(array);
    }
  }
}
}

//stat system call was made to check if command exists and if is executable, if it is, method returns true
int found(char *arg) {
  struct stat sb;
  if((stat(arg, &sb) == 0) && (sb.st_mode & S_IXOTH))
    return 1;
  else return 0;
}
//stat system call was made to check directories
int checkDir(char *arg) {
  struct stat sb;
  if((stat(arg, &sb) == 0) && (sb.st_mode & S_IFDIR))
    return 1;
  else return 0;
}

//string comparison method to check if any direct exit or cd was entered, if both strings match, it returns true
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

//method to find a certain specific character in the original string
int findChar(char *str, char toFind) {
   int count = 0;
  while(str[count] != '\0') {
    if(str[count] == toFind) {
      return 1;
    }
    count++;
  }
    return 0;
}

void runBackground(char *cmd, char ** array, char **envp) {
  execve(cmd, array, envp);
}

void freeArray(char ** array) {
  for(int i = 0; array[i] != 0; i++) {
    free(array[i]);
    array[i] = NULL;
  }
  free(array);
  array = NULL;
}

//piping method taken from The Linux Programming Interface book added with the loops above to find commands in each directory

/*int piping(char *str, char **envp, char ** dir) {
  int pfd[2];
  int count, pid;
  char * cmd;
  char *cmd2;

  if(pipe(pfd) == -1)
    printf("pipe");

  char ** tmp = mytok(str, PIPE);

  switch(fork()) {
  case -1:
    printf("fork");

  case 0:
    if(close(pfd[0]) == -1)
       printf("close 1");
    if(pfd[1] != STDOUT_FILENO) {
      if(dup2(pfd[1], STDOUT_FILENO) == -1)
        printf("dup2 1");
      if(close(pfd[1]) == -1)
        printf("close 2");
    }

      for(int i = 0; dir[i] != (char *)0; i++) {
	//once found a file, put together in the syntax is, ex: /usr/local/sbin/cmd
	char *cmd1 = myconcat(myconcat(dir[i], "/"), tmp[0]);
        int x = found(cmd1);
        if(x == 1) {
	  printf("found");
	  count = 1;
	  cmd = cmd1;
	  break;
	}
      }
       if(count == 1) {
      //if command was found, create child process
	  pid = fork();
	  if(pid == 0){
	//if child process, execute
	    execve(cmd, tmp, envp);
	     count = 0;
	  }
       }

       default:
	 break;
  }

	 switch(fork()) {
	 case -1:
	   printf("fork");
	 case 0:
	   if(close(pfd[1]) == -1)
	     printf("close 3");
	   if(pfd[0] != STDIN_FILENO) {
	     if(dup2(pfd[0], STDIN_FILENO) == -1) {
	       printf("dup2 2");
	     }
	     if(close(pfd[0]) == -1)
	       printf("close 4");
	   }

	      for(int i = 0; dir[i] != (char *)0; i++) {
	//once found a file, put together in the syntax is, ex: /usr/local/sbin/cmd
		char *cmd2 = myconcat(myconcat(dir[i], "/"), tmp[1]);
		int x2 = found(cmd2);
		if(x2 == 1) {
		  printf("found2");
		  count = 1;
		  cmd = cmd2;
		  break;
		}
	      }
	      if(count == 1) {
		//if command was found, create child process
		pid = fork();
		if(pid == 0){
		  //if child process, execute
		  execve(cmd2, tmp, envp);
		  count = 0;
		}
	      }
       default:
	 break;
	 }




if(close(pfd[0]) == -1)
  printf("close 5");
if(close(pfd[1]) == -1)
   printf("close 6");
if(wait(NULL) == -1)
  printf("wait 1");
if(wait(NULL) == -1)
  printf("wait 2");

  }*/
