#include <pthread.h> // pthread_t, pthread_create
#include <string.h> // strerror
#include <stdio.h> // fprintf puts
#include <unistd.h> // sleep
#include <stdlib.h> // malloc

char* data;

struct buffer {
  void* data;
  // instead of a mutex lock, in this example I use this simple flag
  char flag;
};

void* capture_data_thread(void* args)
{
  struct buffer* buffer = (struct buffer*) args;
  while(1)
  {
    if (!buffer->flag)
    {
      puts("WORK: capturing data");
      data[strlen(data) - 1] += 1;
      memcpy(buffer->data, data, strlen(data) + 1);
      sleep(1);
      buffer->flag = 1;
    }
    else
    {
      puts("SKIP: cruncher hasn't finished");
      sleep(1);
    }
  }
  return NULL;
}

void* event_loop_thread(void* args)
{
  struct buffer* buffer = (struct buffer*) args;
  while(1)
  {
    if (buffer->flag)
    {
      puts("WORK: modifying the single copy of data now");
      puts(buffer->data);
      sleep(1);
      buffer->flag = 0;
    }
    else
    {
      puts("SKIP: enjoying my free time");
      sleep(1);
    }
  }
  return NULL;
}

int main (int argc, char* argv[])
{
  char* starter = "\t\tHello World a";
  int thread_status;
  pthread_attr_t attr;
  pthread_t capture_data_pthread;
  pthread_t event_loop_pthread;
  void* exit_status;

  // A shared buffer
  struct buffer buffer;
  buffer.data = malloc(strlen(starter) + 1);

  data = malloc(strlen(starter) + 1);
  memcpy(data, starter, strlen(starter) + 1);

  puts("Hello World!");

  // Redundant defaults, NULL would do the same thing
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // redundant default

  thread_status = pthread_create(&capture_data_pthread, &attr, capture_data_thread, &buffer);
  if (0 != thread_status)
  {
    fprintf(stderr, "thread creation failed with errno %d (%s)\n", thread_status, strerror(thread_status));
  }

  thread_status = pthread_create(&event_loop_pthread, &attr, event_loop_thread, &buffer);
  if (0 != thread_status)
  {
    fprintf(stderr, "thread creation failed with errno %d (%s)\n", thread_status, strerror(thread_status));
  }

  pthread_attr_destroy(&attr);


  // If the threads are not joined, main will exit
  // and subsequently kill them, possibly never having been run
  pthread_join(capture_data_pthread, &exit_status);
  pthread_join(event_loop_pthread, &exit_status);
  
  puts("Good Bye!");
  return 0;
}
