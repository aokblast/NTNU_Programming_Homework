#include <cassert>
#include <iostream>
#include <pthread.h>
#include <string>
#include <sys/_pthreadtypes.h>

const int ROUND = 5;
const std::string num_to_box[] = {"paper", "scissors", "stone"};

pthread_cond_t cond_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int selection[2];
int rounds = 1;
bool ready = false;
bool ready_1 = false;

static int judge_win();
static inline void display_banner();

void*
player1_thread(void *) {
    for (int _r = 0; _r < ROUND; ++_r) {
        pthread_mutex_lock(&mut);

        selection[0] = rand() % 3;
        display_banner();
        std::cout << "Player " << 1 << " played " << num_to_box[selection[0]] << ".\n";

        ready_1 = true;
        pthread_cond_signal(&cond_cv);

        while(!ready)
            pthread_cond_wait(&cond_cv, &mut);
        ready = false;

        pthread_mutex_unlock(&mut);
    }
    return NULL;
}

void*
player2_thread(void *) {
    for (int _r = 0; _r < ROUND; ++_r) {
        pthread_mutex_lock(&mut);

        while(!ready_1)
            pthread_cond_wait(&cond_cv, &mut);
        ready_1 = false;

        selection[1] = rand() % 3;
        display_banner();
        std::cout << "Player " << 2 << " played " << num_to_box[selection[1]] << ".\n";

        display_banner();
        switch (int d = judge_win()) {
        case 0:
            std::cout << "A draw!\n";
            break;
        default:
            std::cout << "Player " << d << " wins!\n";
            break;
        }
        ++rounds;

        ready = true;
        pthread_cond_signal(&cond_cv);

        pthread_mutex_unlock(&mut);
    }
    return NULL;
}

static inline void
display_banner() {
    std::cout << "Round " << rounds << ": ";
}

static int
judge_win() {
    if(selection[0] == selection[1])
        return 0;
    switch(selection[0]) {
    case 0:
        return selection[1] == 1 ? 2 : 1;
    case 1:
        return selection[1] == 2 ? 2 : 1;
    case 2:
        return selection[1] == 0 ? 2 : 1;
    }
    return -1;
}


int
main() {
    srand(time(NULL));
    pthread_t p1, p2;

    assert(pthread_create(&p1, NULL, player2_thread, NULL) == 0);
    assert(pthread_create(&p2, NULL, player1_thread, NULL) == 0);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
