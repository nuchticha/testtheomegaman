#include <pthread.h>
#include "tobject_count.h"

static int object_count;
pthread_mutex_t object_count_mutex = PTHREAD_MUTEX_INITIALIZER;
 
void object_count_init()
{
  object_count = 0;
}

void object_count_inc()
{
  pthread_mutex_lock(&object_count_mutex);
  object_count++;
  pthread_mutex_unlock(&object_count_mutex);
}

void object_count_dec()
{
  pthread_mutex_lock(&object_count_mutex);
  object_count--;
  pthread_mutex_unlock(&object_count_mutex);
}

int object_count_get()
{
  return object_count;
}