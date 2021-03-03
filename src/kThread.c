#include "utils.h"
#include "kThread.h"

#ifdef __cplusplus
#include <functional>
#include <thread>
threaded_job::threadedJob(std::function<void(void)> job)
{
  this->job = job;
  thread = threadLaunch([this]() {
    while (alive)
    {
      if (jobStart)
      {
        jobStart = false;
        jobEnded = false;
        this->job();
        jobEnded = true;
      }
      sleepMillis(1); // WHY DO I NEED THIS?!?!?
    }
  });
}
threaded_job::threaded_job(const threaded_job &base) : threaded_job(base.job) {}
threaded_job::~threaded_job()
{
  this->alive = false;
  kjoin(thread);
}
void threaded_job::start() // start job
{
  if (jobEnded)
  {
    jobStart = true;
    jobEnded = false;
  }
}
void threaded_job::join() // wait until job has finished executing
{
  while (!jobEnded)
  {
  }
}
void threaded_job::stop() //use with caution, if used from inside thread, will cause hang loop
{
  this->alive = false;
  kjoin(thread);
}
void threaded_job::detach() //stops thread but will only join when object destroyed
{
  alive = false;
}
void threaded_job::changeJob(std::function<void(void)> newJob) // changes the function called on job.start(), not tested
{
  this->job = newJob;
}
#else
static threaded_job jobInit(kThread thread, void (*function)())
{
  threaded_job job = {thread, function, false, false, true};
  return job;
}
#ifdef _WIN32
#include <windows.h>
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

static DWORD WINAPI threaded_jobFunc(void *pJob)
{
  // threaded_job *job = (threaded_job *)job;
  // while (job->alive)
  // {
  //   if (job->jobStart)
  //   {
  //     job->jobStart = false;
  //     job->jobEnded = false;
  //     job->job();
  //     job->jobEnded = true;
  //   }
  //   sleepMillis(1);
  // }
  return 0;
}
threaded_job threaded_jobInit(void (*function)())
{
  DWORD ThreadId;
  threaded_job job;
  HANDLE ThreadHandle = CreateThread(NULL, 0, threaded_jobFunc, &job, 0, &ThreadId);
  kThread thread = {ThreadHandle};
  job = jobInit(thread, function);
  return job;
}

#elif defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
static void *threadLaunchFunction(void *function)
{
  ((void (*)())function)();
  return NULL;
}
kThread kThreadInit(pthread_t threadIndex)
{
  kThread thread = {threadIndex};
  return thread;
}
void kjoin(kThread thread)
{
  pthread_join(thread.threadIndex, NULL);
}
void threadDetach(void (*function)())
{
  pthread_t thread;
  pthread_create(&thread, NULL, threadLaunchFunction, function);
  pthread_detach(thread);
}
kThread threadLaunch(void (*function)())
{
  pthread_t threadIndex;
  pthread_create(&threadIndex, NULL, threadLaunchFunction, function);
  return kThreadInit(threadIndex);
}

static void *threaded_jobFunc(void *pJob)
{
  threaded_job *job = (threaded_job *)job;
  while (job->alive)
  {
    if (job->jobStart)
    {
      job->jobStart = false;
      job->jobEnded = false;
      job->job();
      job->jobEnded = true;
    }
    sleepMillis(1);
  }
  return NULL;
}
threaded_job threaded_jobInit(void (*function)())
{
  pthread_t threadIndex;
  threaded_job job;
  pthread_create(&threadIndex, NULL, threaded_jobFunc, &job);
  kThread thread = {threadIndex};
  job = jobInit(thread, function);
  return job;
}
#endif

#endif
