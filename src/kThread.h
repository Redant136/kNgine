#pragma once

#include "utils.h"

#ifdef __cplusplus
#include <functional>
#include <thread>
struct kThread
{
  std::thread thread;
};
static void kjoin(kThread &thread)
{
  thread.thread.join();
}
static void threadDetach(std::function<void(void)> func)
{
  std::thread t = std::thread(func);
  t.detach();
}
static kThread threadLaunch(std::function<void(void)> func)
{
  return {std::thread(func)};
}
class threaded_job final
{ // use "fflush(stdout)" if printing
private:
  bool jobStart = false, jobEnded = true, alive = true;
  kThread thread;
  std::function<void(void)> job;

public:
  threaded_job(std::function<void(void)> job);
  threaded_job(const threaded_job &base);
  ~threaded_job();
  void start() // start job
  {
    if (jobEnded)
    {
      jobStart = true;
      jobEnded = false;
    }
  }
  void join() // wait until job has finished executing
  {
    while (!jobEnded)
    {
    }
  }
  void stop() //use with caution, if used from inside thread, will cause hang loop
  {
    this->alive = false;
    kjoin(thread);
  }
  void detach() //stops thread but will only join when object destroyed
  {
    alive = false;
  }
  void changeJob(std::function<void(void)> newJob) // changes the function called on job.start(), not tested
  {
    this->job = newJob;
  }
};
#else
#ifdef _WIN32
// windows.h already included
static DWORD WINAPI threadLaunchFunction(LPVOID Param)
{
  ((void (*)())function)();
  return 0;
}
typedef struct kThread
{
  Handle threadHandle;
} kThread;
static void kjoin(kThread thread)
{
  if (thread.threadHandle != NULL)
  {
    WaitForSingleObject(thread.threadHandle, INFINITE);
    CloseHandle(thread.threadHandle);
  }
}
static void threadDetach(void *(*function)(void *))
{
  DWORD ThreadId;
  HANDLE ThreadHandle = CreateThread(NULL, 0, threadLaunchFunction, function, 0, &ThreadId);
  // TODO : detach thread
}
static kThread threadLaunch(void *(*function)(void *))
{
  DWORD ThreadId;
  HANDLE ThreadHandle = CreateThread(NULL, 0, threadLaunchFunction, function, 0, &ThreadId);
  kThread t = {ThreadHandle};
}
#elif defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
typedef struct kThread
{
  pthread_t threadIndex;
} kThread;
void kjoin(kThread thread);
void threadDetach(void (*function)());
kThread threadLaunch(void (*function)());
#endif
typedef struct threaded_job
{
  kThread thread;
  void (*job)();
  bool jobStart;
  bool jobEnded;
  bool alive;
} threaded_job;
threaded_job threaded_jobInit(void (*function)());
#define threaded_job(function) threaded_jobInit(function)
#endif
