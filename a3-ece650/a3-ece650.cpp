
#include <iostream>
#include <string>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
using namespace std;
//cd mnt/c/Users/Diego/Documents/ECE\ 650/Assigment\ 3/Assigment_3/A3/build

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
int main(int argc, char *argv[])
{
   int n;
   char *arguments[10];
   pid_t pid[4];
   int pipe_1[2], pipe_2[2];
   pipe(pipe_1); pipe(pipe_2);
  
   for (int i = 0; i < argc; ++i)
   {
      arguments[i] = argv[i];
   }

   arguments[9] = NULL;
   pid[0] = fork();
   if (pid[0] == 0)
   {
      // Rand writes to Python code
      dup2(pipe_1[1], STDOUT_FILENO);
      // close pipes
      for (n = 0; n < 2; n++)
      {
         close(pipe_1[n]); close(pipe_2[n]); 
      }
      execvp("./rgen", arguments);
   }
   
   pid[1] = fork();
   if (pid[1] == 0)
   {
      char foo[100];
      // Python code reads from Rand  
      dup2(pipe_1[0], STDIN_FILENO);
      // Python code writes to C++ code
      dup2(pipe_2[1], STDOUT_FILENO);
      // close pipes
      for (n = 0; n < 2; n++)
      {
         close(pipe_1[n]); close(pipe_2[n]); 
      }
      // execute Python code
      char *CalledPython = "./a1-ece650.py";
      char *pythonArgs[] = { "/usr/bin/python",CalledPython,NULL };
      execvp("/usr/bin/python", pythonArgs);
   }
   
   pid[2] = fork();
   if (pid[2] == 0)
   {
      // C++ code reads from Python code
      dup2(pipe_2[0], STDIN_FILENO);
      // close pipes
      for (n = 0; n < 2; n++)
      {
         close(pipe_1[n]); close(pipe_2[n]); 
      }
      // execute C++ code
      execvp("./a2-ece650", NULL);
      int status;
      waitpid(pid[0], &status, 0);
      exit(0);
   }
   
   pid[3] = fork();
   if (pid[3] == 0)
   {
      // console writes to C++ code
      int status;
      dup2(pipe_2[1], STDOUT_FILENO);
      for (n = 0; n < 2; n++)
      {
         close(pipe_1[n]); 
      }
      string line;
         while (1)
         {
            getline(cin, line);
            
            if (!cin)
            {
               if (cin.eof()) 
               {
                  kill(pid[2], SIGKILL);
                  waitpid(pid[2], &status, 0);
                  break;
               }
            }
            std::size_t path_ = line.find("s");
            std::size_t space = line.find(" ");
            if ((path_ != -1) && ((space != -1)))
            {
               std::cout << line << std::endl;
            }
            else 
            {
               std::cerr << "Error: invalid command use s command Example 's 2 5' \n";
            }
            
         }
   }
   sleep(1);
   for (n = 0; n < 2; n++)
   {
      close(pipe_1[n]); close(pipe_2[n]);
   }
   
   int status;

   while ((waitpid(pid[2], &status, WNOHANG) != pid[2])&& (waitpid(pid[0], &status, WNOHANG) != pid[0]));
   
   for (n = 0; n < 4; n++)
   {
      kill(pid[n], SIGKILL);
      waitpid(pid[n], &status, 0);
   }
   return 0;
}

