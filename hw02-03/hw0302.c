#include "all.h"

typedef struct _teamInfo{
    char teamName[200];
    int points;
    int score;
    int red_cards;
    int win_home;
    int win_away;
    int win;
    int draw;
    int lose;
    int goal_score;
    int goal_against;
} teamInfo;

teamInfo teams[20] = {{"", 0, 0, 0, 0, 0, 0, 0, 0, 0}};

typedef struct _contest{
    char date[200];
    char homeTeam[200];
    int homeScore;
    char awayTeam[200];
    int awayScore;
}  contest;

contest max_contest = {0, 0, 0, 0, 0};

void parsing(FILE *parseFile) {
    char line[1000];
    fgets(line, 1000, parseFile);
    while(!feof(parseFile)) {
        fgets(line, 1000, parseFile);
        //Compare
        contest ctmp = {0, 0};
        //Date
        char *token = strtok(line , ",");
        strcpy(ctmp.date, token);
        //HomeTeam
        token = strtok(NULL, ",");
        if(token == NULL) {
            break;
        }
        int i = 0;
        teamInfo *homeTeam = NULL, *awayTeam = NULL;
        for(i = 0; i < 20; ++i) {
            if(strcmp(teams[i].teamName, token) == 0) {
                homeTeam = &teams[i];
            }else if(teams[i].teamName[0] == '\0') {
                break;
            }
        }
        if(homeTeam == NULL) {
            homeTeam = &teams[i];
            strcpy(homeTeam->teamName, token);
        }
        strcpy(ctmp.homeTeam, homeTeam->teamName);
        //AwayTeam
        token = strtok(NULL, ",");
        for(i = 0; i < 20; ++i) {
            if(strcmp(teams[i].teamName, token) == 0) {
                awayTeam = &teams[i];
            }else if(teams[i].teamName[0] == '\0') {
                break;
            }
        }
        if(awayTeam == NULL) {
            awayTeam = &teams[i];
            strcpy(awayTeam->teamName, token);
        }
        strcpy(ctmp.awayTeam, awayTeam->teamName);
        //FTHG
        token = strtok(NULL, ",");
        int tmp = 0;
        tmp = strtol(token, NULL, 10);
        homeTeam->score += tmp;
        homeTeam->goal_score += tmp;
        awayTeam->goal_against += tmp;
        ctmp.homeScore = tmp;
        //FTAG
        token = strtok(NULL, ",");
        tmp = strtol(token, NULL, 10);
        awayTeam->score += tmp;
        homeTeam->goal_against += tmp;
        awayTeam->goal_score += tmp;
        ctmp.awayScore = tmp;
        //FTR
        token = strtok(NULL, ",");
        if(token[0] == 'H'){
            homeTeam->points += 3;
            homeTeam->win_home += 1;
            homeTeam->win += 1;
            awayTeam->lose += 1;
        }else if(token[0] == 'D') {
            homeTeam->points += 1;
            awayTeam->points += 1;
            homeTeam->draw += 1;
            awayTeam->draw += 1;
        }else if (token[0] == 'A') {
            awayTeam->points += 3;
            awayTeam->win_away += 1;
            awayTeam->win += 1;
            homeTeam->lose += 1;
        }
        //HTHG
        token = strtok(NULL, ",");
        //HTAG
        token = strtok(NULL, ",");
        //HTR
        token = strtok(NULL, ",");
        //Referee
        token = strtok(NULL, ",");
        //HS
        token = strtok(NULL, ",");
        //AS
        token = strtok(NULL, ",");
        //HTS
        token = strtok(NULL, ",");
        //ATS
        token = strtok(NULL, ",");
        //HF
        token = strtok(NULL, ",");
        //AF
        token = strtok(NULL, ",");
        //HC
        token = strtok(NULL, ",");
        //AC
        token = strtok(NULL, ",");
        //HY
        token = strtok(NULL, ",");
        //AY
        token = strtok(NULL, ",");
        //HR
        token = strtok(NULL, ",");
        homeTeam->red_cards += strtol(token, NULL, 10);
        //AR
        token = strtok(NULL, ",");
        awayTeam->red_cards += strtol(token, NULL, 10);
        if(abs(max_contest.awayScore - max_contest.homeScore) < abs(ctmp.awayScore - ctmp.homeScore)) max_contest = ctmp; 
    }
    
}




