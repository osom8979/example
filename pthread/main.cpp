
#include <iostream>
#include <pthread.h>

using namespace std;

static pthread_mutex_t g_mutex;
static pthread_cond_t  g_signal;
static pthread_t       g_thread;

static bool g_result = false;

void * callback(void * arg)
{
    cout << "RUNNER BEGIN.\n";

    pthread_mutex_lock(&g_mutex);
    g_result = true;
    pthread_cond_signal(&g_signal);
    pthread_mutex_unlock(&g_mutex);

    cout << "RUNNER END.\n";

    return nullptr;
}

int main(int argc, char ** argv)
{
    cout << "START.\n";
    pthread_mutex_init(&g_mutex, nullptr);
    pthread_cond_init(&g_signal, nullptr);
    pthread_create(&g_thread, nullptr, &callback, nullptr);

    pthread_mutex_lock(&g_mutex);
    while (g_result == false) {
        cout << "WAIT.\n";
        pthread_cond_wait(&g_signal, &g_mutex);
    }
    pthread_mutex_unlock(&g_mutex);

    cout << "LOCK END.\n";

    pthread_join(g_thread, nullptr);
    pthread_cond_destroy(&g_signal);
    pthread_mutex_destroy(&g_mutex);

    cout << "DESTROY.\n";

    return 0;
}

