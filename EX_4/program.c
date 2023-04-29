#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
 // Ascci range from 32 to 126, then the range is 95
#define BYTE_RANGE 95
// Max length of the input file
#define MAX_INPUT_FILE_NAME_LENGTH 20
#define NUMBER_OF_EXTRA_WORDS 5

typedef struct ThreadData {
   int count[BYTE_RANGE];
   unsigned char buffer[BUFFER_SIZE];
   pthread_mutex_t mutex;
   pthread_cond_t cond_full, cond_empty;
   int bytes_in_buffer;
   char file_name[MAX_INPUT_FILE_NAME_LENGTH];
} ThreadData;

void* thread_A(void* arg) {
   ThreadData *params = (ThreadData *) arg;

   FILE* fp = fopen(params->file_name, "rb");
   if (!fp) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {
      pthread_mutex_lock(&params->mutex);
      while (params->bytes_in_buffer == BUFFER_SIZE)
         pthread_cond_wait(&params->cond_empty, &params->mutex);

      int read_bytes = fread(params->buffer + params->bytes_in_buffer, 1, BUFFER_SIZE - params->bytes_in_buffer, fp);
      params->bytes_in_buffer += read_bytes;

      printf("BYTES JUST READ BUDDY\r\n");
      
      if (read_bytes < BUFFER_SIZE - params->bytes_in_buffer) {
         pthread_mutex_unlock(&params->mutex);
         break;
      }
   
      pthread_cond_signal(&params->cond_full);
      pthread_mutex_unlock(&params->mutex);
   }

   fclose(fp);
   pthread_exit(NULL);
}

void* thread_B(void* arg) {
   ThreadData *params = (ThreadData *) arg;
   memset(params->count, 0, sizeof(params->count));

   while (1) {
      pthread_mutex_lock(&params->mutex);
      while (params->bytes_in_buffer == 0)
         pthread_cond_wait(&params->cond_full, &params->mutex);

      for (int i = 0; i < params->bytes_in_buffer; i++) {
         char c = params->buffer[i];

         // Count only printable ASCII characters
         if (c >= 32 && c <= 126) 
            params->count[c - 32]++;
      }

      params->bytes_in_buffer = 0;
      pthread_cond_signal(&params->cond_empty);
      pthread_mutex_unlock(&params->mutex);

      if (feof(stdin))
         break;
   }

   for (int i = 0; i < BYTE_RANGE; i++) {
      char c = (char) (i + 32);
      printf("'%c': %d\n", c, params->count[i]);
   }

   pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
   printf("Argc: %d\r\n", argc);

   char* fileName = argv[1];
   printf("Test: %s\r\n", fileName);

   if(argc > 2) {
      printf("Too many arguments.\r\n");
      return 1;
   } else if(argc < 2) {
      printf("File name must be passed as argument when starting the program.\r\n");
      return 1;
   } else if(strlen(fileName) > MAX_INPUT_FILE_NAME_LENGTH) {
      printf("Filename exceeds length. Max length is %d\r\n", MAX_INPUT_FILE_NAME_LENGTH);
      return 1;
   }

   pthread_t threadA, threadB;
   void* memory_buffer = malloc(BUFFER_SIZE);
   ThreadData *pThreadData = malloc(sizeof(ThreadData));
   strcpy(pThreadData->file_name, fileName);

   if (pthread_create(&threadA, NULL, thread_A, (void*) pThreadData) != 0) {
      perror("Could not create thread A");
      exit(1);
   }

   if (pthread_create(&threadB, NULL, thread_B, (void*) pThreadData) != 0) {
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
   free(pThreadData);

   return 0;
}