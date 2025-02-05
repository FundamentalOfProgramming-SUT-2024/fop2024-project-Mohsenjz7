#include <stdio.h>
#include <ctype.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h>

#define height 50
#define width 100
#define max_room 12
int difficulty;
int ahang;
int DELAY = 1000000; 
typedef struct Monster{
    char name;
    int attack;
    int health;
    int x;
    int y;
    int iskill;
}Monster;
typedef struct NewPlayer{
    int score;
    char name[50];
    int gold;
}NewPlayer;
typedef struct Itemaslahe{
    char name[10];
    int attack;
    
}Itemaslahe;
typedef struct Itemfood{
    char name[10];
    int attack;
    
}Itemfood;
typedef struct Itemtel{
    char name[10];
    int attack;
    int speed;
    int health;
}Itemtel;
typedef struct position{
    int x;
    int y;
}position;
typedef struct Player{
    int xpos;
    int ypos;
    int health;
    int experiments;
    int gold;
    int food;
    int deffood;
    Itemaslahe item_aslahe[40];
    int count;
    int  count_aslahe;
    char gun;
    Itemfood Food[5];
    int count_food;
    Itemtel Telesm[30];
    char telesm;
    int count_telesm;
}Player; 
typedef struct{
    int xPosition;
    int yPosition;
    int connected;
}Door;
typedef struct Room{
    int xPos;
    int yPos;
    int tool;
    int arrz;
    Door door[5];
    int tedad_door;
}Room;
typedef struct {
    Monster *monster;
    Room *rooms;
    int num_rooms;
    char map[height+1][width+1]; 
    int monster_count;
    int current_room;
}Level;
typedef struct Game{
    int difficulty;
    Player player;
    Level *level;
    int current_level;
    int score;
    int iswin;
}Game;
int sehat_karbar(char username[100]);
int sehat_password(char password[100]);
int sehat_email(char email[100]);
void createuser();
int  login();
void  guestlogin();
void show_menu();
void initialize_map();
void generate_map();
void display_map();
int gamelooping(Game *game , int i ,int  l,const char *filename);
void game();
void create_level(Game *game);
position *handle_input(int key,Game *game);
void update_position(position * new_position);
void zibaiii();
void makefluent(position *new_position,Game *game );
void connectDoors(Game *game) ;
void draw_corridor(int x1, int y1, int x2, int y2,Game *game);
void generate_item(Game *game);
void zibaiiii();
void make_hallway(Game *game);
void desplay_lab(Game *game);
void new_room(Game *game ,int room_num);
void load_game_menu(const char *filename , int i,char username[30]);
void aslahe_bar(Game *game);
void food_bar(Game *game);
void hide_map(Game *game);
void move_monster(Game *game);
void battle(Game *game,position *new_position);
void saveGame(Game *game, const char *filename);
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
int main() {
    setlocale(LC_CTYPE,"");
    initscr();
    cbreak();
    keypad(stdscr, TRUE);   
    show_menu();
    endwin();
    return 0;
}
int sehat_karbar(char username[100]) {
    FILE *fptr = fopen("user.txt", "r");
    if (fptr == NULL) {
        return 0;
    }

    char line[100];
    char username_file[100];
    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%s", username_file);
        if (strcmp(username_file, username) == 0) {
            fclose(fptr);
            return 1;
        }
    }

    fclose(fptr);
    return 0;
}
int sehat_password(char password[100]) {
    int length = strlen(password);
    int found = 1;
    int sum = 0;
    if (length >= 7) {
        for (int i = 0; i < length; i++) {
            sum += isupper(password[i]);
        }
        if (sum == 0) {
            found = 0;
        }
        sum = 0;
        for (int i = 0; i < length; i++) {
            sum += islower(password[i]);
        }
        if (sum == 0) {
            found = 0;
        }
        sum = 0;
        for (int i = 0; i < length; i++) {
            sum += isdigit(password[i]);
        }
        if (sum == 0) {
            found = 0;
        }
    } else {
        found = 0;
    }
    return found;
}
int sehat_email(char email[100]) {
    int length = strlen(email);
    int at_found = 0, dot_found = 0;
    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            at_found = 1;
            if (!isalnum(email[i - 1])) return 0;
        }
        if (at_found && email[i] == '.') {
            if (isalnum(email[i + 1])) {
                dot_found = 1;
                break;
            }
        }
    }
    return at_found && dot_found;
}
void createuser() {
    char username[100];
    clear();
    start_color();
    int addad = rand() % 3 + 1;
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_WHITE);
    bkgd(COLOR_PAIR(addad));
    attron(A_BOLD);
    mvprintw(LINES / 2 - 3, (COLS - strlen("Please Enter Your Username: ")) / 2, "Please Enter Your Username: ");
    attroff(A_BOLD);
    refresh();
    echo();
    getstr(username);

    if (sehat_karbar(username)) {
        clear();
        attron(A_BOLD);
        mvprintw(LINES / 2 - 2, (COLS - strlen("Username already exists.")) / 2, "Username already exists.");
        attroff(A_BOLD);
        refresh();
        getch();
        return;
    }

    echo();
    char password[100];
    attron(A_BOLD);
    mvprintw(LINES / 2 - 1, (COLS - strlen("Please Enter Your Password: ")) / 2, "Please Enter Your Password: ");
    attroff(A_BOLD);
    refresh();
    getstr(password);
    

    if (!sehat_password(password)) {
        clear();
        attron(A_BOLD);
        mvprintw(LINES / 2, (COLS - strlen("Password Must Be At Least 7 Characters And Include Uppercase, Lowercase, And Digits.")) / 2, "Password Must Be At Least 7 Characters And Include Uppercase, Lowercase, And Digits.");
        attroff(A_BOLD);
        refresh();
        getch();
        return;
    }

    char email[100];
    attron(A_BOLD);
    mvprintw(LINES / 2 + 1, (COLS - strlen("Please Enter Your Email: ")) / 2, "Please Enter Your Email: ");
    attroff(A_BOLD);
    refresh();
    getstr(email);

    if (!sehat_email(email)) {
        clear();
        attron(A_BOLD);
        mvprintw(LINES / 2 + 2, (COLS - strlen("Your Email is Not Correct!")) / 2, "Your Email is Not Correct!");
        attroff(A_BOLD);
        refresh();
        getch();
        return;
    }

    FILE *fptr = fopen("user.txt", "a");
    fprintf(fptr, "%s %s %s\n", username, password, email);
    fclose(fptr);

    char filename[150];
    sprintf(filename, "%s.txt", username);
    FILE *userFile = fopen(filename, "w");
    if (userFile) {
        fprintf(userFile, "Gold: 0\nScore: 0\n");
        fclose(userFile);
    }

    clear();
    attron(A_BOLD);
    mvprintw(LINES / 2 + 4, (COLS - strlen("A new user has been created. :D")) / 2, "A new user has been created. :D");
    attroff(A_BOLD);
    refresh();
    getch();

    clear();
    load_game_menu(filename , 1,username);
}
int login() {
    echo();
    clear();
    FILE *fptr;
    char new_username[100];
    start_color();
    int addad=rand()%3+1;
    init_pair(1,COLOR_YELLOW,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    bkgd(COLOR_PAIR(addad));
    attron(A_BOLD);
    mvprintw(LINES / 2 - 2, (COLS - strlen("Please enter your username: ")) / 2, "Please enter your username: ");
    attroff(A_BOLD);
    refresh();
    
    getstr(new_username);
    if (!sehat_karbar(new_username)) {
        clear();
        attron(A_BOLD);
        mvprintw(LINES / 2 - 1, (COLS - strlen("Username does not exist!")) / 2, "Username does not exist!");
        attroff(A_BOLD);
        refresh();
        getch();
        return 0;
    }
    
    char new_password[100];
    attron(A_BOLD);
    mvprintw(LINES / 2, (COLS - strlen("Please enter your password: ")) / 2, "Please enter your password: ");
    attroff(A_BOLD);
    refresh();
    getstr(new_password);
    
    fptr = fopen("user.txt", "r");

    char line[100];
    char old_username[100], old_password[100];
    int found = 0;
    while (fgets(line, sizeof(line), fptr)) {
        sscanf(line, "%s %s", old_username, old_password);
        if ((strcmp(old_username, new_username) == 0) && (strcmp(old_password, new_password) == 0)) {
            clear();
            int i= 0;
            while(i<3){
                addad=rand()%3+1;
                attron(A_BOLD);
                mvprintw(LINES / 2 + 2, (COLS - strlen("Login successful.")) / 2, "Login successful.");
                attroff(A_BOLD);
                bkgd(COLOR_PAIR(addad));
                usleep(DELAY);
                refresh();
                i++;
                }
            getch();
            found = 1;
            break;
        }
    }

    if (!found) {
        clear();
        mvprintw(LINES / 2 + 2, (COLS - strlen("Invalid username or password!")) / 2, "Invalid username or password!");
        getch();
        return 0;
        refresh();
        
        
    }
    char filename[150];
    fclose(fptr);
    sprintf(filename, "%s.txt", new_username);
    load_game_menu(filename , 0,new_username);
    
}
void guestlogin() {
    clear();
    char username[100];
    start_color();
    int addad=rand()%3+1;
    init_pair(1,COLOR_YELLOW,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_BLUE,COLOR_WHITE);
    attron(A_BOLD);
    mvprintw(LINES / 2 - 2, (COLS - strlen("Please enter your guest username: ")) / 2, "Please enter your guest username: ");
    attroff(A_BOLD);
    bkgd(COLOR_PAIR(addad));
    refresh();
    getstr(username);
    clear();
    attron(A_BOLD);
    mvprintw(LINES / 2 - 1, (COLS - strlen("Logged in as guest: ")) / 2, "Logged in as guest: %s", username);
    attroff(A_BOLD);
    refresh();
    getch();
    
    game(1,"save.txt");
    return ;
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void game(int i ,const char *filename) {
    
    Game *game = malloc(sizeof(Game));
    game->iswin=0;
    game->current_level = 0;
    game->level = malloc(10 * sizeof(Level));
    game->level->rooms=malloc(13*sizeof(Room));
    game->level->monster=malloc(20*sizeof(Monster));
    game->score=0;
    if (gamelooping(game , 0 ,i , filename) == -1) {
        clear();
        mvprintw(LINES / 2, (COLS - strlen("Exiting to menu...")) / 2, "Exiting to menu...");
        saveGame(game, filename);
        refresh();
        usleep(2000000);
        show_menu();
    }

    free(game->level);
    free(game);
}
void last_level(Game* game){
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;

    game->level[game->current_level].monster=malloc(20*sizeof(Monster));
    initialize_map(game);
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            if (i == 0 || i == 30 - 1) {
                game->level[game->current_level].map[10 + i][10 + j] =  'O';
            }else {
                if (j == 0 || j == 40- 1) {
                game->level[game->current_level].map[10 + i][10 + j] = '|';
                }else {
                game->level[game->current_level].map[10 + i][10 + j] = '.';
                }
            }
        }
    }
    game->player.xpos=12;
    game->player.ypos=12;
    game->level[game->current_level].map[12][12]='@';
    game->level[game->current_level].map[27][36]='#';
    int count =0;
    int monster_s= rand() % 5+2;

    while (monster_s> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 's';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'S';
            game->level[game->current_level].monster[count].attack=6;
            game->level[game->current_level].monster[count].health=20;
            game->level[game->current_level].monster[count].iskill=0;

            count++;
            monster_s--;
        }
    }
    
    

    int monster_u= rand() % 5+2;

    while (monster_u> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'U';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'U';
            game->level[game->current_level].monster[count].attack=10;
            game->level[game->current_level].monster[count].health=25;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_u--;
        }
    }
    game->level[game->current_level].monster_count=count;
    display_map(game);
    int gold_count = rand()%15+2;
    while (gold_count > 0) {
        int i = rand() % 40;
        int j = rand() % 30;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'G'; 
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, "☆");
            attroff(COLOR_PAIR(2));
            gold_count--;
        }
    }
    int black_gold_count = rand()%10+2;
    while (black_gold_count > 0) {
        int i = rand() % 40;
        int j = rand() % 30;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'B';
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, "★");
            attroff(COLOR_PAIR(2));
            black_gold_count--;
        }
    }
    
    desplay_lab(game);
    
    // hide_map();
    zibaiiii();
    return ;

}
void create_level(Game *game){

    
    game->level[game->current_level].monster=malloc(20*sizeof(Monster));
    initialize_map(game);
    game->level[game->current_level].rooms=malloc(13*sizeof(Room));
    generate_map(game);
    game->level[game->current_level].current_room=0;
    int random =2;
    while(game->level[game->current_level].map[game->level[game->current_level].rooms[0].yPos+random][game->level[game->current_level].rooms[0].xPos+random]!='.'){
        random=rand()%4+1;
    }
    game->player.xpos = game->level[game->current_level].rooms[0].xPos+random;
    game->player.ypos = game->level[game->current_level].rooms[0].yPos+random;
    
    int count = 0;
    int x;
    int y;
    
    int monster_d= rand() % 3 + 1;

    while (monster_d> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'D';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'D';
            game->level[game->current_level].monster[count].attack=1;
            game->level[game->current_level].monster[count].health=5;
            game->level[game->current_level].monster[count].iskill=0;

            count++;
            monster_d--;
        }
    }
    

    int monster_f= rand() % 3 + 1;

    while (monster_f> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'F';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'F';
            game->level[game->current_level].monster[count].attack=2;
            game->level[game->current_level].monster[count].health=10;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_f--;
        }
    }
    

    int monster_g= rand() % 3 + 1;

    while (monster_g> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'g';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'g';
            game->level[game->current_level].monster[count].attack=4;
            game->level[game->current_level].monster[count].health=15;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_g--;
        }
    }
    int monster_s= rand() % 3;

    while (monster_s> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 's';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'S';
            game->level[game->current_level].monster[count].attack=6;
            game->level[game->current_level].monster[count].health=20;
            game->level[game->current_level].monster[count].iskill=0;

            count++;
            monster_s--;
        }
    }
    
    

    int monster_u= rand() % 2;

    while (monster_u> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'U';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'U';
            game->level[game->current_level].monster[count].attack=10;
            game->level[game->current_level].monster[count].health=25;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_u--;
        }
    }
    game->level[game->current_level].monster_count=count;
    display_map(game);
    generate_item(game);
    // hide_map(game);
    desplay_lab(game);
    
    // hide_map();
    zibaiiii();
    return ;
    
}
void saveGame(Game *game, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        mvprintw(1, 2, "Error: Unable to open file for saving!");
        refresh();
        return;
    }
    
    fwrite(&game->score, sizeof(int), 1, file);
    fwrite(&game->current_level, sizeof(int), 1, file);
    fwrite(&game->player, sizeof(Player), 1, file);
    
    for (int i = 0; i <= game->current_level; i++) {
        Level *level = &game->level[i];
        fwrite(&level->num_rooms, sizeof(int), 1, file);
        fwrite(level->rooms, sizeof(Room), level->num_rooms, file);
        fwrite(&level->monster_count, sizeof(int), 1, file);
        fwrite(level->monster, sizeof(Monster), level->monster_count, file);
        fwrite(level->map, sizeof(char), (height + 1) * (width + 1), file);
    }
    
    fclose(file);
    mvprintw(LINES / 2, (COLS - strlen("Game saved successfully!")) / 2, "Game saved successfully!");
    refresh();
    usleep(1000000);
}

