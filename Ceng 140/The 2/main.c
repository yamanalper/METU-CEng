#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SIZE 34000

void solve_labyrinth(char** rows,int x,int y,char a){
    
    if (x<0 || y<0 || rows[x][y] != ' ') {
        return;
    }
    rows[x][y] = a;
    solve_labyrinth(rows, x+1, y,a);
    solve_labyrinth(rows, x, y+1,a);
    solve_labyrinth(rows, x-1,y,a);
    solve_labyrinth(rows, x, y-1,a);
}
bool check_exit(char** rows,int x, int y, int length, int line_count){
    int i=0;
    for (i=0;i<length;i++){
        if (rows[0][i] == '*' || rows[i][0] == '*' || rows[line_count-1][i] == '*' || rows[i][length] == '*'){
            return true;
        }
    }
    return false;
}

int main() {
    int count = 0,line_count = 0,i,x,y,length;
    char** rows = NULL;
    char line[MAX_SIZE],ele;
    scanf("%d %d",&x,&y);
    
    while((ele = getchar()) != EOF && count < MAX_SIZE){
        line[count] = ele;
        ++count;
        if (ele == '\n'){
            char* current_line = (char*)malloc(count * sizeof(char));
            length = count-1;
            line[count] = '\0';
            rows = (char **)realloc(rows, (line_count+1) * sizeof(char*));
            strcpy(current_line, line);
            rows[line_count] = current_line;
            line_count++;
            count = 0;
        }
    }
    
    solve_labyrinth(rows,x,y,'*');
    if (check_exit(rows,x,y,length,line_count) == false){
        solve_labyrinth(rows,x,y,'.');
    }
    for(i=0;i<line_count;i++){
        printf("%s",rows[i]);
    }
    return 0;
}


