#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

sem_t sem_tobacco, sem_paper, sem_matches, sem_barber;
bool is_done = false;

void* smoker(void* arg) {
    char item_needed = *((char*)arg);

    while (!is_done) {
        if (item_needed == 'T') {
            sem_wait(&sem_tobacco);
        } else if (item_needed == 'P') {
            sem_wait(&sem_paper);
        } else if (item_needed == 'M') {
            sem_wait(&sem_matches);
        }

        if (!is_done) {
            std::cout << item_needed << std::endl;
            sleep(1);
        }
    }
    return nullptr;
}

void* bartender(void*) {
    std::string input;
    std::cin >> input;

    size_t index = 0;
    while (index < input.size() && !is_done) {
        char item = input[index++];

        if (item == 'T') {
            sem_post(&sem_tobacco);
        } else if (item == 'P') {
            sem_post(&sem_paper);
        } else if (item == 'M') {
            sem_post(&sem_matches);
        }

        sleep(1);
    }

    is_done = true;
    kill(0, SIGTERM);
    return nullptr;
}

int main() {
    sem_init(&sem_tobacco, 0, 0);
    sem_init(&sem_paper, 0, 0);
    sem_init(&sem_matches, 0, 0);
    sem_init(&sem_barber, 0, 0);

    pthread_t smokerT, smokerP, smokerM;
    char t = 'T', p = 'P', m = 'M';
    pthread_create(&smokerT, nullptr, smoker, &t);
    pthread_create(&smokerP, nullptr, smoker, &p);
    pthread_create(&smokerM, nullptr, smoker, &m);

    pthread_t bartender_thread;
    pthread_create(&bartender_thread, nullptr, bartender, nullptr);

    pthread_join(smokerT, nullptr);
    pthread_join(smokerP, nullptr);
    pthread_join(smokerM, nullptr);
    pthread_join(bartender_thread, nullptr);

    sem_destroy(&sem_tobacco);
    sem_destroy(&sem_paper);
    sem_destroy(&sem_matches);
    sem_destroy(&sem_barber);

    return 0;
}