void loadGame(Game *game, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        mvprintw(1, 2, "Error: Unable to open file for loading!");
        refresh();
        return;
    }
    
    fread(&game->score, sizeof(int), 1, file);
    fread(&game->current_level, sizeof(int), 1, file);
    fread(&game->player, sizeof(Player), 1, file);
    
    game->level = (Level *)malloc((game->current_level + 1) * sizeof(Level));
    if (!game->level) {
        fclose(file);
        mvprintw(1, 2, "Error: Memory allocation failed!");
        refresh();
        return;
    }
    
    for (int i = 0; i <= game->current_level; i++) {
        Level *level = &game->level[i];
        fread(&level->num_rooms, sizeof(int), 1, file);
        
        level->rooms = (Room *)malloc(level->num_rooms * sizeof(Room));
        if (!level->rooms) {
            fclose(file);
            mvprintw(1, 2, "Error: Memory allocation failed for rooms!");
            refresh();
            return;
        }
        
        fread(level->rooms, sizeof(Room), level->num_rooms, file);
        fread(&level->monster_count, sizeof(int), 1, file);
        
        level->monster = (Monster *)malloc(level->monster_count * sizeof(Monster));
        if (!level->monster) {
            fclose(file);
            mvprintw(1, 2, "Error: Memory allocation failed for monsters!");
            refresh();
            return;
        }
        
        fread(level->monster, sizeof(Monster), level->monster_count, file);
        fread(level->map, sizeof(char), (height + 1) * (width + 1), file);
    }
    
    fclose(file);
    mvprintw(1, 2, "Game loaded successfully!");
    refresh();
    usleep(100000);
}
int gamelooping(Game *game , int i ,int  l,const char *filename) {
    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;
    int ch;
    noecho();
    curs_set(0);
    position *new_position;
    
    if (game->current_level == 0 && i == 0){
        initialize_map(game);
        generate_map(game);
        display_map(game);
        zibaiii();
        generate_item(game);
        zibaiiii();
        desplay_lab(game);
    }
    if (i == 1){
        display_map(game);
        zibaiiii();
        desplay_lab(game);
    }

    while ((ch = getch()) != 'q') { 
        new_position = handle_input(ch, game);
        if (new_position == NULL) {
            continue;
        }
        if (game->level[game->current_level].map[new_position->y][new_position->x] == '#'){
                    clear();
                    mvprintw(0, (COLS - 10) / 2, "You Won!");
                    mvprintw(1, (COLS - 20) / 2, "   Score: %d", game->score);
                    
                    refresh();
                    usleep(1000000);
                    
                    clear();
                    mvprintw(2, (COLS - 50) / 2, " hvj             .d$$b");
                    mvprintw(3, (COLS - 50) / 2, "                          .' TO$;\\");
                    mvprintw(4, (COLS - 50) / 2, "                         /  : TP._;");
                    mvprintw(5, (COLS - 50) / 2, "                        / _.;  :Tb|");
                    mvprintw(6, (COLS - 50) / 2, "                       /   /   ;j$j");
                    mvprintw(7, (COLS - 50) / 2, "                   _.-\"       d$$$$");
                    mvprintw(8, (COLS - 50) / 2, "                 .' ..       d$$$$;");
                    mvprintw(9, (COLS - 50) / 2, "                /  /P'      d$$$$P. |\\");
                    mvprintw(10, (COLS - 50) / 2, "               /   \"      .d$$$P' |\\^\"l");
                    mvprintw(11, (COLS - 50) / 2, "             .'           `T$P^\"\"\"\"  :");
                    mvprintw(12, (COLS - 50) / 2, "         ._.'      _.'                ;");
                    mvprintw(13, (COLS - 50) / 2, "      `-.-\".-'-' ._.       _.-\"    .-\"");
                    mvprintw(14, (COLS - 50) / 2, "    `.-\" _____  ._              .-\"");
                    mvprintw(15, (COLS - 50) / 2, "   -(.g$$$$$$$b.              .'");
                    mvprintw(16, (COLS - 50) / 2, "     \"\"^^T$$$P^)            .(:");
                    mvprintw(17, (COLS - 50) / 2, "       _/  -\"  /.'         /:/;");
                    mvprintw(18, (COLS - 50) / 2, "    ._.'-'`-'  \")/         /;/;");
                    mvprintw(19, (COLS - 50) / 2, " `-.-\"..--\"\"   \" /         /  ;");
                    mvprintw(20, (COLS - 50) / 2, ".-\" ..--\"\"        -'          :");
                    mvprintw(21, (COLS - 50) / 2, "..--\"\"--.-\"         (\\      .-(\\");
                    mvprintw(22, (COLS - 50) / 2, "  ..--\"\"              -\\(\\/;");
                    mvprintw(23, (COLS - 50) / 2, "    _.                      :");
                    mvprintw(24, (COLS - 50) / 2, "                            ;`-");
                    mvprintw(25, (COLS - 50) / 2, "                           :\\");
                    mvprintw(26, (COLS - 50) / 2, "                           ;");
                    refresh();
                    usleep(1000000);
                    game->iswin = 1;
                    return -1;

        }
        
        if (game->level[game->current_level].map[new_position->y][new_position->x] == '$') {
            game->current_level += 1;
            if(game->current_level == 4){
                clear();
                last_level(game);
                continue; 
                    
            }else{
                clear();
                create_level(game);
                continue; 
            }
        }
            
        
        
        battle(game, new_position);
        makefluent(new_position, game);
        move_monster(game);
        free(new_position);
        
        if (game->player.health < 1){
            clear();
            mvprintw(0, (COLS - 10) / 2, "You Lost!");
            saveGame(game, filename);
            return -1;
        }
        desplay_lab(game);
    }
    
    if (l == 0){
        saveGame(game, filename);
    }
    return -1;
}
void zibaii_room(Game *game, int y, int x) {
    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;
    int room_number = -1; 

    for (int i = 0; i < game->level[game->current_level].num_rooms; i++) {
        for (int j = 0; j < game->level[game->current_level].rooms[i].tedad_door; j++) {
            if ((game->level[game->current_level].rooms[i].door[j].xPosition == x) &&
                (game->level[game->current_level].rooms[i].door[j].yPosition == y)) {
                room_number = i;
                break;
            }
        }
        
    }
    new_room(game, room_number);
    refresh();
    return;
}
void new_room(Game *game, int room_num) {
    srand(time(NULL));
    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;
    int tedad_door;
    int random_door;
    if (room_num==game->level[game->current_level].num_rooms-1){
        tedad_door=game->level[game->current_level].rooms[0].tedad_door;
        random_door=rand()%tedad_door;
        int x = game->level[game->current_level].rooms[0].door[random_door].xPosition;
        int y = game->level[game->current_level].rooms[0].door[random_door].yPosition;
        if (game->level[game->current_level].map[y][x+1]==' '){
            game->player.xpos=x-1;
            game->player.ypos=y;
        }else if (game->level[game->current_level].map[y][x-1]==' '){
            game->player.xpos=x+1;
            game->player.ypos=y;
        }else if (game->level[game->current_level].map[y+1][x]==' '){
            game->player.xpos=x;
            game->player.ypos=y-1;
        }else if (game->level[game->current_level].map[y-1][x]==' '){
            game->player.xpos=x;
            game->player.ypos=y+1;
        }
        attron(COLOR_PAIR(4));
        mvaddch(game->player.ypos + start_row, game->player.xpos + start_col, '@');
        attroff(COLOR_PAIR(4));
        }
    else {
        tedad_door=game->level[game->current_level].rooms[room_num+1].tedad_door;
        random_door=rand()%tedad_door;
        int x = game->level[game->current_level].rooms[room_num+1].door[random_door].xPosition;
        int y = game->level[game->current_level].rooms[room_num+1].door[random_door].yPosition;
        if (game->level[game->current_level].map[y][x-1]=='.'){
            game->player.xpos=x-1;
            game->player.ypos=y;
        }else if (game->level[game->current_level].map[y][x+1]=='.'){
            game->player.xpos=x+1;
            game->player.ypos=y;
        }else if (game->level[game->current_level].map[y+1][x]=='.'){
            game->player.xpos=x;
            game->player.ypos=y+1;
        }else if (game->level[game->current_level].map[y-1][x]=='.'){
            game->player.xpos=x;
            game->player.ypos=y-1;
        }
        
        attron(COLOR_PAIR(4));
        mvaddch(game->player.ypos + start_row, game->player.xpos + start_col, '@');
        attroff(COLOR_PAIR(4));
    }
    
    usleep(DELAY);
    refresh();
    WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
    mvwprintw(win , 1,1,"LOG !!!! welcome to Newroom Bi###");
    wrefresh(win);
    usleep(1000000);
    mvwprintw(win , 1,1,"                                  ");
    wrefresh(win);
    return;
}
void red(Game *game){
    // int start_col = (COLS - width) / 2;
    // int start_row = (LINES - height) / 2;
    // init_pair(1,COLOR_BLACK,COLOR_RED);
    // start_color();
    // WINDOW * new = newwin(20 ,50  , start_row , start_col);
    // wbkgd(new,COLOR_PAIR(1));
    // mvwprintw(new,1,1,"Monster  >>> Trap");
    // wrefresh(new);
    // usleep(DELAY);
    // delwin(new);
    // wrefresh(new);
    // return;
}
void check_position(position * new_position , Game *game){
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    
    if (game->level[game->current_level].map[new_position->y][new_position->x] == '^') {
        game->player.health -= 1;
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == 'G') {
        game->player.gold += 1;
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == 'B'){
        game->player.gold +=5;
    }else if(game->level[game->current_level].map[new_position->y][new_position->x] == '/'){
        
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DO you want Mace ? Y/N (already have it!)");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';
            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
            strcpy(game->player.item_aslahe[game->player.count_aslahe++].name,"/");
            mvwprintw(win , 1,1,"                                            ");

        }
        mvwprintw(win , 1,1,"                                            ");
        wrefresh(win);
        refresh();
    }else if(game->level[game->current_level].map[new_position->y][new_position->x] == '*'){
        
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DO you want Dagger ? Y/N");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';
            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
            strcpy(game->player.item_aslahe[game->player.count_aslahe++].name,"*");
            mvwprintw(win , 1,1,"                                              ");

        }
        mvwprintw(win , 1,1,"                                                 ");
        wrefresh(win);
        refresh();
    }else if(game->level[game->current_level].map[new_position->y][new_position->x] == '~'){
        
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DO you want Magic Wand ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';
            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
            strcpy(game->player.item_aslahe[game->player.count_aslahe++].name,"~");
            mvwprintw(win , 1,1,"                                              ");

        }
        mvwprintw(win , 1,1,"                                                  ");
        wrefresh(win);
        refresh();
    }else if(game->level[game->current_level].map[new_position->y][new_position->x] == '>'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DO you want Normal Arrow ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';
            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
            strcpy(game->player.item_aslahe[game->player.count_aslahe++].name,">");
            mvwprintw(win , 1,1,"                                         ");

        }
        mvwprintw(win , 1,1,"                                                ");
        wrefresh(win);
        refresh();
    }else if(game->level[game->current_level].map[new_position->y][new_position->x] == '='){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DO you want Dagger ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';
            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
            strcpy(game->player.item_aslahe[game->player.count_aslahe++].name,"=");
            mvwprintw(win , 1,1,"                                             ");

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '1'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"Do you want bread ? Y/N  ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            if (game->player.count_food<5){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Food[game->player.count_food++].name,"1");
                mvwprintw(win , 1,1,"                                             ");
            }else {
                mvwprintw(win , 1,1,"You are full!                           ");
                wrefresh(win);
                getchar();
            }

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '2'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"Do you want Kebab ? Y/N  ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            if (game->player.count_food<5){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';

                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Food[game->player.count_food++].name,"2");
                mvwprintw(win , 1,1,"                                             ");
            }else {
                mvwprintw(win , 1,1,"You are full!                                 ");
                wrefresh(win);
                getchar();
            }

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '3'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"Do you want tokhme lobia ? Y/N  ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            if (game->player.count_food<5){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Food[game->player.count_food++].name,"3");
                mvwprintw(win , 1,1,"                                             ");
            }else {
                mvwprintw(win , 1,1,"You are full!                                 ");
                wrefresh(win);
                getchar();
            }

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '4'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"Do you want kebab ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            if (game->player.count_food<5){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
        
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Food[game->player.count_food++].name,"4");
                mvwprintw(win , 1,1,"                                             ");
            }else {
                mvwprintw(win , 1,1,"You are full!                                 ");
                wrefresh(win);
                getchar();
            }

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '5'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"HEALTH TELESM ha ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
        
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Telesm[game->player.count_telesm++].name,"5");
                mvwprintw(win , 1,1,"                                             ");
            

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '6'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"SPEED TELESM ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
        
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Telesm[game->player.count_telesm++].name,"6");
                mvwprintw(win , 1,1,"                                             ");

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }else if (game->level[game->current_level].map[new_position->y][new_position->x] == '7'){
        WINDOW *win =newwin(3 , 50 , start_row+6 , start_col+2 );
        mvwprintw(win , 1,1,"DAMAGE TELESM ? Y/N ");
        wrefresh(win);
        char ch=getchar();
        if (ch == 'Y' || ch== 'y'){
            
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
                mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';
        
                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;
                strcpy(game->player.Telesm[game->player.count_telesm++].name,"7");
                mvwprintw(win , 1,1,"                                             ");
            

        }
        mvwprintw(win , 1,1,"                                           ");
        wrefresh(win);
        refresh();
    }
    return;

}
void makefluent(position *new_position, Game *game) {
    game->player.count++;
    if (game->player.count%10==0){
        game->player.deffood=1;
    }
    
    game->player.food=(game->player.food-1*game->player.deffood)%100;

    if (game->player.food>80){game->player.health+=game->player.deffood*difficulty;}



    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;

    if (new_position == NULL) {
        return;
    }   
    check_position(new_position , game);
    
    if (game->level[game->current_level].map[new_position->y][new_position->x] == '.' || 
        game->level[game->current_level].map[new_position->y][new_position->x] == '^'||game->level[game->current_level].map[new_position->y][new_position->x] == 'G'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == 'B') {
        if (game->level[game->current_level].map[game->player.ypos][game->player.xpos] =='^'){
                game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '^';
                mvaddch(game->player.ypos + start_row, game->player.xpos + start_col, '^');
                
                game->level[game->current_level].map[new_position->y][new_position->x] = '@';

                attron(COLOR_PAIR(4));
                mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
                attroff(COLOR_PAIR(4));
                
                game->player.xpos = new_position->x;
                game->player.ypos = new_position->y;


        }else{
            game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
            mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
            
            game->level[game->current_level].map[new_position->y][new_position->x] = '@';

            attron(COLOR_PAIR(4));
            mvaddch(new_position->y + start_row, new_position->x + start_col, '@');
            attroff(COLOR_PAIR(4));
            
            game->player.xpos = new_position->x;
            game->player.ypos = new_position->y;
        }
    }
    else if (game->level[game->current_level].map[new_position->y][new_position->x] == '+') {
        game->level[game->current_level].map[game->player.ypos][game->player.xpos] = '.';
        mvaddnstr(game->player.ypos + start_row, game->player.xpos + start_col, "∙",-1);
        zibaii_room(game, new_position->y, new_position->x);
    }
    refresh();
}
position *handle_input(int key,Game *game) {
    keypad(stdscr, TRUE);
    curs_set(0);

    position *new_position = (position *)malloc(sizeof(position));
    if (new_position == NULL) {
        return NULL;
    }

    int x = game->player.xpos;
    int y = game->player.ypos;
    new_position->x = x;
    new_position->y = y;

    switch (key) {
        case 'w': 
        case 'W':
            new_position->y -= 1;
            break;
        case 's': 
        case 'S':
            new_position->y += 1;
            break;
        case 'A':
        case 'a': 
            new_position->x -= 1;
            break;
        case 'D':
        case 'd':
            new_position->x += 1;
            break;
        case 'z':
        case 'Z':
            new_position->x -=1;
            new_position->y -=1;
            break;
        case 'e':
        case 'E':
            new_position->x +=1;
            new_position->y -=1;
            break;
        case 'x':
        case 'X':
            new_position->x -=1;
            new_position->y +=1;
            break;
        case 'c':
        case 'C':
            new_position->x +=1;
            new_position->y +=1;
            break;
        case 'i':
        case 'I':
            aslahe_bar(game);
            break;
        case 'o':
        case 'O':
            food_bar(game);
            break;
        
        default:
            break;
    }

    if (new_position->x >= 0 && new_position->x < width &&
        new_position->y >= 0 && new_position->y < height &&
        (game->level[game->current_level].map[new_position->y][new_position->x] == '.'||game->level[game->current_level].map[new_position->y][new_position->x] == '+'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '#'||game->level[game->current_level].map[new_position->y][new_position->x] == '^'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '$'||game->level[game->current_level].map[new_position->y][new_position->x] == 'B'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == 'G'||game->level[game->current_level].map[new_position->y][new_position->x] == '/'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '*'||game->level[game->current_level].map[new_position->y][new_position->x] == '~'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '>'||game->level[game->current_level].map[new_position->y][new_position->x] == '='
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '1'||game->level[game->current_level].map[new_position->y][new_position->x] == '2'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '3'||game->level[game->current_level].map[new_position->y][new_position->x] == '4'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '5'||game->level[game->current_level].map[new_position->y][new_position->x] == '6'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == '7'||game->level[game->current_level].map[new_position->y][new_position->x] == 'D'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == 'F'||game->level[game->current_level].map[new_position->y][new_position->x] == 'g'
        ||game->level[game->current_level].map[new_position->y][new_position->x] == 'S'||game->level[game->current_level].map[new_position->y][new_position->x] == 'U')) {
        return new_position;
    }

    free(new_position);
    return NULL;
}
void zibaiii(){

    start_color();
    int addad=rand()%3+1;
    init_pair(1,COLOR_YELLOW,COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();
}
void zibaiiii(){  
    char *str = "✓";
    char *circle = "◯"; 
    char *black_circle = "●"; 
    char *white_circle = "⚪️";
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    for (int j= 0 ; j<height-10 ;j++){
        mvaddnstr(j+start_row+5,start_col,white_circle,-1);
    }
    for (int j= 0 ; j<height-10;j++){
        mvaddnstr(j+start_row+5,start_col+width,white_circle,-1);
    }
    for (int i= 0 ; i<(width)/2 ;i++){
        mvaddnstr(start_row+5,i+start_col,white_circle,-1);
    }
    for (int i= 0 ; i<(width+2)/2 ;i++){
        mvaddnstr(start_row+height-5,i+start_col,white_circle,-1);
    }
    refresh();

}
void generate_item(Game *game) {
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2,COLOR_YELLOW , COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA , COLOR_BLACK);
    int count = 1;
    while (count == 1) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.') {
            int stairwell_nearby = 0;

            for (int l = -10; l <= 10; l++) {
                for (int s = -10; s <= 10; s++) {
                    int x = i + s;
                    int y = j + l;

                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        if (game->level[game->current_level].map[y][x] == '@') {
                            stairwell_nearby = 1;
                            break;
                        }
                    }
                }
                if (stairwell_nearby)
                    break;
            }

            if (!stairwell_nearby) {
                
                game->level[game->current_level].map[j][i] = '$';
                attron(COLOR_PAIR(1));
                mvaddch(j + start_row, i + start_col, '$');
                attroff(COLOR_PAIR(1));
                refresh();
                count = 0;
                
            }
        }
    }
    //♥

    const char  *food_items[] = {"♥", "♥", "♥"};
    int food_count = rand()%3+1;

    while (food_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            const char *food = food_items[rand() % (sizeof(food_items) / sizeof(food_items[0]))];
            game->level[game->current_level].map[j][i] = '1'; 
            mvaddstr(j + start_row, i + start_col, food);
            food_count--;
        }
    }
    const char  *food_items1[] = {"♥", "♥", "♥"};
    food_count = rand()%1+1;

    while (food_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            const char *food = food_items1[rand() % (sizeof(food_items1) / sizeof(food_items1[0]))];
            game->level[game->current_level].map[j][i] = '2';
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, food);
            attroff(COLOR_PAIR(2));
   
            food_count--;
        }
    }
    const char  *food_items2[] = {"♥", "♥", "♥"};
    food_count = rand()%1+1;

    while (food_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            const char *food = food_items2[rand() % (sizeof(food_items2) / sizeof(food_items2[0]))];
            game->level[game->current_level].map[j][i] = '3';
            attron(COLOR_PAIR(3));
            mvaddstr(j + start_row, i + start_col, food);
            attroff(COLOR_PAIR(3));
   
            food_count--;
        }
    }
    // 
    const char  *food_items3[] = {"♥", "♥", "♥"};
    food_count = rand()%2+1;
    int random=rand()%3+1;
    while (food_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            const char *food = food_items3[rand() % (sizeof(food_items3) / sizeof(food_items3[0]))];
            game->level[game->current_level].map[j][i] = '4';
            attron(COLOR_PAIR(random));
            mvaddstr(j + start_row, i + start_col, food);
            attroff(COLOR_PAIR(random));
   
            food_count--;
        }
    }
    int trap_count = rand() % 6 + 4;

    while (trap_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = '^';
            mvaddnstr(j + start_row, i + start_col, "∙",-1);
            trap_count--;
        }
    }
    int gold_count = rand()%3+2;
    while (gold_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'G'; 
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, "☆");
            attroff(COLOR_PAIR(2));
            gold_count--;
        }
    }
    int black_gold_count = rand()%2+1;
    while (black_gold_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'B';
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, "★");
            attroff(COLOR_PAIR(2));
            black_gold_count--;
        }
    }
    char *weapon_items[] = {"\u2692", "\u2692", "\u2692", "\u2692", "\u2692"};
    int weapon_count = 2;

    while (weapon_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            const char *weapon = weapon_items[rand() % (sizeof(weapon_items) / sizeof(weapon_items[0]))];
            game->level[game->current_level].map[j][i] = '/'; 
            mvaddstr(j + start_row, i + start_col, weapon);
            weapon_count--;
        }
    }
    char *weapon_items1[] = {"\U0001F5E1", "\U0001F5E1", "\U0001F5E1", "\U0001F5E1", "\U0001F5E1"};
    weapon_count = 1;

    while (weapon_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            char *weapon1 = weapon_items1[rand() % (sizeof(weapon_items1) / sizeof(weapon_items1[0]))];
            game->level[game->current_level].map[j][i] = '*'; 
            mvaddstr(j + start_row, i + start_col, weapon1);
            weapon_count--;
        }
    }
    char *weapon_items2[] = {"\U0001FA84", "\U0001FA84", "\U0001FA84", "\U0001FA84", "\U0001FA84"};
    weapon_count = rand()%2;

    while (weapon_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j][i+1] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+') {
            char *weapon2 = weapon_items2[rand() % (sizeof(weapon_items2) / sizeof(weapon_items2[0]))];
            game->level[game->current_level].map[j][i] = '~'; 
            mvaddstr(j + start_row, i + start_col, weapon2);
            weapon_count--;
        }
    }
    char *weapon_items3[] = {"\u27B3", "\u27B3", "\u27B3", "\u27B3", "\u27B3"};
    weapon_count = rand()%2+1;

    while (weapon_count > 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            char *weapon3 = weapon_items3[rand() % (sizeof(weapon_items3) / sizeof(weapon_items3[0]))];
            game->level[game->current_level].map[j][i] = '>'; 
            mvaddstr(j + start_row, i + start_col, weapon3);
            weapon_count--;
        }
    }
    char *weapon_items4[] = {"\u2694", "\u2694", "\u2694", "\u2694", "\u2694"};
    weapon_count = rand()%2+1;

    while (weapon_count > 0) {
        int i = rand() % width;
        int j = rand() % height;
        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            char *weapon4 = weapon_items4[rand() % (sizeof(weapon_items4) / sizeof(weapon_items4[0]))];
            game->level[game->current_level].map[j][i] = '='; 
            mvaddstr(j + start_row, i + start_col, weapon4);
            weapon_count--;
        }
    }
    int spell_count = rand()%2+1;
    
    while (spell_count > 0) {
        int i = rand() % width;
        int j = rand() % height;
        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = '5'; 

            mvaddstr(j + start_row, i + start_col,"♦" );
            spell_count--;
        }
    }
    
    spell_count = rand()%2+1;
    while (spell_count > 0) {
        int i = rand() % width;
        int j = rand() % height;
        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = '6'; 
            attron(COLOR_PAIR(2));
            mvaddstr(j + start_row, i + start_col, "♦");
            attroff(COLOR_PAIR(2));
            spell_count--;
        }
    }
    spell_count = rand()%2;
    while (spell_count > 0) {
        int i = rand() % width;
        int j = rand() % height;
        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = '7'; 
            attron(COLOR_PAIR(3));
            mvaddstr(j + start_row, i + start_col, "♦");
            attron(COLOR_PAIR(3));
            spell_count--;
        }
    }

    refresh();
}
void playerset_up_first(Room **rooms, int room_count, Game *game) {
    srand(time(NULL));
    game->player.experiments = 0;
    game->player.gold = 0;
    game->player.health=100;
    game->player.food=100;
    game->player.deffood=1;
    if(difficulty==1){
        game->player.health = 100;
    }else if(difficulty==2){
        game->player.health=80;
    }else if(difficulty==3){
        game->player.health=60;
    }
    int count = 0;
    int x;
    int y;
    
    int monster_d= rand() % 3 + 1;

    while (monster_d> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'D';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'D';
            game->level[game->current_level].monster[count].attack=1;
            game->level[game->current_level].monster[count].health=5;
            game->level[game->current_level].monster[count].iskill=0;
            count++;
            monster_d--;
        }
    }
    

    int monster_f= rand() % 3 + 1;

    while (monster_f> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'F';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'F';
            game->level[game->current_level].monster[count].attack=2;
            game->level[game->current_level].monster[count].health=10;
            game->level[game->current_level].monster[count].iskill=0;

            count++;
            monster_f--;
        }
    }
    

    int monster_g= rand() % 3 + 1;

    while (monster_g> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'g';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'g';
            game->level[game->current_level].monster[count].attack=4;
            game->level[game->current_level].monster[count].health=15;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_g--;
        }
    }
    int monster_s= rand() % 3;

    while (monster_s> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 's';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'S';
            game->level[game->current_level].monster[count].attack=6;
            game->level[game->current_level].monster[count].health=20;
            game->level[game->current_level].monster[count].iskill=0;


            count++;
            monster_s--;
        }
    }
    
    

    int monster_u= rand() % 2;

    while (monster_u> 0) {
        int i = rand() % width;
        int j = rand() % height;

        if (game->level[game->current_level].map[j][i] == '.'&&game->level[game->current_level].map[j+1][i] != '+'&&game->level[game->current_level].map[j-1][i] != '+'&&game->level[game->current_level].map[j][i-1] != '+'&&game->level[game->current_level].map[j][i+1]!= '+') {
            game->level[game->current_level].map[j][i] = 'U';
            game->level[game->current_level].monster[count].x=i;
            game->level[game->current_level].monster[count].y=j;
            game->level[game->current_level].monster[count].name = 'U';
            game->level[game->current_level].monster[count].attack=10;
            game->level[game->current_level].monster[count].health=25;
            game->level[game->current_level].monster[count].iskill=0;
            

            count++;
            monster_u--;
        }
    }

        
    game->level[game->current_level].monster_count=count;
    game->player.count_aslahe=1;
    strcpy(game->player.item_aslahe[0].name,"/");
    game->player.count=0;
    game->player.gun='/';
    game->player.count_food=0;
    game->player.count_telesm=0;
    int random =2;
    while(game->level[game->current_level].map[game->level[game->current_level].rooms[0].yPos+random][game->level[game->current_level].rooms[0].xPos+random]!='.'){
        random=rand()%4+1;
    }
    int l = game->level[game->current_level].rooms[0].xPos+random;
    int k = game->level[game->current_level].rooms[0].yPos+random;
    game->level[game->current_level].current_room=0;

    game->level[game->current_level].map[k][l] = '@'; 
    game->player.xpos = l; 
    game->player.ypos = k; 
}
void aslahe_bar(Game *game){
    int choice = 0;
    int key;
    start_color();
    char temp[10];
    init_pair(5,COLOR_CYAN,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    WINDOW *win=newwin(17,30,start_row+5,start_col+width+3);
    wattron(win,COLOR_PAIR(1));
    box(win , 0,0);
    wattroff(win,COLOR_PAIR(1));
    int Mace=0;
    for (int  i= 0 ; i<game->player.count_aslahe ;i++){
        if (strcmp(game->player.item_aslahe[i].name,"/")==0){
            Mace++;
        }

    }
    int Dagger=0;
    for (int  i= 0 ; i<game->player.count_aslahe ;i++){
        if (strcmp(game->player.item_aslahe[i].name,"*")==0){
            Dagger+=8;
        }

    }
    int Wand=0;
    for (int  i= 0 ; i<game->player.count_aslahe ;i++){
        if (strcmp(game->player.item_aslahe[i].name,"~")==0){
            Wand+=10;
        }

    }
    int Arrow=0;
    for (int  i= 0 ; i<game->player.count_aslahe ;i++){
        if (strcmp(game->player.item_aslahe[i].name,">")==0){
            Arrow+=20;
        }

    }
    int Sword=0;
    for (int  i= 0 ; i<game->player.count_aslahe ;i++){
        if (strcmp(game->player.item_aslahe[i].name,"=")==0){
            Sword++;
        }

    }
    int HEALTH=0;
    for (int  i= 0 ; i<game->player.count_telesm ;i++){
        if (strcmp(game->player.Telesm[i].name,"5")==0){
            HEALTH++;
        }

    }
    int SPEED=0;
    for (int  i= 0 ; i<game->player.count_telesm ;i++){
        if (strcmp(game->player.Telesm[i].name,"6")==0){
            SPEED++;
        }

    }
    int DAMAGE=0;
    for (int  i= 0 ; i<game->player.count_telesm ;i++){
        if (strcmp(game->player.Telesm[i].name,"7")==0){
            DAMAGE++;
        }

    }
    const char *aslahe_menu[] = {
        "➳ Arrow  ",
        "🗡️ Dagger ",
        "⚔️ Sword  ", 
        "🪄 Magic Wand", 
        "⚒️ Mace   ",
        "🔮 Health Spell ",
        "⚡ Speed spell ",
        "💣 damage spell "
    };
    do {
        werase(win);
        box(win, 0, 0);
        wattron(win, COLOR_PAIR(6));
        mvwprintw(win, 1, 1, "➤ %s: %01d", aslahe_menu[0],Arrow );
        mvwprintw(win, 2, 1, "➤ %s: %01d", aslahe_menu[1], Dagger);
        mvwprintw(win, 4, 1, "➤ %s: %01d", aslahe_menu[2], Sword );
        mvwprintw(win, 5, 1, "➤ %s: %01d", aslahe_menu[3],  Wand);
        mvwprintw(win, 6, 1, "➤ %s: %01d", aslahe_menu[4], Mace );
        mvwprintw(win, 8, 1, "➤ %s: %01d", aslahe_menu[5], HEALTH );
        mvwprintw(win, 9, 1, "➤ %s: %01d", aslahe_menu[6], SPEED );
        mvwprintw(win, 10, 1, "➤ %s: %01d", aslahe_menu[7], DAMAGE );
        switch (choice) {
            case 0:
                wattron(win,A_REVERSE);
                mvwprintw(win, 1, 1, "➤ %s: %01d", aslahe_menu[0],Arrow);
                wattroff(win,A_REVERSE);
                break;
            case 1:
                wattron(win,A_REVERSE);
                mvwprintw(win, 2, 1, "➤ %s: %01d", aslahe_menu[1], Dagger);
                wattroff(win,A_REVERSE);
                break;
            case 2:
                wattron(win,A_REVERSE);
                mvwprintw(win, 4, 1, "➤ %s: %01d", aslahe_menu[2], Sword);
                wattroff(win,A_REVERSE);
                break;
            case 3:
                wattron(win,A_REVERSE);
                mvwprintw(win, 5, 1, "➤ %s: %01d", aslahe_menu[3],  Wand);
                wattroff(win,A_REVERSE);
                break;
            case 4:
                wattron(win,A_REVERSE);
                mvwprintw(win, 6, 1, "➤ %s: %01d", aslahe_menu[4], Mace );
                wattroff(win,A_REVERSE);
                break;
            case 5:
                wattron(win,A_REVERSE);
                mvwprintw(win, 8, 1, "➤ %s: %01d", aslahe_menu[5], HEALTH);
                wattroff(win,A_REVERSE);
                break;
            case 6:
                wattron(win,A_REVERSE);
                mvwprintw(win, 9, 1, "➤ %s: %01d", aslahe_menu[6], SPEED );
                wattroff(win,A_REVERSE);
                break;
            case 7:
                wattron(win,A_REVERSE);
                mvwprintw(win, 10, 1, "➤ %s: %01d", aslahe_menu[7], DAMAGE );
                wattroff(win,A_REVERSE);
                break;
        }
        mvwprintw(win, 12, 3, "▲/▼: move ENTER: choice !");
        mvwprintw(win, 13, 3, "ESC : go out!");
        
        wrefresh(win);
        key = getch();

        switch(key) {
            case KEY_UP:
                choice = (choice > 0) ? choice-1 : 7;
                break;
            case KEY_DOWN:
                choice = (choice < 7) ? choice+1 : 0;
                break;
            case 10: 
                if (choice==0){
                    if (Sword>0){
                        game->player.gun='=';
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==1){
                    if (Dagger>0){
                        game->player.gun='*';
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==2){
                    if (Wand >0){
                        game->player.gun='~';
                    }
                    else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==3){
                    if (Arrow>0){
                        game->player.gun='>';
                    }
                    else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==4){
                    if (Mace>0){
                        game->player.gun='/';
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==6){
                    if (HEALTH>0){
                        game->player.telesm='5';
                        game->player.deffood=2;
                        game->player.count=0;
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==47){
                    if (SPEED>0){
                        game->player.telesm='6';
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==5){
                    if (DAMAGE>0){
                        game->player.telesm='7';

                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }
                delwin(win);
                WINDOW *win1=newwin(17,30,start_row+5,start_col+width+3);
                wrefresh(win1);
                return;
            case 27: 
                delwin(win);
                WINDOW *win2=newwin(17,30,start_row+5,start_col+width+3);
                wrefresh(win2);
                return;
        }
    } while(1);
}
void food_bar(Game *game){
    int choice = 0;
    int key;
    start_color();
    char temp[10];
    init_pair(5,COLOR_CYAN,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    WINDOW *win=newwin(13,45,start_row+5,start_col+width+3);
    wattron(win,COLOR_PAIR(1));
    box(win , 0,0);
    wattroff(win,COLOR_PAIR(1));
    int bread=0;
    for (int  i= 0 ; i<=game->player.count_food ;i++){
        if (strcmp(game->player.Food[i].name,"1")==0){
            bread++;
        }

    }
    int kebab=0;
    for (int  i= 0 ; i<=game->player.count_food ;i++){
        if (strcmp(game->player.Food[i].name,"2")==0){
            kebab++;
        }

    }
    int tokhme_lobia=0;
    for (int  i= 0 ; i<=game->player.count_food ;i++){
        if (strcmp(game->player.Food[i].name,"3")==0){
            tokhme_lobia++;
        }

    }
    int fased=0;
    for (int  i= 0 ; i<=game->player.count_food ;i++){
        if (strcmp(game->player.Food[i].name,"4")==0){
            fased++;
        }

    }
    const char *FOOD_menu[] = {
        "🥖 BREAD   ", 
        "🍖 KEBAB ", 
        "🍇 TOKHME_LOBIA", 
        "🥛 SHIRE_FASEDE  ", 
        
    };
    do {
        werase(win);
        box(win, 0, 0);
        wattron(win, COLOR_PAIR(6));
        mvwprintw(win, 1, 1, "➤ %s: %01d", FOOD_menu[0],  bread);
        mvwprintw(win, 2, 1, "➤ %s: %01d", FOOD_menu[1], kebab);
        mvwprintw(win, 3, 1, "➤ %s: %01d", FOOD_menu[2], tokhme_lobia );
        mvwprintw(win, 4, 1, "➤ %s: %01d", FOOD_menu[3],  fased);
        switch (choice) {
            case 0:
                wattron(win,A_REVERSE);
                mvwprintw(win, 1, 1, "➤ %s: %01d", FOOD_menu[0],bread);
                wattroff(win,A_REVERSE);
                break;
            case 1:
                wattron(win,A_REVERSE);
                mvwprintw(win, 2, 1, "➤ %s: %01d", FOOD_menu[1], kebab);
                wattroff(win,A_REVERSE);
                break;
            case 2:
                wattron(win,A_REVERSE);
                mvwprintw(win, 3, 1, "➤ %s: %01d", FOOD_menu[2], tokhme_lobia );
                wattroff(win,A_REVERSE);
                break;
            case 3:
                wattron(win,A_REVERSE);
                mvwprintw(win, 4, 1, "➤ %s: %01d", FOOD_menu[3],  fased);
                wattroff(win,A_REVERSE);
                break;
        }
        mvwprintw(win, 10, 3, "▲/▼: move ENTER: choice !");
        mvwprintw(win, 11, 3, "ESC : go out!");
        if (game->player.food>100){
            game->player.food=100;
            }
        float food = (float)game->player.food / 100;
        int bar_length = 20;
        mvwprintw(win,7,1,"FOODBAR: [");
        int filled = (int)(food * bar_length);
        int l = 13;
        for (int i = 0; i < filled; i++) {
            mvwprintw(win,7,i+l,"█");
            
        }
        l+=filled;
        for (int i = 0; i < bar_length - filled; i++) {
            mvwprintw(win,7,i+l,"-");
        }
        mvwprintw(win,7, 33,"] %d/%d\n", game->player.health, 100);
        
        wrefresh(win);
        key = getch();

        switch(key) {
            case KEY_UP:
                choice = (choice > 0) ? choice-1 : 3;
                break;
            case KEY_DOWN:
                choice = (choice < 3) ? choice+1 : 0;
                break;
            case 10: 
                if (choice==0){
                    if (bread>0){
                        game->player.health+=5;
                        game->player.food=(game->player.food+5)%100;
                        for (int  i= 0 ; i<=game->player.count_food ;i++){
                            if (strcmp(game->player.Food[i].name,"1")==0){
                                
                                strcpy(game->player.Food[i].name,".");
                                break;
                            }
                        }
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(13,45,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==1){
                    if (kebab>0){
                        game->player.health+=12;
                        game->player.food=(game->player.food+12)%100;

                        for (int  i= 0 ; i<=game->player.count_food ;i++){
                            if (strcmp(game->player.Food[i].name,"2")==0){
                                strcpy(game->player.Food[i].name,".");
                                break;
                            }
                        }
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(13,45,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==2){
                    if (tokhme_lobia >0){
                        game->player.health+=8;
                        game->player.food=(game->player.food+8)%100;

                        for (int  i= 0 ; i<=game->player.count_food ;i++){
                            if (strcmp(game->player.Food[i].name,"3")==0){
                                strcpy(game->player.Food[i].name,".");
                                break;
                            }
                        }
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(13,45,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }else if (choice==3){
                    if (fased>0){
                        game->player.health-=7;
                        game->player.food=(game->player.food-7)%100;

                        for (int  i= 0 ; i<=game->player.count_food ;i++){
                            if (strcmp(game->player.Food[i].name,"4")==0){
                                strcpy(game->player.Food[i].name,".");
                                break;
                            }
                        }
                    }else {
                        delwin(win);
                        WINDOW *win1=newwin(13,45,start_row+5,start_col+width+3);
                        wrefresh(win1);
                        return;
                    }
                }
                game->player.count_food--;
                delwin(win);
                WINDOW *win1=newwin(13,45,start_row+5,start_col+width+3);
                wrefresh(win1);
                return;
            case 27: 
                delwin(win);
                WINDOW *win2=newwin(13,45,start_row+5,start_col+width+3);
                wrefresh(win2);
                return;
        }
    } while(1);



}
void battle(Game *game, position *new_position) {
    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;
    int count = game->level[game->current_level].monster_count;

    for (int i = 0; i < count; i++) {
        if (game->level[game->current_level].monster[i].iskill == 1) {
            continue;
        }

        if (game->level[game->current_level].monster[i].x == new_position->x &&
            game->level[game->current_level].monster[i].y == new_position->y) {

            WINDOW *win = newwin(3, 50, start_row + 6, start_col + 2);
            mvwprintw(win, 1, 1, "you hurt %c !!!", game->level[game->current_level].monster[i].name);
            
            if (game->player.gun == '/') {
                game->level[game->current_level].monster[i].health -= 5;
            } else if (game->player.gun == '*') {
                game->level[game->current_level].monster[i].health -= 12;
            } else if (game->player.gun == '~') {
                game->level[game->current_level].monster[i].health -= 15;
            } else if (game->player.gun == '=') {
                game->level[game->current_level].monster[i].health -= 5;
            } else if (game->player.gun == '>') {
                game->level[game->current_level].monster[i].health -= 10;
            }

            wrefresh(win);
            usleep(1000000);
            mvwprintw(win, 1, 1, "                                            ");
            wrefresh(win);

            if (game->level[game->current_level].monster[i].health > 0) {
                game->player.health -= game->level[game->current_level].monster[i].attack;
                mvwprintw(win, 1, 1, "%c hurt you!!!", game->level[game->current_level].monster[i].name);
            } else {
                mvwprintw(win, 1, 1, "you have killed %c !!! Johnwick bashe", game->level[game->current_level].monster[i].name);
                game->level[game->current_level].monster[i].iskill = 1;
                game->level[game->current_level].map[game->level[game->current_level].monster[i].y][game->level[game->current_level].monster[i].x] = '.';
                mvaddnstr(game->level[game->current_level].monster[i].y + start_row,
                          game->level[game->current_level].monster[i].x + start_col, "∙", -1);
            }

            wrefresh(win);
            usleep(1000000);
            mvwprintw(win, 1, 1, "                                            ");
            wrefresh(win);
            refresh();
        }
    }
}
void move_monster(Game *game) {
    int start_col = (COLS - width) / 2;
    int start_row = (LINES - height) / 2;

    for (int i = 0; i < game->level[game->current_level].monster_count; i++) {
        Monster *monster = &game->level[game->current_level].monster[i];
        int player = 0;
        if (monster->iskill==1){
            continue;
        }
        for (int l = -8; l <= 8; l++) {
            for (int s = -8; s <= 8; s++) {
                int x = monster->x + s;
                int y = monster->y + l;

                if (x >= 0 && x < width && y >= 0 && y < height) {
                    if (x == game->player.xpos && y == game->player.ypos) {
                        player = 1;
                        break;
                    }
                }
            }
            if (player) break;
        }

        if (!player) continue;

        position new_position = {monster->x, monster->y};

        if (game->player.xpos > monster->x && game->player.ypos > monster->y &&
            game->level[game->current_level].map[monster->y + 1][monster->x + 1] == '.') {
            new_position.x++;
            new_position.y++;
        }else if (game->player.xpos < monster->x && game->player.ypos > monster->y &&
                   game->level[game->current_level].map[monster->y + 1][monster->x - 1] == '.') {
            new_position.x--;
            new_position.y++;
        }else if (game->player.xpos > monster->x && game->player.ypos < monster->y &&
                   game->level[game->current_level].map[monster->y - 1][monster->x + 1] == '.') {
            new_position.x++;
            new_position.y--;
        }else if (game->player.xpos < monster->x && game->player.ypos < monster->y &&
                   game->level[game->current_level].map[monster->y - 1][monster->x - 1] == '.') {
            new_position.x--;
            new_position.y--;
        }else if ( game->player.ypos < monster->y &&
                   game->level[game->current_level].map[monster->y - 1][monster->x] == '.') {
            new_position.y--;
        }else if (game->player.ypos > monster->y &&
                   game->level[game->current_level].map[monster->y + 1][monster->x] == '.') {
            new_position.y++;
        }else if (game->player.xpos > monster->x  &&
                   game->level[game->current_level].map[monster->y][monster->x + 1] == '.') {
            new_position.x++;
        }else if (game->player.xpos < monster->x  &&
                   game->level[game->current_level].map[monster->y][monster->x - 1] == '.') {
            new_position.x--;
        }

        game->level[game->current_level].map[monster->y][monster->x] = '.';
        mvaddnstr(monster->y + start_row, monster->x + start_col, "∙", -1);

        game->level[game->current_level].map[new_position.y][new_position.x] = monster->name;
        mvaddch(new_position.y + start_row, new_position.x + start_col, monster->name);

        monster->x = new_position.x;
        monster->y = new_position.y;
    }

    refresh();
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void initialize_map(Game * game){
    for (int i = 0 ;i <height;i++){
        for (int j =0 ; j<width ;j++){
            game->level[game->current_level].map[i][j]=' ';
        }
    }
}
void generate_random_door(Room *room, int isSingleDoorRoom,int *tedad,int count,int max_room_2,Game *game) {
    srand(time(NULL));
    int count_door;
    if (count==max_room_2-1){
        if ((*tedad%2)==0){
            count_door=2;
        }else{
            count_door=3;
        }
    }else{
        if (isSingleDoorRoom){
            count_door=1;
        }else{
            int rand_door = rand() % 100;
            if (rand_door < 10) { 
                count_door = 4; 
            }else if (rand_door<80){
                count_door=2;
            }
            else if (rand_door<100){
                count_door=3;
            }
        }
    }
    *tedad+=count_door;
    for (int i = 0; i < count_door; i++) {
        room->tedad_door=0;
        int kenar_ham=0;
        int jahat;
        while(!kenar_ham){
            jahat = rand() % 4 + 1;
            switch (jahat) {
                case 1: 
                    room->door[i].xPosition = rand() % (room->tool - 2) + room->xPos + 1;
                    room->door[i].yPosition = room->yPos;
                    break;
                case 2: 
                    room->door[i].xPosition = rand() % (room->tool - 2) + room->xPos + 1;
                    room->door[i].yPosition = room->yPos + room->arrz - 1;
                    break;
                case 3: 
                    room->door[i].xPosition = room->xPos;
                    room->door[i].yPosition = rand() % (room->arrz - 2) + room->yPos + 1;
                    break;
                case 4: 
                    room->door[i].xPosition = room->xPos + room->tool - 1;
                    room->door[i].yPosition = rand() % (room->arrz - 2) + room->yPos + 1;
                    break;
            }
            kenar_ham=1;
            for (int j = 0; j < i; j++) {
                if (((room->door[i].xPosition - room->door[j].xPosition) <= 1) &&((room->door[i].xPosition - room->door[j].xPosition)>=-1)&&
                    ((room->door[i].yPosition - room->door[j].yPosition) <= 1)&&(room->door[i].yPosition - room->door[j].yPosition)>=-1){
                    kenar_ham = 0; 
                    break;
                }
            }   
        
        }
        game->level[game->current_level].map[room->door[i].yPosition][room->door[i].xPosition] = '+';
        game->level[game->current_level].rooms[count].door[i].xPosition=room->door[i].xPosition;
        game->level[game->current_level].rooms[count].door[i].yPosition=room->door[i].yPosition;

        (room->tedad_door)+=1;
    }
    game->level[game->current_level].rooms[count].tedad_door=count_door;
}
void draw_room(Room room,Game *game) {
    for (int i = 0; i < room.arrz; i++) {
        for (int j = 0; j < room.tool; j++) {
            if (i == 0 || i == room.arrz - 1) {
                game->level[game->current_level].map[room.yPos + i][room.xPos + j] =  'O';
            }else {
                if (j == 0 || j == room.tool - 1) {
                game->level[game->current_level].map[room.yPos + i][room.xPos + j] = '|';
                }else {
                game->level[game->current_level].map[room.yPos + i][room.xPos + j] = '.';
                }
            }
        }
    }
    
}
void generate_map(Game * game) {
    srand(time(NULL));
    Room *rooms = malloc(max_room * sizeof(Room));
    int max_room_2 = rand() % 3 + 6; 
    int count = 0;
    int hasSingleDoorRoom = 0; 
    int tedad = 0;
    game->level[game->current_level].num_rooms=max_room_2;
    while (count < max_room_2) {
        int tool = rand() % 6 + 6; 
        int arrz = rand() % 6 + 6;
        int x = rand() % (width - tool - 20) + 10; 
        int y = rand() % (height - arrz - 20) + 10; 
        int tadakhol = 0;

        for (int i = 0; i < count; i++) {
            if (x-5 < rooms[i].xPos + rooms[i].tool && x + tool+5 > rooms[i].xPos &&
                y-5 < rooms[i].yPos + rooms[i].arrz && y + arrz +5> rooms[i].yPos) {
                tadakhol = 1;
                break;
            }
        }

        if (!tadakhol) {
            rooms[count].arrz = arrz;
            game->level[game->current_level].rooms[count].arrz=arrz;
            rooms[count].tool = tool;
            game->level[game->current_level].rooms[count].tool=tool;
            rooms[count].xPos = x;
            game->level[game->current_level].rooms[count].xPos=x;
            rooms[count].yPos = y;
            game->level[game->current_level].rooms[count].yPos=y;
            draw_room(rooms[count],game);

            if (!hasSingleDoorRoom) {
                generate_random_door(&rooms[count], 1, &tedad, count, max_room_2,game ); 
                hasSingleDoorRoom = 1;
            }else {
                generate_random_door(&rooms[count], 0, &tedad, count, max_room_2,game); 
            }

            count++;
        }
        game->level[game->current_level].rooms[count]=rooms[count];
    }
    if (game->current_level==0){
        playerset_up_first(&rooms, max_room_2,game);
    }
    
    
    free(rooms);
}
int xdelta(int x,int y,int x1 ,int y1 ,int x2,int y2, Game * game){
    if ((game->level[game->current_level].map[y][x+1]==' '||game->level[game->current_level].map[y][x+1]=='#')&&(x<x2)){
        x++;
    }
    if ((game->level[game->current_level].map[y][x-1]==' '||game->level[game->current_level].map[y][x-1]=='#')&&(x>x2)){
        x--;
    }
    return x;
}
int ydelta(int x,int y,int x1 ,int y1 ,int x2,int y2, Game * game){
    if ((game->level[game->current_level].map[y+1][x]==' '||game->level[game->current_level].map[y+1][x]=='#')&&(y<y2)){
        y++;
    }
    if ((game->level[game->current_level].map[y-1][x]==' '||game->level[game->current_level].map[y-1][x]=='#')&&(y>y2)){
        y--;
    }
    return y;

}
void initialize(int x1 , int y1 ,int x2 , int y2,Game *game){
    int x=x1;
    int y=y1;

    while(x!=x2||y!=y2){
        if (x==xdelta(x,y,x1,y1,x2,y2,game)){
            y=ydelta(x,y,x1,y1,x2,y2,game);
        }
        if (y==ydelta(x,y,x1,y1,x2,y2,game)){
            x=xdelta(x,y,x1,y1,x2,y2,game);
        }

        
        game->level[game->current_level].map[y][x]='#';
        mvaddch(y,x,'#');
    }
    refresh();

}
void display_map(Game *game) {
    clear();
    start_color();
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(4,COLOR_WHITE,COLOR_BLACK);
    init_pair(5,COLOR_CYAN,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
    char *black_circle = "●";
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (game->level[game->current_level].map[i][j]=='^'){
                // attron(COLOR_PAIR(1));
                mvaddnstr(start_row + i, start_col + j, "∙",-1);
                // attroff(COLOR_PAIR(1));
            }else if(game->level[game->current_level].map[i][j]=='$'){
                attron(COLOR_PAIR(1));
                mvaddch(start_row + i, start_col + j, '$');
                attroff(COLOR_PAIR(1));
            }else if(game->level[game->current_level].map[i][j]=='.'){
                mvaddnstr(start_row + i, start_col + j,"∙",-1);
            }else if(game->level[game->current_level].map[i][j]=='@'){
                attron(COLOR_PAIR(4));
                mvaddch(start_row + i, start_col + j,'@');
                attroff(COLOR_PAIR(4));
            }else if(game->level[game->current_level].map[i][j]=='G'){
                attron(COLOR_PAIR(6));
                mvaddnstr(j + start_row, i + start_col, "G",-1);
                attroff(COLOR_PAIR(6));
            }else if(game->level[game->current_level].map[i][j]=='B'){
                attron(COLOR_PAIR(6));
                mvaddnstr(j + start_row, i + start_col, "B",-1);
                attroff(COLOR_PAIR(6));
            }else if(game->level[game->current_level].map[i][j]=='/'){
                mvaddstr(j + start_row, i + start_col,"/" );
            }else if(game->level[game->current_level].map[i][j]=='*'){
                attron(COLOR_PAIR(3));
                mvaddnstr(j + start_row, i + start_col, "*",-1);
                attroff(COLOR_PAIR(3));
            }else if(game->level[game->current_level].map[i][j]=='~'){

                mvaddnstr(j + start_row, i + start_col, "~",-1);

            }else if(game->level[game->current_level].map[i][j]=='>'){

                mvaddnstr(j + start_row, i + start_col, ">",-1);

            }else if(game->level[game->current_level].map[i][j]=='='){

                mvaddnstr(j + start_row, i + start_col, "=",-1);

            }else if(game->level[game->current_level].map[i][j]=='|'){
                attron(COLOR_PAIR(2)| A_BOLD);
                mvaddch(start_row + i, start_col + j,'|');
                attroff(COLOR_PAIR(2)|A_BOLD);
            }else if (game->level[game->current_level].map[i][j]=='O'){
                attron(COLOR_PAIR(2)| A_BOLD);
                mvaddch(start_row + i, start_col + j,'O');
                attroff(COLOR_PAIR(2)|A_BOLD);

            }else if (game->level[game->current_level].map[i][j]=='+'){
                attron(COLOR_PAIR(2)| A_BOLD);
                mvaddch(start_row + i, start_col + j,'+');
                attroff(COLOR_PAIR(2)|A_BOLD);

            }else if (game->level[game->current_level].map[i][j]=='1'){
            
                mvaddnstr(start_row + i, start_col + j,"♥",-1);
                

            }else if (game->level[game->current_level].map[i][j]=='2'){
                attron(COLOR_PAIR(6));
                mvaddnstr(start_row + i, start_col + j,"♥",-1);
                attroff(COLOR_PAIR(6));

            }else if (game->level[game->current_level].map[i][j]=='3'){
                attron(COLOR_PAIR(2));
                mvaddnstr(start_row + i, start_col + j,"♥",-1);
                attroff(COLOR_PAIR(2));

            }else if (game->level[game->current_level].map[i][j]=='4'){
                attron(COLOR_PAIR(6));
                mvaddnstr(start_row + i, start_col + j,"♥",-1);
                attroff(COLOR_PAIR(6));

            }else if (game->level[game->current_level].map[i][j]=='5'){
                mvaddnstr(start_row + i, start_col + j,"♦",-1);

            }else if (game->level[game->current_level].map[i][j]=='6'){
                attron(COLOR_PAIR(6));
                mvaddnstr(start_row + i, start_col + j,"♦",-1);
                attroff(COLOR_PAIR(6));

            }else if (game->level[game->current_level].map[i][j]=='7'){
                attron(COLOR_PAIR(2));
                mvaddnstr(start_row + i, start_col + j,"♦",-1);
                attroff(COLOR_PAIR(2));

            }
            else {
                mvaddch(start_row + i, start_col + j, game->level[game->current_level].map[i][j]);}
        }
    }
    refresh();
}
void desplay_lab(Game *game){
    int start_row = (LINES - height) / 2;
    int start_col = (COLS - width) / 2;
    if (game->player.health>100){
        game->player.health=100;
    }
    float health = (float)game->player.health / 100;
    int bar_length = 20;
    mvprintw(start_row+7,start_col+3,"Health: [");
    int filled = (int)(health * bar_length);
    int l = start_col+12;
    for (int i = 0; i < filled; i++) {
        mvprintw(start_row+7,i+l,"█");
        
    }
    l+=filled;
    for (int i = 0; i < bar_length - filled; i++) {
        mvprintw(start_row+7,i+l,"-");
    }
    mvprintw(start_row+7, start_col+32,"] %d/%d\n", game->player.health, 100);

    WINDOW *win=newwin(3,8,start_row+height-8,start_col+7);
    mvwprintw(win,1,1,"HP: %d",game->player.health);
    WINDOW *win1=newwin(3,10,start_row+height-8,start_col+24);
    mvwprintw(win1,1,1,"LEVEL: %d",game->current_level+1);
    WINDOW *win2=newwin(3,10,start_row+height-8,start_col+43);
    mvwprintw(win2,1,1,"Gold: %d",game->player.gold);
    WINDOW *win3=newwin(3,16,start_row+height-8,start_col+60);
    mvwprintw(win3,1,1,"Score: %d",((game->player.gold*15)/2)*(game->current_level+1));
    game->score=((game->player.gold*15)/2);
    wrefresh(win);
    wrefresh(win1);
    wrefresh(win2);
    wrefresh(win3);
    refresh();
    // WINDOW *win=newwin(3,9,start_row+height-8,start_col+7);
    // box(win,0,0);
    // mvwprintw(win,1,1,"HP : %d",game->player.health);
    // WINDOW *win=newwin(3,9,start_row+height-8,start_col+7);
    // box(win,0,0);
    // mvwprintw(win,1,1,"HP : %d",game->player.health);
    // WINDOW *win=newwin(3,9,start_row+height-8,start_col+7);
    // box(win,0,0);
    // mvwprintw(win,1,1,"HP : %d",game->player.health);
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void play_music(int choice) {
    const char *music_file = NULL;
    
    switch (choice) {
        case 3:
            music_file = "Asrae.mp3";  
            break;
        case 4:
            music_file = "1_17 - Winter Has Come - Ramin Djawadi (320).mp3"; 
            break;
        case 5:
            music_file = "Jesper Kyd - Ezios Family - musicgeek.ir.mp3";
            break;
        case 6:
            music_file = "Belaad Party.mp3"; 
            break;
        default:
            return;
    }

    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music *music = Mix_LoadMUS(music_file);
    
    Mix_PlayMusic(music, 1); 
    
    getchar();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    return;
}
void setting(){
    clear();
    int chooce = 1;
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    noecho();
    char *black_circle = "●";
    int start_row = (LINES - 10) / 2;
    int start_col = (COLS - 30) / 2;
    WINDOW *win = newwin(10, 30, start_row, start_col);
    box(win, 0, 0);
    curs_set(0);
    
    do {
        attron(A_BOLD|A_ITALIC);
        mvprintw(5, (COLS - strlen("==== S E T T I N G ===")) / 2, "==== S E T T I N G ===");
        

        mvprintw(12, (COLS - strlen("........Difficulty........")) / 2 - 12, "........Difficulty.......");
        mvprintw(13, (COLS - strlen("easy")) / 2 - 14, "%s easy", chooce == 1 ? black_circle : " ");
        mvprintw(14, (COLS - strlen("normal")) / 2 - 14, "%s normal", chooce == 2 ? black_circle : " ");
        mvprintw(15, (COLS - strlen("hard")) / 2 -14, "%s hard", chooce == 3 ? black_circle : " ");
        mvprintw(12, (COLS - strlen(".......Music......")) / 2 + 12, "........Music.......");
        mvprintw(13, (COLS - strlen("Asrea( remix)")) / 2 + 12, "%s Asrea( remix)", chooce == 4 ? black_circle : " ");
        mvprintw(14, (COLS - strlen("Game of Thrones")) / 2 + 12, "%s Game of Thrones", chooce == 5 ? black_circle : " ");
        mvprintw(15, (COLS - strlen("Ezio's Family")) / 2 + 12, "%s Ezio's Family", chooce == 6 ? black_circle : " ");
        mvprintw(16, (COLS - strlen("Hiphopologist (belaad party)")) / 2 + 12, "%s Hiphopologist (belaad party)", chooce == 7 ? black_circle : " ");
        attroff(A_BOLD|A_ITALIC);
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                chooce = (chooce == 1) ? 1 : chooce - 1;
                break;
            case KEY_DOWN:
                chooce = (chooce == 7) ? 7 : chooce + 1; 
                break;
            case 10: 

                if (chooce > 3) {
                    play_music(chooce-1); 
                }else{
                    difficulty=chooce;
                }
                break;
                return;
            case 27:
                return;
        }
    } while (chooce != 0);
    
}
void scoreboard(const char *filename, char username[50]) {
    clear();
    int start_row = (LINES - 10) / 2;
    int start_col = (COLS - 30) / 2;
    char filename2[200];
    FILE *fptr = fopen("user.txt", "r");

    Game game1;
    NewPlayer *newplayer = (NewPlayer *)malloc(20 * sizeof(NewPlayer));

    char line[100], username_file[100];
    int count = 0;
    
    while (fgets(line, sizeof(line), fptr) && count < 20) {
        sscanf(line, "%s", username_file);
        sprintf(filename2, "%s.txt", username_file);
        FILE *fptr2 = fopen(filename2, "rb");
        if (!fptr2) 
            continue;
        fread(&game1.score, sizeof(int), 1, fptr2);
        fread(&game1.player, sizeof(Player), 1, fptr2);
        fclose(fptr2);
        if (game1.score>10000){
            continue;
        }
        newplayer[count].score = game1.score;
        newplayer[count].gold = (game1.score*2/15);
        strcpy(newplayer[count].name, username_file);
        count++;
    }
    fclose(fptr);
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (newplayer[j].score < newplayer[j + 1].score) {
                NewPlayer tmp = newplayer[j];
                newplayer[j] = newplayer[j + 1];
                newplayer[j + 1] = tmp;
            }
        }
    }

    clear();
    mvprintw(start_row - 2, start_col, "SCOREBOARD");
    mvprintw(start_row, start_col, "RANK   NAME            SCORE   GOLD");

    for (int i = 0; i < count; i++) {
        if (strcmp(newplayer[i].name, username) == 0) {
            attron(A_BOLD );
        }

        mvprintw(start_row + i + 1, start_col, "%-6d %-15s %-7d %-5d", i + 1, newplayer[i].name, newplayer[i].score, newplayer[i].gold);
        
        if (strcmp(newplayer[i].name, username) == 0) {
            attroff(A_BOLD | A_UNDERLINE);
        } else if (i == 0) {
            attroff(A_BOLD);
        }
        if (i == 0) {
            mvprintw(start_row + i + 1, start_col + 40, "🥇");
        } else if (i == 1) {
            mvprintw(start_row + i + 1, start_col + 40, "🥈");
        } else if (i == 2) {
            mvprintw(start_row + i + 1, start_col + 40, "🥉");
        }
    }
    getch();
    refresh();

    free(newplayer);
    return;
}
void load_game_menu(const char *filename, int i,char username[30]) {
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    char *black_circle = "●";
    int chooce = 0;
    clear();
    noecho();
    int start_row = (LINES - 10) / 2;
    int start_col = (COLS - 30) / 2;
    WINDOW *win = newwin(10, 30, start_row, start_col);
    box(win, 0, 0);
    curs_set(0);

    do {
        clear();
        attron(A_BOLD);
        mvprintw(5, (COLS - strlen("=== Load Game Menu ===")) / 2, "=== Load Game Menu ===");
        attroff(A_BOLD);

        mvprintw(7, (COLS - strlen(" Start New Game")) / 2, "%s Start New Game", chooce == 0 ? black_circle : " ");
        if (i == 0) mvprintw(8, (COLS - strlen(" Load Saved Game")) / 2, "%s Load Saved Game", chooce == 1 ? black_circle : " ");
        mvprintw(9-i, (COLS - strlen(" Scoreboard")) / 2, "%s Scoreboard", chooce == (i == 0 ? 2 : 1) ? black_circle : " ");
        mvprintw(10-i, (COLS - strlen(" Setting")) / 2, "%s Setting", chooce == (i == 0 ? 3 : 2) ? black_circle : " ");
        mvprintw(11-i, (COLS - strlen(" Exit")) / 2, "%s Exit", chooce == (i == 0 ? 4 : 3) ? black_circle : " ");

        refresh();
        int choice = getch();

        switch (choice) {
            case KEY_UP:
                chooce = (chooce - 1 + (i == 0 ? 5 : 4)) % (i == 0 ? 5 : 4);
                break;
            case KEY_DOWN:
                chooce = (chooce + 1) % (i == 0 ? 5 : 4);
                break;
            case 10:
                if (chooce == 0) {
                    game(0, filename);
                } else if (i == 0 && chooce == 1) {
                    Game loadedGame;
                    loadGame(&loadedGame, filename);
                    gamelooping(&loadedGame, 1, 0, filename);

                }else if(chooce==2&&i==0){
                    scoreboard(filename,username);

                }else if(chooce==1&&i==1){
                    scoreboard(filename,username);

                }else if(chooce==2&&i==1){
                    setting();

                }else if(chooce==3&&i==0){
                    setting();

                }
                else if (chooce == (i == 0 ? 4 : 3)) {
                    clear();
                    mvprintw(LINES / 2, (COLS - strlen("Exiting...")) / 2, "Exiting...");
                    refresh();
                    usleep(1000000);
                    
                    show_menu();
                }
                break;
        }
    } while (1);
}
void show_menu() {
    clear();
    curs_set(0);
    int choice = 1;
    int key;
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    int start_y = (LINES - height) / 2;
    int start_x = (COLS - width) / 2;
    WINDOW *menu_win = newwin(height, width, start_y, start_x);
    keypad(menu_win, TRUE);
    box(menu_win, 0, 0);
    wbkgd(menu_win, COLOR_PAIR(1));
    mvwprintw(menu_win, 1, (width - strlen(" Main Menu ")) / 2, " Main Menu ");
    mvwhline(menu_win, 2, 1, ACS_HLINE, width - 2);
    const char *options[] = {
        "Create new user",
        "Login",
        "Guest login",
        "Exit"
    };
    
    do {
        for (int i = 0; i < 4; i++) {
            if (choice == i + 1) {
                wattron(menu_win, COLOR_PAIR(2) | A_BOLD | A_REVERSE);
            } else {
                wattron(menu_win, COLOR_PAIR(3) | A_BOLD);
            }
            mvwprintw(menu_win, i + 4, (width - strlen(options[i])) / 2, "%s", options[i]);
            wattroff(menu_win, COLOR_PAIR(2) | A_BOLD | A_REVERSE);
            wattroff(menu_win, COLOR_PAIR(3) | A_BOLD);
        }
        wrefresh(menu_win);
        
        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
                choice = (choice == 1) ? 4 : choice - 1;
                break;
            case KEY_DOWN:
                choice = (choice == 4) ? 1 : choice + 1;
                break;
            case 10:
                delwin(menu_win);
                endwin();
                switch (choice) {
                    case 1:
                        createuser();
                        show_menu();
                        break;
                    case 2:
                        if (login()) {
                            clear();
                        }
                        show_menu();
                        break;
                    case 3:
                        guestlogin();
                        show_menu();
                        clear();
                        break;
                    case 4:
                        break ;
                }
                
                return;
        }
    }while(1);
}