/*
Class: CPSC 346-01
Team Member 1: Nicholas Walker
Team Member 2: N/A 
GU Username of project lead: nwalker
Pgm Name: proj4.c 
Pgm Desc: Outline of a shell
Usage: ./a.out 
history will list 10 previous commands
!! causes the most recent command to be executed
!N causes nth command in history to be executed
*/

#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>


#define MAX_LINE 80
#define TRUE 80

char** getInput();
char** parseInput(char*);
void dispOutput(char**);
void childProcess(char*, char**);
void displayHistory(char**, int);
char** parseHistory(char**, int);
void histcpy(char**, char*, int);
char* parseStr(char*);


int main(int argc, char* argv[])
{
  //A pointer to an array of pointers to char.  In essence, an array of 
  //arrays.  Each of the second level arrays is a c-string. These hold
  //the command line arguments entered by the user.
  //as exactly the same structure as char* argv[]
  char **args; 
  char* first; 
  int save1 = 0;
  char** hist = (char**)malloc(MAX_LINE);
  

  while (TRUE)
  {
     printf("myShell> ");
     fflush(stdout);
     args = getInput();
     first = *args++;
     histcpy(hist, first, save1);
     if(save1 <= 9)
     	save1++;
     dispOutput(args);
     childProcess(args[0], args);
     char** i1 = args;
     //char* itchar = *i1++;

     if(args != NULL)
     {
     	dispOutput(args);
     	if(!strcmp(args[0], "quit"))
     	{
     		printf("\n==> done \n");
     		break;
     	}
     	else if(!strcmp(args[0], "!!"))
     	{
     		childProcess(hist[1], args);
     	}
     	else if(!strcmp(args[0], "cd"))
     	{
     		chdir(args[1]);
     	}
     	else if(!(strcmp(args[0], "history")))
     	{
     		displayHistory(hist, save1);
     	}
     	
     	
     	else if(args[0][0] == '!')
     	{
     		char* str = *args;
     		*str++;
     		int a = atoi(str);
     		if(a < save1 + 1)
     		{
     			char* temp = parseStr(hist[a]);
     			char** newargs = parseInput(temp);
     			*newargs++;
     			childProcess(newargs[0], newargs);
     		}
     		else
     		{
     			printf("\n==> invalid record\n");
     		}
     	}
     }


         //printf("%s\n", args);
     //if the user has entered "quit" break out of the loop.
     //break;
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
    char* inp = (char*) malloc(MAX_LINE);
    //printf("Enter text <= 80 characters in length");
    char* start = inp;
    char c;
    char** out;
    
    while((c = getchar()) != '\n')
    {
        *inp++ = c;
    }
    *inp = '\0';
    out = parseInput(start);
    return out;

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
    char** terms = (char**) malloc(MAX_LINE);
    char** alt = terms;
    *terms++ = parseStr(inp);
    char* token = strtok(inp, " ");
    *terms++ = token;
    //printf("%s\n", out[0]);
	//printf("%s\n", out[1]);
    while(token != NULL)
    {
        token = strtok(NULL, " ");
        *terms++ = token;
    }
    *terms++ = "\0";
    //*ret++ = "\0";
    terms = alt;
    return terms;
} 
 
/*
Displays the arguments entered by the user and parsed by getInput
*/
void dispOutput(char** args)
{
	char** new = args;
	//printf("%s\n", args[i]);
	while(*new != "\0")
	{
		new++;
	}
}

//forks child from parent shell process
void childProcess(char* command, char** param)
{
    id_t pid;
    char** i = param;
    char* first = *i++;
    //char** new2 = params;
    pid = fork();
    
    if (pid < 0)
    {
        fprintf(stderr, "error in fork");
    }
    else
        if (pid == 0)
        {
            execvp(command, param);
            exit(0);
        }
    else
    {
        wait(NULL);
    }
}

//copies what is in history
void histcpy(char** history, char* new_item, int i)
{
    while(i > 0)
    {
        history[i] = history[i-1];
        i--;
    }
    history[0] = new_item;
}

//returns history command to shell
char** parseHistory(char** history, int i)
{
    printf("a");
    char* selected = parseStr(history[i]);
    printf("%s", selected);
    return parseInput(selected);
}

//accepts a cstring and outputs a copy
char* parseStr(char* source)
{
	char point;
	char* src = source;
    char* to = (char*)malloc(MAX_LINE);
    char* cpy = to;
    
    //char* under = src;
    point = *src++;
    int i = 0;
    while(point != '\0')
    {
        *to = point;
        point = *src++;
        to++;
    }
    return cpy;
}


//prints history
void displayHistory(char** history, int i)
{
    char** print = history;
    int k = i;
    while(k > 1)
    {
        print++;
        k--;
    }
    while(i > 0)
    {
        printf("%d: %s", i, *print);
        print--;
        i--;
    }
}