int main(){
    char fileName [100] = {0}, teamName[200] = {0};
    printf("Please open a season record:");
    scanf(" %s", fileName);
    FILE *readFile = fopen(fileName, "r");
    int mode = 0, breakFlag = 0;
    parsing(readFile);

    if(readFile == NULL) {
        printf("Isn't exist.\n");
        return 0;
    }

    printf("1) Who is the winner in this season?\n");
    printf("2) Which team gets the most scores?\n");
    printf("3) Which team gets the most red cards?\n");
    printf("4) Which team wins the most games at home?\n");
    printf("5) Which team wins the most games away from home?\n");
    printf("6) Which game has the most scoring gap?\n");
    printf("7) Team information.\n");
    printf("8) Exit\n");

    while(!breakFlag) {
        printf("Choice (1-7, 8:exit):");
        scanf(" %d", &mode);
        int max = 0, max_index = 0;
        switch (mode){
            case 1:
                for(int i = 0; i < 20; ++i) {
                    if(max < teams[i].points){
                        max = teams[i].points;
                        max_index = i;
                    }
                }
                printf("The winner is %s\n", teams[max_index].teamName);
                break;
            case 2:
                for(int i = 0; i < 20; ++i) {
                    if(max < teams[i].score){
                        max = teams[i].score;
                    }
                }
                for(int i = 0; i < 20; ++i) {
                    if(max == teams[i].score) {
                        printf("%s ", teams[i].teamName);
                    }
                }
                printf("%d\n", max);
                break;
            case 3:
                for(int i = 0; i < 20; ++i) {
                    if(max < teams[i].red_cards){
                        max = teams[i].red_cards;
                        max_index = i;
                    }
                }
                for(int i = 0; i < 20; ++i) {
                    if(max == teams[i].red_cards) {
                        printf("%s ", teams[i].teamName);
                    }
                }
                printf("%d\n", max);
                break;
            case 4:
                for(int i = 0; i < 20; ++i) {
                    if(max < teams[i].win_home){
                        max = teams[i].win_home;
                        max_index = i;
                    }
                }
                for(int i = 0; i < 20; ++i) {
                    if(max == teams[i].win_home) {
                        printf("%s ", teams[i].teamName);
                    }
                }
                printf("%d\n", max);
                break;
            case 5:
                for(int i = 0; i < 20; ++i) {
                    if(max < teams[i].win_away){
                        max = teams[i].win_away;
                        max_index = i;
                    }
                }
                for(int i = 0; i < 20; ++i) {
                    if(max == teams[i].win_away) {
                        printf("%s ", teams[i].teamName);
                    }
                }
                printf("%d\n", max);
                break;
            case 6:
                printf("%s, %s(%d) vs %s(%d)\n", max_contest.date, max_contest.homeTeam, max_contest.homeScore, max_contest.awayTeam, max_contest.awayScore);
                break;
            case 7:
                printf("Team: ");
                scanf(" %s", teamName);
                teamInfo *target = NULL;
                for(int i = 0; i < 20; ++i) {
                    if(strcmp(teamName, teams[i].teamName) == 0) {
                        target = &teams[i];
                    }
                }
                if(target == NULL) {
                    printf("Team not found.\n");
                    continue;
                }
                printf("Points: %d\nWin/Draw/Lose: %d/%d/%d\nGoals Scored/Goals Against: %d/%d\n", target->points, target->win, target->draw, target->lose, target->goal_score, target->goal_against);
                break;                                    
            case 8:
                breakFlag = 1;
                break;
        }
    }

    fclose(readFile);
}