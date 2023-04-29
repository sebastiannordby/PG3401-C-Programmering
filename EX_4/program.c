#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256

int count[BYTE_RANGE];
unsigned char buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
pthread_cond_t cond_full, cond_empty;
int bytes_in_buffer = 0;

void* thread_A(void* arg) {
   FILE* fp = fopen("eksamen_v23_oppgave4_pg2265.txt", "rb");
   if (!fp) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {
      pthread_mutex_lock(&mutex);
      while (bytes_in_buffer == BUFFER_SIZE)
         pthread_cond_wait(&cond_empty, &mutex);

      int read_bytes = fread(buffer + bytes_in_buffer, 1, BUFFER_SIZE - bytes_in_buffer, fp);
      bytes_in_buffer += read_bytes;

      if (read_bytes < BUFFER_SIZE - bytes_in_buffer) {
         pthread_mutex_unlock(&mutex);
         break;
      }
      pthread_cond_signal(&cond_full);
      pthread_mutex_unlock(&mutex);
   }
   fclose(fp);
   pthread_exit(NULL);
}

void* thread_B(void* arg) {
   memset(count, 0, sizeof(count));

   while (1) {
      pthread_mutex_lock(&mutex);
      while (bytes_in_buffer == 0)
         pthread_cond_wait(&cond_full, &mutex);

      for (int i = 0; i < bytes_in_buffer; i++)
         count[buffer[i]]++;

      bytes_in_buffer = 0;
      pthread_cond_signal(&cond_empty);
      pthread_mutex_unlock(&mutex);

      if (bytes_in_buffer == 0)
         break;
   }
   for (int i = 0; i < BYTE_RANGE; i++)
      printf("%d: %d\n", i, count[i]);
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   pthread_t threadA, threadB;
   void* memory_buffer = malloc(BUFSIZE);

   if (pthread_create(&threadA, NULL, readPDF, (void*)memory_buffer) != 0) {
      perror("Could not create thread A");
      exit(1);
   }

   if (pthread_create(&threadB, NULL, countBytes, (void*)memory_buffer) != 0) {
      perror("Could not create thread B");
      exit(1);
   }

   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(1);
   }
   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(1);
   }

   free(memory_buffer);

   return 0;
}









