#if defined (WIN32) || defined (_WIN32)
#   include <Windows.h>
#   define TEST_THREAD_RESULT unsigned long
#   define TEST_THREAD_CREATE(func, arg) assert(CreateThread(NULL, 0, func, arg, 0, NULL) != NULL)
#else
#   include <pthread.h>
#   define TEST_THREAD_RESULT void*
#   define TEST_THREAD_CREATE(func, arg) do { pthread_t thread; assert(pthread_create(&thread, NULL, func, arg) == 0); } while(0)
#endif

TEST_THREAD_RESULT client_start(void* data);