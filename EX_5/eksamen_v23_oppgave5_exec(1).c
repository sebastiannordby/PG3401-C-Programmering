
#include <stdio.h>

/* The popen() function is a standard C library function that allows a program to open a pipe to 
   a process for reading or writing. The name popen stands for "process open". The function takes 
   two arguments: a string specifying the command to execute, and a string specifying the mode 
   in which the pipe should be opened. The mode string should be either "r" (to open the pipe for 
   reading) or "w" (to open the pipe for writing). The function returns a FILE pointer that can be 
   used to read from or write to the pipe.

   The below function takes a command as argument, executes that command by calling popen, and 
   returns as a string the output of that command, as would have been printed to the terminal.*/

char* ExecuteCommand(const char *pcszCommand) {
   FILE* fp = NULL;
   char *pszReturnString = malloc(MAX_BUFFER_SIZE);
   if (pszReturnString == NULL) return NULL;
   memset(pszReturnString, 0, MAX_BUFFER_SIZE);

   fp = popen(pcszCommand, "r");
   if (fp == NULL) {
      sprintf(pszReturnString, "Error: Failed to execute command");
   }
   else {
      if (fgets(pszReturnString, MAX_BUFFER_SIZE - 1, fp) == NULL) {
         sprintf(pszReturnString, "Error: Failed to read output");
      }
      pclose(fp);
   }
   return pszReturnString;
}

/* IMPORTANT NOTE: This function has alloced a string by malloc, it must be freed BY THE CALLER...*/
