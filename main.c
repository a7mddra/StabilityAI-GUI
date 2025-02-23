#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 200

void cin(char *context) {
    int ln = 0;
    while (1) {
        char c = getchar();
        if (c == '\n') {
            context[ln] = '\0';
            break;
        }
        if (ln < MAX_INPUT - 1) {
            context[ln++] = c;
        }
    }
}

void reply(const char *response) {
    printf("\033[1;31m""-> ""\033[0m");
    fflush(stdout);

    char buffer[256];  
    int index = 0;
    int delay = 90000; 

    for (int i = 0; i < strlen(response); i++) {
        if (response[i] == ' ' || response[i] == '\0') {  
            buffer[index] = '\0';  
            printf("%s ", buffer);
            fflush(stdout);
            usleep(delay);  
            index = 0;  
        } else {
            buffer[index++] = response[i];
        }
    }

    if (index > 0) {  
        buffer[index] = '\0';
        printf("%s", buffer);
    }

    printf("\n");
    fflush(stdout);
}


void processContext(char *context) {
    reply("That sounds like a great idea!\n   Let me work my magic! 🎨✨\n");

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "python3 main.py \"%s\"", context);

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        reply("Oops! Something went wrong while trying to generate your image. 😞 Please try again.\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "done") == 0) {
            reply("Done! Hope you like it! 😃\n   Let me know if you’d like another one.\n");

        }
    }
    pclose(fp);
}


int main() {
    char context[MAX_INPUT];

    printf("\033[H\033[2J");
    reply("Hey there! 👋 I'm your creative AI assistant.\n   Let's turn your ideas into art! 🎨\n");

    while (1) {
        printf("\033[1;32m""-> ""\033[0m");
        fflush(stdout);
        cin(context);

        if (strcmp(context, "exit") == 0 || strcmp(context, "quit") == 0) {
            reply("Alright, take care!\n   If you ever need more art, just call me! 😊\n");
            break;
        }

        processContext(context);
    }

    return 0;
}
