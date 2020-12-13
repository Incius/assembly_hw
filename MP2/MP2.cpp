

#include <iostream>

#include <pthread.h>
#include <semaphore.h>
#include <windows.h>
#include <chrono>
#include <string>
#include <vector>



using namespace std;



class Semaphore {
private:
    bool            signaled;   // <- changed
    pthread_mutex_t m;
    pthread_cond_t  c;

    void Lock() { pthread_mutex_lock(&m); }          // <- helper inlines added
    void Unlock() { pthread_mutex_unlock(&m); }
public:

    /* -- CONSTRUCTOR/DESTRUCTOR */

    Semaphore(bool);

    //~Semaphore();

    /* -- SEMAPHORE OPERATIONS */

    void P();   // changed to void: you don't return anything
    void V();
};
Semaphore::Semaphore(bool s) {        // don't use leading underscores on identifiers
    signaled = s;
    pthread_mutex_init(&m,nullptr);
}
    void Semaphore::P(){
    Lock();              // added
    while (!signaled){   // this must be a loop, not if!
        pthread_cond_wait(&c, &m);
    }
    signaled = false;
    Unlock();
}

void Semaphore::V(){
    bool previously_signaled;
    Lock();
    previously_signaled = signaled; 
    signaled = true;
    Unlock();  // always release the mutex before signaling
    if (!previously_signaled)
      pthread_cond_signal(&c); // this may be an expensive kernel op, so don't hold mutex
}

Semaphore sem = Semaphore(0);
pthread_t TobacoSmoker;
pthread_t PaperSmoker;
pthread_t MatchesSmoker;
pthread_t Dealer;
void* Smoke(void* i)
{
    int a = *((int*)i);
    // wait for a signal from the main proc
    // by attempting to decrement the semaphore
    sem.P();

    // this call blocks until the semaphore's count
    // is increased from the main proc

    std::cout << "Smoker "<<a<<" started making a sigarette"<<std::endl; // response message

    // wait for 3 seconds to imitate some work
    // being done by the thread
    Sleep(3000);

    std::cout << "Smoker " << a << " finished making a sigarette" << std::endl;

    // signal the main proc back
    sem.V();
    return NULL;
}
void* Deal(void* i)
{
    int j = *((int*)i);
    for (size_t i = 0; i < j; i++)
    {
        int choice = rand() % 3;
        switch (choice)
        {
        case(0):
            std::cout << "Dealer dealt Paper and Matches(0)" << std::endl;
            pthread_create(&TobacoSmoker, 0, Smoke, (void*)&choice);
            break;
        case(1):
            std::cout << "Dealer dealt Tobaco and Matches(1)" << std::endl;
            pthread_create(&PaperSmoker, 0, Smoke, (void*)&choice);
            break;
        case(2):
            std::cout << "Dealer dealt Paper and Tobaco(2)" << std::endl;
            pthread_create(&MatchesSmoker, 0, Smoke, (void*)&choice);
            break;
        }
        sem.V();
        Sleep(4000);
        sem.P();
    }
    return NULL;
}


int main()
{
    int i ;

    std::string input;
    std::cout << "[main] Send the signal\n"; // message
    boolean flag = false;
    do {
        try {
            std::cout << "Input number of sigarettes" << std::endl;
            std::cin >> input;
            i = std::stoi(input);
            flag = true;
        }
        catch (...) {
            std::cout << "incorrect input" << std::endl;
        }
    } while (!flag);
    pthread_create(&Dealer, 0, Deal, (void*)&i);
    pthread_join(Dealer, NULL);
    std::cout << "finish" << std::endl;
}