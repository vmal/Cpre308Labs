/*
Vaibhav Malhotra
Project 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define bufferSize 64
#define delim " \t\r\n\a"


/*
This function will exit out of shell.
*/
int exit_func(char **args)
{
    return 0;
}
/*
Prints the process id of the current process.
*/
int pid_func(char **args)
{
    printf("Process ID is: %d\n", getpid());
    return 1;
}
/*
Prints the parent process id of the current process.
*/
int ppid_func(char **args)
{
    printf("Parent Process ID is: %d\n", getppid());
    return 1;
}
/*
Changes to a different directory the user has specified to. 
*/
int cd_func(char **args)
{
    char buffer[600];
    if(args[1] == NULL)
    {
        chdir(getenv("HOME"));
        printf("You changed to Home directory: %s\n",getcwd(buffer, sizeof(buffer)));
    }
    else
    {
        if(chdir(args[1]) == 0)
        {
            printf("You changed to the following directory: %s\n",getcwd(buffer, sizeof(buffer)));            
            
        }
        else
            perror("error:");
            
    }

    return 1;
}
/*
Shows the current working directory the user is in.
*/
int pwd_func(char **args)
{
    char buffer[600];

    if(getcwd(buffer, sizeof(buffer)) != NULL)//you have to get the current dir
        printf("Current working directory: %s\n", buffer);//print it out
    else
        perror("getcwd() error");

    return 1;
}
/*
Sets the value of the variable.
*/
int set_func(char **args)
{
    if(args[2] == NULL)
    {
        setenv(args[1], NULL, 1);
    }
    else
    {
        setenv(args[1], args[2], 1);
    }

    return 1;
}
/*
Gets the value of the variable.
*/
int get_func(char **args)
{
    printf("Value of Variable is: %s\n",getenv(args[1]));
}
//This function basically forks a current process and keeps a track of the parent and the child id. It also makes sure that the child process is 
//finished before the parent process.
void ufunx(char **args){

    pid_t pid, ppid;
    int status;

    pid = fork();//it creates a child process
    if (pid == 0) {
        printf("[%d] %s\n", getpid(), args[0]);
        if (execvp(args[0], args) == -1) {
            perror("error");
        }
        exit(EXIT_FAILURE);

    } else if (pid < 0) {
        perror("error");

    } else {
        do {
            ppid = waitpid(pid, &status, WUNTRACED);//the parent will wait for child
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    if (WIFEXITED(status)){
        printf("[%d] %s EXIT %d\n", pid, args[0], WEXITSTATUS(status));
    }
    if(WIFSIGNALED(status))
        printf("[%d] %s EXIT %d\n", pid, args[0], WTERMSIG(status));
    
}

/*
This function is used for comparing the first keyword from args variable and then calling the associated function to execute an appropriate command.
*/
int execute(char **args)
{
    int status;
    if(args[0]==NULL)
        return 1;

    if(strcmp(args[0], "exit") == 0)
       status = exit_func(args);
    else if(strcmp(args[0], "pid") == 0)
       status = pid_func(args);
    else if(strcmp(args[0], "ppid") == 0)
       status = ppid_func(args);
    else if(strcmp(args[0], "cd") == 0)
       status = cd_func(args);
    else if(strcmp(args[0], "pwd") == 0)
       status = pwd_func(args);  
    else if(strcmp(args[0], "set") == 0)
       status = set_func(args);  
    else if(strcmp(args[0], "get") == 0)
       status = get_func(args);  
    // else if(strcmp(args[0], "sleep") == 0)
    //    status = sleep_func(args);  
    else
        ufunx(args);

    return status;
}
/*
This function splits the string into a char array of substrings.
*/
char **split(char *in)
{
    int size = bufferSize;
    int pos = 0;
    char **list = malloc(bufferSize * sizeof(char*));
    char *token;

    if(!list)
    {
        fprintf(stderr, "size error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(in, delim);

    while(token != NULL)
    {

        list[pos] = token;
        pos++;

        if(pos >= size)
        {
            size+= bufferSize;
            list = realloc(list, size * sizeof(char*));

            if(!list)
            {
        
                fprintf(stderr, "size error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    list[pos] = NULL;
    return list;
}
/*
The main function takes in the user commands and then splits the user commands into different words of array and then passes the char array of commands to execute
function to execute the commands.
*/

int main(int argc,char **argv)
{

   int c=0; 
   int status=1;
   char shell[400] = "308sh> ";
   while((c=getopt(argc, argv, "p:")) != EOF) //This is for taking a prompt from user
   {    
       switch(c)
       {
           case 'p':
           strcpy(shell, optarg);
           strcat(shell,"> ");
           break;
       }
   }

   while(status) //runs in infinite loop until status becomes 0.
   {
       char *userInput[500];
       char **cmds;
       printf("%s",shell);

       fgets(userInput,sizeof(userInput),stdin);
       //printf("%s",userInput);
       cmds = split(userInput);
       status = execute(cmds); //executes commands.
       
       free(cmds);
       
   }
    
}

