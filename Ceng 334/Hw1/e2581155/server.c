#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "game_structs.h"
#include "print_output.h"
#include <time.h>


#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, 0, fd)

typedef struct {
    int fd_read;
    int fd_write;
    pid_t pid;
    char character;
    char **args;
} player_t;

int grid_width, grid_height, streak_size, player_count;
player_t *players = NULL;
gu *updt;


void read_config();
void setup_pipes();
void fork_players();
bool check_win(char grid[grid_width][grid_height], int x, int y, char c);
void game_loop();
void cleanup();

int main() {
    read_config();
    setup_pipes();
    fork_players();
    game_loop();
    cleanup();
    return 0;
}

void read_config() {
    scanf("%d %d %d %d", &grid_width, &grid_height, &streak_size, &player_count);
    players = malloc((player_count) * sizeof(player_t));
    char buffer[256];
    for (int i = 0; i < player_count; i++) {
        scanf(" %c", &players[i].character);
        int arg_count;
        scanf("%d", &arg_count);
        players[i].args = malloc((arg_count + 2) * sizeof(char *));
        scanf("%s", buffer);
        players[i].args[0] = strdup(buffer);
        for (int j = 1; j <= arg_count; j++) {
            scanf("%s", buffer);
            players[i].args[j] = strdup(buffer);
        }
        players[i].args[arg_count + 1] = NULL;
    }
}

void setup_pipes() {
    for (int i = 0; i < player_count; i++) {
        int fd[2];
        if (PIPE(fd) == -1) {
            perror("socketpair");
            exit(EXIT_FAILURE);
        }
        players[i].fd_read = fd[0];
        players[i].fd_write = fd[1];
    }
}

void fork_players() {
    for (int i = 0; i < player_count; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            close(players[i].fd_read);

            dup2(players[i].fd_write, STDIN_FILENO);
            dup2(players[i].fd_write, STDOUT_FILENO);

            close(players[i].fd_write);

            execv(players[i].args[0], players[i].args);

            perror("execv");
            exit(EXIT_FAILURE);
        }

        players[i].pid = pid;

    }
}

bool check_win(char grid[grid_width][grid_height], int x, int y, char c) {
    int count;

    count = 1;
    for (int i = x - 1; i >= 0 && grid[i][y] == c; i--) count++;
    for (int i = x + 1; i < grid_width && grid[i][y] == c; i++) count++;
    if (count >= streak_size) return true;

    count = 1;
    for (int i = y - 1; i >= 0 && grid[x][i] == c; i--) count++;
    for (int i = y + 1; i < grid_height && grid[x][i] == c; i++) count++;
    if (count >= streak_size) return true;

    count = 1;
    for (int i = 1; x - i >= 0 && y - i >= 0 && grid[x - i][y - i] == c; i++) count++;
    for (int i = 1; x + i < grid_width && y + i < grid_height && grid[x + i][y + i] == c; i++) count++;
    if (count >= streak_size) return true;

    count = 1;
    for (int i = 1; x + i < grid_width && y - i >= 0 && grid[x + i][y - i] == c; i++) count++;
    for (int i = 1; x - i >= 0 && y + i < grid_height && grid[x - i][y + i] == c; i++) count++;
    if (count >= streak_size) return true;

    return false;
}

void game_loop() {
    char grid[grid_width][grid_height];
    int filled = 0;
    bool game = true;
    updt = malloc(sizeof(gu) * grid_height * grid_width);
    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            grid[i][j] = '\0';
        }
    }
    while(game){
        fd_set pset;
        int max_fd = 0;
        FD_ZERO(&pset);
        for(int i = 0; i < player_count; i++){
            FD_SET(players[i].fd_read, &pset);
            if(players[i].fd_read > max_fd) max_fd = players[i].fd_read;
        }
        select(max_fd+1, &pset, NULL, NULL, NULL);
        struct timespec ts = {0, 1000000}; // 1 ms
        nanosleep(&ts, NULL);
        for(int i = 0; i < player_count; i ++){
            int n;
            cm mes;
            if(FD_ISSET(players[i].fd_read, &pset)){
                n = read(players[i].fd_read, &mes, sizeof(cm));
                cmp out;
                out.client_message = &mes;
                out.process_id = players[i].pid;
                print_output(&out, NULL, NULL, 0);
                if (n > 0 && mes.type == START) {
                    sm reply;
                    smp rep;
                    reply.type = RESULT;
                    reply.filled_count = filled;
                    reply.success = 0;
                    rep.server_message = &reply;
                    rep.process_id = players[i].pid;
                    write(players[i].fd_read, &reply, sizeof(sm));
                    print_output(NULL, &rep, updt, filled);
                }
                else if (n > 0 && mes.type == MARK){
                    int x = mes.position.x;
                    int y = mes.position.y;
                    if(x < grid_width && x >= 0 && y < grid_height && y >= 0 && grid[x][y] == '\0'){
                        sm marked;
                        smp rep;
                        grid[x][y] = players[i].character;
                        updt[filled].character = players[i].character;
                        updt[filled].position.x = x;
                        updt[filled].position.y = y;
                        filled++;
                        marked.type = RESULT;
                        marked.filled_count = filled;
                        marked.success = 1;
                        rep.process_id = players[i].pid;
                        rep.server_message = &marked;
                        write(players[i].fd_read, &rep, sizeof(sm));
                        print_output(NULL, &rep, updt, filled);
                        if (check_win(grid,x ,y, players[i].character)) {
                            sm end;
                            end.type = END;
                            for (int n = 0; n < player_count; n++) {
                                smp endm;
                                endm.server_message = &end;
                                endm.process_id = players[n].pid;
                                write(players[n].fd_read, &end, sizeof(sm));
                                print_output(NULL, &endm, NULL, 0);
                            }
                            printf("Winner: Player%c\n", players[i].character);
                            game = false;
                            break;
                        }
                        if(filled == grid_width * grid_height){
                            sm end;
                            end.type = END;
                            for (int n = 0; n < player_count; n++){
                                smp endm;
                                endm.server_message = &end;
                                endm.process_id = players[n].pid;
                                write(players[n].fd_read, &end, sizeof(sm));
                                print_output(NULL, &endm, NULL, 0);
                            }
                            printf("Draw\n");
                            game = false;
                            break;
                        }

                    }
                    else{
                        sm fail;
                        smp failed;
                        fail.type = RESULT;
                        fail.success = 0;
                        fail.filled_count = filled;
                        failed.server_message = &fail;
                        failed.process_id = players[i].pid;
                        write(players[i].fd_read, &failed, sizeof(sm));
                        print_output(NULL, &failed, updt, filled);
                    }
                }
            }
            if(!game) break;
        }
    }
}

void cleanup() {
    for (int i = 0; i < player_count; i++) {
        close(players[i].fd_read);
        close(players[i].fd_write);
    }
    for (int i = 0; i < player_count; i++) {
        waitpid(players[i].pid, NULL, 0);
    }
    for (int i = 0; i < player_count; i++) {
        for (int j = 0; players[i].args[j] != NULL; j++) {
            free(players[i].args[j]);
        }
        free(players[i].args);
    }
    free(players);
    free(updt);
}
