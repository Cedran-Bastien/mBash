#include <stdio.h> 
#include <string.h>
 #include <stdlib.h> 
 #include <unistd.h>
 #include <assert.h> 
 #include <sys/wait.h>
 #define MAXLI 2048 

 char cmd[MAXLI]; 
 char path[MAXLI]; 
 int pathidx; 

 struct command 
 {
    char cmd[MAXLI];
    struct command *next;
 };

 struct command *head = NULL;
 struct command *tail = NULL;

 void add_to_history(char *cmd);
 void print_history();

 void mbash(); 
 void getpath(); 
 void parse_cmd(); 
 void execute_cmd(); 

 int main(int argc, char** argv) { 
    getpath(); 
    while (1) { 
        printf("Commande: ");
        fgets(cmd, MAXLI, stdin);
        parse_cmd(); 
        execute_cmd(); 
    } 
    return 0; 
 } 
 void getpath() {
    char *path_env = getenv("PATH"); 
    strcpy(path, path_env);
    pathidx = strlen(path); 
 }
 void parse_cmd() { 
    char *p = strtok(cmd, " \t\n");
    add_to_history(cmd);
    if (strcmp(p, "cd") == 0)
    { 
        p = strtok(NULL, " \t\n");
        if (p == NULL) {
            chdir(getenv("$HOME"));
        } else {
            chdir(p);
        }

    }
    else if (strcmp(p, "history") == 0)
    {
        print_history();
        return;
    }
//    else if (strcmp(p, "pwd") == 0) {
//        char cwd[MAXLI];
//        if (getcwd(cwd, sizeof(cwd)) != NULL) printf("%s\n", cwd);
//        else perror("getcwd() error"); return; }
//        else if (strcmp(p, "exit") == 0) {
//        exit(0);
//    }
    else 
    { 
        char fullcmd[MAXLI];
        int background = 0; 
    if (p[strlen(p) - 1] == '&')
    { 
        background = 1;
        p[strlen(p) - 1] = '\0';
    } 
    sprintf(fullcmd, "%s/%s", path, p);
    strcpy(cmd, fullcmd);
    if (background) strcat(cmd, " &");
    }
 }
 void add_to_history(char *cmd)
 {
    struct command *new_cmd = malloc(sizeof(struct command));
    strcpy(new_cmd->cmd, cmd);
    new_cmd->next = NULL;
    if (head == NULL) { 
        head = new_cmd;
        tail = new_cmd;
    } else { 
        tail->next = new_cmd; 
        tail = new_cmd; 
    } 
 }
 void print_history() { 
    struct command *current = head;
    int idx = 1;
    while (current != NULL) {
        printf("%d %s\n", idx, current->cmd);
        current = current->next; idx++;
    }
 }
 

 void execute_cmd() { 
    pid_t pid = fork();
    if (pid < 0) { 
        printf("Fork failed\n");
        exit(1); 
    }
    if (pid == 0) {
        char *argv[MAXLI];
        int idx = 0;
        argv[idx] = strtok(cmd, " \t\n");
    while (argv[idx] != NULL) 
    { 
        idx++;
        argv[idx] = strtok(NULL, " \t\n");
    }
        execve()
    //execvp(argv[0], argv);
    printf("Command not found\n");
    exit(1);
    }
    else
    { 
        int status;
        waitpid(pid, &status, 0);
    }
 }
 