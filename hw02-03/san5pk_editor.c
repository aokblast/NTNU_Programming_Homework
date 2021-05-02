#include "all.h"
#include "san5pk_editor.h"
#include "bitMani.h"

uint64_t getFileSize(fstream File) {
    fseek(File, 0, SEEK_END);
    uint64_t size = ftell(File);
    fseek(File, 0, SEEK_SET);
    return size;
}

void edit_monarch(fstream readFile) {
    monarInfo target[1], find[1];
    bool isIn = false;
    uint64_t fileSize = getFileSize(readFile);
    int64_t tmp = 0;
    //Enter query
    enter_target:
    printf("Please input the monarch data you want to search (-1 for re-enter, -2 for return menu):\n");
    printf("Reputation(Must be original value):");
    scanf(" %ld", &tmp); 
    if(tmp == -1) goto enter_target;
    else if (tmp == -2) return;
    target->heart = tmp;
    printf("Gold:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    else if(tmp == -2) return;
    target->gold = tmp;
    printf("Food:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    else if(tmp == -2) return;
    target->food = tmp;
    //Find require
    while(fileSize - ftell(readFile) >= sizeof(monarInfo)) {
        fread(find, sizeof(monarInfo), 1, readFile);
        find->food &= THREEBYTE;
        find->gold &= THREEBYTE;
        if(((find->heart) == (target->heart)) && ((find->food) == (target->food)) && ((find->gold) == (target->gold))) {
            isIn = true;
            break;
        }
        fseek(readFile,-(sizeof(monarInfo)) + 1 ,SEEK_CUR);
    }
    //Change Value
    if(isIn) {
        printf("Enter the data you want to change(-1 for not change):\n");
        printf("Reputation(0 <= enter_num <= 1000):");
        tmp = 0;
        scanf(" %ld", &tmp);
        while(tmp > 1000 || tmp < -1) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->heart = tmp;
        }
        printf("Gold(0 <= enter_num <= 3000000):");
        scanf(" %ld", &tmp);
        while(tmp > 3000000 || tmp < -1) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->gold = tmp;
        }
        printf("Food(0 <= enter_num <= 3000000):");
        scanf(" %ld", &tmp);
        while(tmp > 3000000 || tmp < -1) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->food = tmp;
        }
        fseek(readFile, -sizeof(monarInfo), SEEK_CUR);
        fwrite(find, sizeof(monarInfo), 1, readFile);
        fseek(readFile, 0, SEEK_SET);
    }else {
        printf("Monarch not found, Please re-enter.\n");
        fseek(readFile, 0, SEEK_SET);
        goto enter_target;
    }

}
void edit_city(fstream readFile) {
    cityinfo target[1], find[1], test[1];
    int count =  0;
    int64_t tmp = 0;
    uint64_t fileSize = getFileSize(readFile);
    uint64_t offset = 0;

    //Enter query(Population, Soldier, Defeat)
    enter_target:
    printf("Please input the under three City data you want to search (-1 for re-enter, -2 for return to menu):\n");
    printf("Population:");
    scanf(" %ld", &tmp); 
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->population = tmp / 100;
    printf("Preparatory-Soidier:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->prepare_soldier = tmp;
    printf("Defend:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->defeat = tmp;

    //Find require
    while(fileSize - ftell(readFile) >= sizeof(cityinfo)) {
        fread(test, sizeof(cityinfo), 1, readFile);
        test->prepare_soldier &= THREEBYTE;
        if(((test->prepare_soldier) == (target->prepare_soldier)) && ((test->population) == (target->population)) && ((test->defeat) == (target->defeat))) {
            ++count;
            find[0] = test[0];
            offset = ftell(readFile);
        }
        fseek(readFile,-(sizeof(cityinfo)) + 1 ,SEEK_CUR);
    }

    if(count == 0) {
        printf("Find no City. Please re-enter.\n");
        fseek(readFile, 0, SEEK_SET);
        goto enter_target;
    }else if (count != 1) {
        //Enter query(Development, Business, Water)
        enter_target_2:
        printf("Find %d City, Please give more information(-1 for re-enter, -2 for return to menu):\n", count);
        count = 0;
        fseek(readFile, 0, SEEK_SET);
        printf("Development:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->development = tmp;
        printf("Business:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->business = tmp;
        printf("Water:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->water = tmp;
        //Query
        while(fileSize - ftell(readFile) >= sizeof(cityinfo)) {
            fread(test, sizeof(cityinfo), 1, readFile);
            test->prepare_soldier &= THREEBYTE;
            if(((test->prepare_soldier) == (target->prepare_soldier)) && ((test->population) == (target->population)) && ((test->defeat) == (target->defeat)) && ((test->development) == (target->development)) && ((test->water) == (target->water)) && ((test->business) == (target->business))) {
                ++count;
                find[0] = test[0];
                offset = ftell(readFile);
            }
            fseek(readFile,-(sizeof(cityinfo)) + 1 ,SEEK_CUR);
        }
    }
    if(count == 0) {
        printf("Find no City. Please re-enter.\n");
        fseek(readFile, 0, SEEK_SET);
        goto enter_target_2;
    }else if (count != 1) {
        //Enter query(Loyalty, Train, Moral)
        enter_target_3:
        printf("Find %d City, Please give more information(-1 for re-enter, -2 for return to menu):\n", count);
        count = 0;
        fseek(readFile, 0, SEEK_SET);
        printf("Loyalty:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->loyalty = tmp;
        printf("Train:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->train = tmp;
        printf("Moral:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->moral = tmp;
        //Query
        while(fileSize - ftell(readFile) >= sizeof(cityinfo)) {
            fread(test, sizeof(cityinfo), 1, readFile);
            test->prepare_soldier &= THREEBYTE;
            if(((test->prepare_soldier) == (target->prepare_soldier)) && ((test->population) == (target->population)) && ((test->defeat) == (target->defeat)) && ((test->development) == (target->development)) && ((test->water) == (target->water)) && ((test->business) == (target->business)) && ((test->moral) == (target->moral)) && ((test->train) == (target->train)) && ((test->loyalty) == (target->loyalty))) {
                ++count;
                find[0] = test[0];
                offset = ftell(readFile);
            }
            fseek(readFile,-(sizeof(cityinfo)) + 1 ,SEEK_CUR);
        }
    }

    //Change Value
    if(count == 1) {
        printf("Enter the data you want to change(-1 for not change):\n");
        printf("Prepared Soldier(0 <= enter_num <= 5000000) If it is 0, you can't change train:");
        scanf(" %ld", &tmp);
        while(tmp > 5000000  || tmp < -1) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->prepare_soldier = tmp;
        }
        printf("Population(50000 <= enter_num <= 3000000):");
        scanf(" %ld", &tmp);
        while((tmp > 3000000 || tmp < 50000) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            tmp /= 100;
            find->population = tmp;
        }
        printf("Defend(0 <= enter_num <= 999):");
        scanf(" %ld", &tmp);
        while((tmp > 999 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->defeat = tmp;
        }
        printf("Development(0 <= enter_num <= 999) Maximum may different from city:");
        scanf(" %ld", &tmp);
        while((tmp > 999 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->development = tmp;
        }
        printf("Business(0 <= enter_num <= 999) Maximum may different from city:");
        scanf(" %ld", &tmp);
        while((tmp > 999 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->business = tmp;
        }
        printf("Water(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->water = tmp;
        }
        printf("Loyalty(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->loyalty = tmp;
        }
        if(find->prepare_soldier != 0){
            printf("Train(0 <= enter_num <= 100):");
            scanf(" %ld", &tmp);
            while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
                printf("Invalid number, Please re-input:");
                scanf(" %ld", &tmp);
            }
            if(tmp != -1) {
                find->train = tmp;
            }
        }
        printf("Moral(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->moral = tmp;
        }
        fseek(readFile, offset -sizeof(cityinfo), SEEK_SET);
        fwrite(find, sizeof(cityinfo), 1, readFile);
        fseek(readFile, 0, SEEK_SET);
        return;
    }
    
}

void printSkillStat(uint32_t skill) {
    printf("PresentStatus(1 for on, 0 for off):\n");
    printf(" 1. FireStrategy:    %u ", skill & 1);
    printf(" 2. Confusion:       %u ", (skill >> 1) & 1);
    printf(" 3. Coax:            %u ", (skill >> 2) & 1);
    printf(" 4. Pack:            %u ", (skill >> 3) & 1);
    printf("\n");
    printf(" 5. Magic:           %u ", (skill >> 4) & 1);
    printf(" 6. Cheer:           %u ", (skill >> 5) & 1);
    printf(" 7. Cure:            %u ", (skill >> 6) & 1);
    printf(" 8. PrayRain:        %u ", (skill >> 7) & 1);
    printf("\n");
    printf(" 9. SkyChange:       %u ", (skill >> 8) & 1);
    printf("10. Extinguish:      %u ", (skill >> 9) & 1);
    printf("11. FallStone:       %u ", (skill >> 10) & 1);
    printf("12. Divination:      %u ", (skill >> 11) & 1);
    printf("\n");
    printf("13. Ambush:          %u ", (skill >> 12) & 1);
    printf("14. Illusion:        %u ", (skill >> 13) & 1);
    printf("15. WitchCraft:      %u ", (skill >> 14) & 1);
    printf("16. Excitation:      %u ", (skill >> 15) & 1);
    printf("\n");
    printf("17. WaterSkill:      %u ", (skill >> 16) & 1);
    printf("18. Flame:           %u ", (skill >> 17) & 1);
    printf("19. Rouse:           %u ", (skill >> 18) & 1);
    printf("20. SnapShot:        %u ", (skill >> 19) & 1);
    printf("\n");
    printf("21. FastAttack:      %u ", (skill >> 20) & 1);
    printf("22. Rattan:          %u ", (skill >> 21) & 1);
    printf("23. Wushuang:        %u ", (skill >> 22) & 1);
    printf("24. FarArrow:        %u ", (skill >> 23) & 1);
    printf("\n");
    printf("25. Compulsory:      %u ", (skill >> 24) & 1);
    printf("26. Calm:            %u ", (skill >> 25) & 1);
    printf("27. LayOut:          %u ", (skill >> 26) & 1);
    printf("28. WaterGod:        %u ", (skill >> 27) & 1);
    printf("\n");
    printf("29. AntiTreacherous: %u ", (skill >> 28) & 1);
    printf("30. FallSand:        %u ", (skill >> 29) & 1);
    printf("31. BackShot:        %u ", (skill >> 30) & 1);
    printf("32. RideShot:        %u ", (skill >> 31) & 1);
    printf("\n");
}

uint32_t setSkill(uint32_t skill) {
    
    while(1) {
        int test = 0;
        printSkillStat(skill);
        printf("Please enter a index to change the status(1-32 for change, -1 for end):");
        scanf(" %d", &test);
        while((test > 32 || test < 0) ^ (test ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %d", &test);
        }
        if(test == -1) break;
        else skill ^= (1 << (test - 1));
    }
    return skill;

}

void edit_commander(fstream readFile) {
    heroinfo target[1], find[1], test[1];
    int count =  0;
    int64_t tmp = 0;
    uint64_t fileSize = getFileSize(readFile);
    uint64_t offset = 0;

    //Enter query(Reputation, Soldier, Expirence)
    enter_target:
    printf("Please input the under three hero data you want to search (-1 for re-enter, -2 for return to menu):\n");
    printf("Reputation(YongMing):");
    scanf(" %ld", &tmp); 
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->reputation = tmp;
    printf("Expirence:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->exp = tmp;
    printf("Soldier:");
    scanf(" %ld", &tmp);
    if(tmp == -1) goto enter_target;
    if(tmp == -2) return;
    target->soldier = tmp;

    //Find require
    while(fileSize - ftell(readFile) >= sizeof(heroinfo)) {
        fread(test, sizeof(heroinfo), 1, readFile);
        if(((test->reputation) == (target->reputation)) && ((test->exp) == (target->exp)) && ((test->soldier) == (target->soldier))) {
            ++count;
            find[0] = test[0];
            offset = ftell(readFile);
        }
        fseek(readFile,-(sizeof(heroinfo)) + 1 ,SEEK_CUR);
    }

    if(count == 0) {
        printf("Find no hero. Please re-enter.\n");
        fseek(readFile, 0, SEEK_SET);
        goto enter_target;
    }else if (count != 1) {
        //Enter query(Force, Wise, Policy)
        enter_target_2:
        printf("Find %d hero, Please give more information(-1 for re-enter, -2 for return to menu):\n", count);
        count = 0;
        fseek(readFile, 0, SEEK_SET);
        printf("Force:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->force = tmp;
        printf("Wise:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->wise = tmp;
        printf("Policy:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_2;
        if(tmp == -2) return;
        target->policy = tmp;
        //Query
        while(fileSize - ftell(readFile) >= sizeof(heroinfo)) {
            fread(test, sizeof(heroinfo), 1, readFile);
            if(((test->reputation) == (target->reputation)) && ((test->exp) == (target->exp)) && ((test->soldier) == (target->soldier)) && ((test->force) == (target->force)) && ((test->wise) == (target->wise)) && ((test->policy) == (target->policy))) {
                ++count;
                find[0] = test[0];
                offset = ftell(readFile);
            }
            fseek(readFile,-(sizeof(heroinfo)) + 1 ,SEEK_CUR);
        }
    }
    if(count == 0) {
        printf("Find no Hero. Please re-enter.\n");
        fseek(readFile, 0, SEEK_SET);
        goto enter_target_2;
    }else if (count != 1) {
        //Enter query(Charm, Train, Moral, Loyalty)
        enter_target_3:
        printf("Find %d Hero, Please give more information(-1 for re-enter, -2 for return to menu):\n", count);
        count = 0;
        fseek(readFile, 0, SEEK_SET);
        printf("Charm:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->charm = tmp;
        printf("Train:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->train = tmp;
        printf("Moral:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->moral = tmp;
        printf("Loyalty:");
        scanf(" %ld", &tmp);
        if(tmp == -1) goto enter_target_3;
        if(tmp == -2) return;
        target->loyalty = tmp;
        //Query
        while(fileSize - ftell(readFile) >= sizeof(heroinfo)) {
            fread(test, sizeof(heroinfo), 1, readFile);
            if(((test->reputation) == (target->reputation)) && ((test->exp) == (target->exp)) && ((test->soldier) == (target->soldier)) && ((test->force) == (target->force)) && ((test->wise) == (target->wise)) && ((test->policy) == (target->policy)) && ((test->charm) == (target->charm)) && ((test->train) == (target->train)) && ((test->moral) == (target->moral)) && ((test->loyalty) == (target->loyalty))) {
                ++count;
                find[0] = test[0];
                offset = ftell(readFile);
            }
            fseek(readFile,-(sizeof(heroinfo)) + 1 ,SEEK_CUR);
        }
    }

    //Change Value
    if(count == 1) {
        printf("Enter the data you want to change(-1 for not change):\n");
        printf("Reputation(YongMing)(0 <= enter_num <= 10000):");
        scanf(" %ld", &tmp);
        while((tmp > 10000 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->reputation = tmp;
        }
        printf("Expirence(0 <= enter_num <= 60000):");
        scanf(" %ld", &tmp);
        while((tmp > 60000 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->exp = tmp;
        }
        printf("Soldier(0 <= enter_num <= 20000) max may different from everyone:");
        scanf(" %ld", &tmp);
        while((tmp > 20000 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->soldier = tmp;
        }
        printf("Force(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->force = tmp;
        }
        printf("Wise(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->wise = tmp;
        }
        printf("Policy(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->policy = tmp;
        }
        printf("Charm(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->charm = tmp;
        }
        printf("Train(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->train = tmp;
        }
        printf("Moral(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->moral = tmp;
        }
        printf("Loyalty(0 <= enter_num <= 100):");
        scanf(" %ld", &tmp);
        while((tmp > 100 || tmp < 0) ^ (tmp ==  -1)) {
            printf("Invalid number, Please re-input:");
            scanf(" %ld", &tmp);
        }
        if(tmp != -1) {
            find->loyalty = tmp;
        }
        printf("Skill:");
        find->skills = setSkill(find->skills);
        fseek(readFile, offset - sizeof(heroinfo), SEEK_SET);
        fwrite(find, sizeof(heroinfo), 1, readFile);
        fseek(readFile, 0, SEEK_SET);
        return;
    }
}

