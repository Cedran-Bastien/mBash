 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_HISTORY 100

int main(int argc, char *argv[], char *envp[]) {
    char input[1024];
    char cwd[1024];
    char hostname[1024];
    char *username;
    char history[MAX_HISTORY][1024];
    int history_count = 0;

    while (1) {
        // afficher le prompt
        username = getlogin();
        gethostname(hostname, sizeof(hostname));
        getcwd(cwd, sizeof(cwd));
        printf("%s@%s:%s$ ", username, hostname, cwd);
        
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\r\n")] = 0; // enlever les caractères de fin de ligne

        // Ajouter la commande à l'historique
        strcpy(history[history_count++ % MAX_HISTORY], input);

        // Parser la commande et les arguments
        char *command = strtok(input, " ");
        char *args[64];
        int i = 0;

        if (command == NULL) {
            printf("%s", "Veuillez entrer une commande \n");
            continue;
        }

        while (command != NULL) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        args[i] = NULL; // execve attend un tableau d'arguments terminé par NULL

        if (strcmp(args[0], "cd") == 0) {
            // Gérer la commande cd
            if (args[1] == NULL || strcmp(args[1], "~") == 0)
            {
                chdir(getenv("HOME"));
            }
            else
            {
                chdir(args[1]);
            }   
        } else if (strcmp(args[0], "pwd") == 0) {
            // Gérer la commande pwd
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "history") == 0) {
            // Gérer la commande history
            int start = history_count - 10;
            if (start < 0) {
                start = 0;
            }
            for (int i = start; i < history_count; i++) {
                printf("%d %s\n", i + 1, history[i % MAX_HISTORY]);
            }
        }
        else {
            // Vérifier si la commande existe dans les répertoires de la variable PATH
            int found = 0;
            char *path = strdup(getenv("PATH")) ;
            char *saveptr;
            char *inter = strtok_r(path, ":", &saveptr);
            char *Complet=malloc(sizeof(char)*50);
            while (inter != NULL)
            {
                sprintf(Complet,"%s/%s",inter,args[0]);
                if (access(Complet, X_OK) == 0) {
                    found = 1;
                    break;
                }
                inter = strtok_r(NULL, ":", &saveptr);
            }

            if  (!found){
                printf("%s : commande introuvable" , args[0]);
            } else{
                int pid;
                pid= fork();
                // Lancer la commande en utilisant execve
                if (pid == 0)
                {
                    execve(Complet, args, envp);
                }
                else
                {
                    waitpid(pid, NULL, 0);
                }
            }
            
        }
    }

    return 0;
}
