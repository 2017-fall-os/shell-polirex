int main(int arg, char **argv, char **argu);
int found(char *arg);
int cmp(char *str, char *str2);
int letters(char *str);
char *myconcat(char *str1, char *str2);
//int piping(char *str, char **envp, char **dir);
void runBackground(char *cmd, char ** array, char **envp);
int findChar(char *str, char toFind);
int checkDir(char *arg);
void freeArray(char **array);
#ifndef DELIM
#define SPACE ' '
#define COLON ':'
#define PIPE '|'
#endif
