/*
Outline of a shell
*/

#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80
#define TRUE 80

char** getInput();
char** parseInput(char*);
void dispOutput(char**);

int main(int argc, char* argv[])
{
  //A pointer to an array of pointers to char.  In essence, an array of 
  //arrays.  Each of the second level arrays is a c-string. These hold
  //the command line arguments entered by the user.
  //as exactly the same structure as char* argv[]
  char **args;  

  while (TRUE)
  {
     printf("myShell> ");
     fflush(stdout);
     args = getInput();
     dispOutput(args);
     //printf("%s\n", args);
     //if the user has entered "quit" break out of the loop.
     break;
     //See figure 3.36 on p. 158.  Do items 1, 2, 3 
  }
   return 0;
}

/*
Reads input string from the key board.   
invokes parseInput and returns the parsed input to main
*/
char** getInput()
{
	char* inp = (char*)malloc(MAX_LINE); //dynamically allocate memory
 printf("Enter text <= 80 characters in length\n");
 scanf("%[^\n]s",inp); //while the input is not null terminator, read
 //printf("%s\n", inp);
 return parseInput(inp);

}  

/*
inp is a cstring holding the keyboard input
returns an array of cstrings, each holding one of the arguments entered at
the keyboard. The structure is the same as that of argv
Here the user has entered three arguements:
myShell>cp x y
*/ 
char** parseInput(char* inp)
{
	int i = 0;
	char* token = strtok(inp, " ");
	char** ret = (char**)malloc(MAX_LINE);
	while(token != NULL)
	{
		token = strtok(NULL, " ");
		*ret++ = token;
		
	}
	//printf("%s\n", out[0]);
	//printf("%s\n", out[1]);
	*ret++ = "\0";
	return ret;
} 
 
/*
Displays the arguments entered by the user and parsed by getInput
*/
void dispOutput(char** args)
{
	char** print = args;
	//printf("%s\n", args[i]);
	while(*print != "\0")
	{
		print++;
	}
}  
