#pragma once

#include "kutils.h"

#ifdef __cplusplus
#include <functional>
#include <thread>
struct kThread
{
  std::thread*thread;
};
static void kjoin(kThread &thread)
{
  thread.thread->join();
}
static void kThreadDetach(std::function<void(void*)> func,void*arg)
{
  std::thread t = std::thread(func,arg);
  t.detach();
}
static kThread kThreadLaunch(std::function<void(void*)> func,void*arg)
{
  std::thread*thread=new std::thread(func,arg);
  kThread t = {thread};
  t.thread=thread;
  return t;
}
class threaded_job final
{ // use "fflush(stdout)" if printing
private:
  bool jobStart = false, jobEnded = true, alive = true;
  void*arg=NULL;
  kThread thread;
  std::function<void(void*)> job;

public:
  threaded_job(std::function<void(void*)> job)
  {
    this->job = job;
    std::function<void(void*)>f;
    thread = kThreadLaunch([this](void*a)->void{
      while (alive)
      {
        if (jobStart)
        {
          jobStart = false;
          jobEnded = false;
          this->job(this->arg);
          jobEnded = true;
        }
        sleepMillis(1); // WHY DO I NEED THIS?!?!?
      }
    },NULL);
  }
  threaded_job(const threaded_job &base) : threaded_job(base.job) {}
  ~threaded_job()
  {
    this->alive = false;
    kjoin(thread);
  }

  void start(void*arg=NULL) // start job
  {
    if (jobEnded)
    {
      this->arg=arg;
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
  void changeJob(std::function<void(void*)> newJob) // changes the function called on job.start(), not tested
  {
    this->job = newJob;
  }
};
#else
#ifdef _WIN32
// windows.h already included
typedef struct kThread
{
  HANDLE threadHandle;
} kThread;
static void kjoin(kThread thread)
{
  if (thread.threadHandle != NULL)
  {
    WaitForSingleObject(thread.threadHandle, INFINITE);
    CloseHandle(thread.threadHandle);
  }
}
static void kThreadDetach(void *(*function)(void *),void*arg)
{
  DWORD ThreadId;
  HANDLE ThreadHandle = CreateThread(NULL, 0, function, arg, 0, &ThreadId);
  // TODO : detach thread
}
static kThread kThreadLaunch(void *(*function)(void *),void*arg)
{
  DWORD ThreadId;
  HANDLE ThreadHandle = CreateThread(NULL, 0, function, arg, 0, &ThreadId);
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

typedef struct threaded_job
{
    bool jobStart;
    bool jobEnded;
    bool alive;
    kThread thread;
    void* (*job)(void*);
    void* arg;
} threaded_job;

threaded_job threaded_jobInit(void (*function)())
{
  DWORD ThreadId;
  HANDLE ThreadHandle = CreateThread(NULL, 0, threaded_jobFunc, NULL, 0, &ThreadId);
  kThread thread = {ThreadHandle};
  //job = jobInit(thread, function);
  threaded_job job={ false, true, true,thread,function,NULL };

  return job;
}
#elif defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
typedef struct kThread
{
  pthread_t threadIndex;
} kThread;
static kThread kThreadInit(pthread_t threadIndex)
{
  kThread k = {threadIndex};
  return k;
}
static void kjoin(kThread thread)
{
  pthread_join(thread.threadIndex, NULL);
}
static void kThreadDetach(void*(*function)(void*),void*arg)
{
  pthread_t thread;
  pthread_create(&thread, NULL, function, arg);
  pthread_detach(thread);
}
static kThread kThreadLaunch(void*(*function)(void*),void*arg)
{
  pthread_t threadIndex;
  pthread_create(&threadIndex, NULL, function, arg);
  return kThreadInit(threadIndex);
}

typedef struct threaded_job
{
  bool jobStart;
  bool jobEnded;
  bool alive;
  kThread thread;
  void *(*job)(void *);
  void *arg;
} threaded_job;

#include <stdio.h>

static threaded_job threaded_jobInit(void *(*function)(void *))
{
  threaded_job job = {false, true, true,{0},function,NULL};
  return job;
}
#define threaded_job(function) threaded_jobInit(function)
static void *threaded_jobFunc(void *pJob)
{
  threaded_job *job = (threaded_job *)pJob;
  while (job->alive)
  {
    if (job->jobStart)
    {
      job->jobStart = false;
      job->jobEnded = false;
      job->job(job->arg);
      job->jobEnded = true;
    }
    sleepMillis(1);
  }
  return NULL;
}
static void activateJob(threaded_job*job){
  pthread_t threadIndex;
  pthread_create(&threadIndex, NULL, threaded_jobFunc, job);
  kThread thread = {threadIndex};
  job->thread=thread;
}
#define threaded_jobCreateActivate(jobName, function) threaded_job(function);activateJob(&jobName);
static void threaded_jobStart(threaded_job *job,void*arg)
{
  if(job->jobEnded){
    job->arg=arg;
    job->jobEnded=false;
    job->jobStart=true;
  }
}
static void threaded_jobJoin(threaded_job *job) // wait until job has finished executing
{
  while(!job->jobEnded){
  }
}
static void threaded_jobStop(threaded_job*job) // use with caution, if used from inside thread, will cause hang loop
{
  job->alive=false;
  kjoin(job->thread);
}
static void threaded_jobDetach(threaded_job*job) // stops thread but will only join when object destroyed
{
  job->alive=false;
}

#endif
#endif
