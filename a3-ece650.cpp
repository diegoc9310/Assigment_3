// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 


#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main()
{
   int link[2];
   pid_t pid;
   char foo[4096];
 
   if (pipe(link) == -1)
      die("pipe");

   if ((pid = fork()) == -1)
      die("fork");

   if (pid == 0) {

      dup2(link[1], STDOUT_FILENO);
      close(link[0]);
      close(link[1]);
      execlp("python", "python", "a1-ece650.py", "test", (char*)NULL);
      die("execl");

   }
   else {

      close(link[1]);
      int nbytes = read(link[0], foo, sizeof(foo));

      char *pythonArgs[] = { "./a2-ece650",foo,NULL };
      execvp("./a2-ece650", pythonArgs);

      
      wait(NULL);

   }
   return 0;
}
//#include <iostream>
//#include <stdio.h>
//#include <unistd.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <stdlib.h>
//using namespace std;
//
//int main (int argc, char **argv) {
// 
//   pid_t pid;
//   int mypipe[2];
//   int ret;
//   ret = pipe(mypipe);
//
//   if (ret == -1)
//   {
//      perror("pipe");
//      exit(1);
//   }
//
//   pid = fork();
//
//   if (pid == 0)
//   {
//      dup2(mypipe[1], 1); // redirect stdout to pipe
//      execlp("python", "python", "a1-ece650.py", "test", (char*)NULL);
//   }
//   if (pid > 0)
//   {
//      dup2(mypipe[0], 0); // get stdin from pipe     
//      
//      char buff[1000] = { 0 };
//      read(STDIN_FILENO, buff, 250);
//      printf("---- in process2 -----\n%s\n", buff);
//      char *pythonArgs[] = { "./a2-ece650","V 15","E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}",NULL };
//      execvp("./a2-ece650", pythonArgs);
//      wait((int*)-1);
//
//   }
//   return 0;
//
//   
//   
//}
