// Note: 
// When running this with the text provided in the exam text (provided-text.txt)
// it does seem to be some numbers off in the frequency count of words.
// But when i try this with the file "working-text.txt" it seems to work.
// This might be my editor, or a bug.
// I guess the sensor(you) would have to evaluate.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFFER_SIZE 4096 // How many characters the buffer can hold at each iteration
#define BYTE_RANGE 95  // ASCII range from 32 to 126, then the range is 95
#define MAX_INPUT_FILE_NAME_LENGTH 20 // Max length of the input file name
#define NUMBER_OF_EXTRA_WORDS 5 // Number of words to count: and, at, it, my, Hamlet

typedef struct ThreadData {
   // Count of the different ASCII characters
   int count[BYTE_RANGE];

   // A buffer of data read from file by thread_a
   unsigned char buffer[BUFFER_SIZE];

   // Mutex is used for synchronization between thread_a and thread_b
   // By using a mutex we can lock this ThreadData so
   // its not modifies if either of the threads are reading
   // or writing data.
   pthread_mutex_t mutex;


   pthread_cond_t cond_full, cond_empty;

   // Number of bytes in the buffer as of know
   int bytes_in_buffer;

   // Name of the file to read data from.
   char file_name[MAX_INPUT_FILE_NAME_LENGTH];

   // Flag to check if thread_a is finished reading from
   // the given file. This can be checked in thread b
   // so the thread know when it can quit.
   bool done_reading; 

} ThreadData;

// Counts number of times the search_word occur in the text_to_search.
// isalpha() checks if a character is in the alphabet or not,
int countWordOccurences(char *text_to_search, char *search_word) {
    int count = 0;
    int text_to_search_len = strlen(text_to_search);
    int search_word_len = strlen(search_word);

    // Loop over every character in the text_to_search    
    for (int i = 0; i < text_to_search_len; i++) {
        // If the character at in the search text is equal
        // to the starting character at the search word
        if (text_to_search[i] == search_word[0]) {
            int j;

            // We start at 1, because the if above checks 
            // whether the first characters match.
            // Then we loop through the other characters and see if those match as well
            // if not we break.
            for (j = 1; j < text_to_search_len; j++) {
                if (text_to_search[i + j] != search_word[j]) 
                    break;
            }

            // If we have not hit the break above, j would be equal to the search_word_len.
            // And if the text is looped through or is not a letter in alpabet
            if (j == search_word_len && 
               (i + j == text_to_search_len || !isalpha(text_to_search[i + j]))) {
                count++;
            }
        }
    }

    return count;
}

// Thread A reads text from a given file, provided by ThreadData.
// Should fill up the buffer with size of makro BUFFER_SIZE.
// When the buffer is filled, it can signal that buffer is ready to read.
void* thread_A(void* arg) {
   ThreadData *params = (ThreadData *) arg;

   FILE* input_file = fopen(params->file_name, "rb");
   if (!input_file) {
      perror("Failed to open file");
      exit(EXIT_FAILURE);
   }

   while (1) {
      pthread_mutex_lock(&params->mutex);

      // While there still is more characters in the buffer, and 
      // params->done_reading is false.
      // ThreadB will signal when its done reading and the execution can continue.
      while (params->bytes_in_buffer == BUFFER_SIZE && !params->done_reading)
         pthread_cond_wait(&params->cond_empty, &params->mutex);

      int read_bytes = fread(params->buffer + params->bytes_in_buffer, 1, 
         BUFFER_SIZE - params->bytes_in_buffer, input_file);
      params->bytes_in_buffer += read_bytes;

      if (read_bytes < BUFFER_SIZE - params->bytes_in_buffer) {
         params->done_reading = true;
         pthread_cond_signal(&params->cond_full);
         pthread_mutex_unlock(&params->mutex);
         break;
      }
   
      pthread_cond_signal(&params->cond_full);
      pthread_mutex_unlock(&params->mutex);
   }

   fclose(input_file);
   pthread_exit(NULL);
}

// Thread B should read data from the the buffer, provided in the ThreadData.
// Thread A fills up the buffer from file, and thread B processes the data
// by counting frequency of ASCII characters from 32-126, and frequency
// of the words: "and", "at", "it", "my", "Hamlet"
void* thread_B(void* arg) {
   ThreadData *params = (ThreadData *) arg;
   char *words[NUMBER_OF_EXTRA_WORDS] = 
      {"and", "at", "it", "my", "Hamlet"}; // Words to search for
   int word_count[NUMBER_OF_EXTRA_WORDS] = {0}; // Holds frequency of words above

   memset(params->count, 0, sizeof(params->count));


   while (1) {
      pthread_mutex_lock(&params->mutex);

      // Wait for data to be read into buffer by thread a.
      while (params->bytes_in_buffer == 0)
         pthread_cond_wait(&params->cond_full, &params->mutex);

      pthread_mutex_unlock(&params->mutex);

      for (int i = 0; i < params->bytes_in_buffer; i++) {
         char c = params->buffer[i];

         // Count only printable ASCII characters.
         if (c >= 32 && c <= 126) 
            params->count[c - 32]++; // c - 32, because arrays starts at 0
      }

      // Count the occurences of the words specified above.
      // Then append the frequency to the appropriate counter.
      // May return 0, but appending 0 does not actually do anything.
      for(int i = 0; i < NUMBER_OF_EXTRA_WORDS; i++) {
         word_count[i] += countWordOccurences(params->buffer, words[i]);
      }

      params->bytes_in_buffer = 0; // Clear the buffer
      pthread_cond_signal(&params->cond_empty);
      pthread_mutex_unlock(&params->mutex);

      // Thread a signals that its done reading from the file, 
      // so thread b can break and print the results.
      if (params->done_reading)
         break;
   }

   // Print the ASCII characters and there frequency.
   for (int i = 0; i < BYTE_RANGE; i++) {
      // ASCII representation of the int
      // Since i starts at zero and the frequency in the array
      // 32 is appended. So index 0 represent ASCII sign nr 32.
      char c = (char) (i + 32); 

      printf("'%c': %d\n", c, params->count[i]);
   }

   // Print frequency of the extra words.
   for (int j = 0; j < NUMBER_OF_EXTRA_WORDS; j++) {
      printf("%s: %d\n", words[j], word_count[j]);
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

   pthread_t threadA, threadB; // Declare threads
   ThreadData *pThreadData = malloc(sizeof(ThreadData)); // Allocate memory for ThreadData
   
   // Try to initialize the thread mutex.
   if(pthread_mutex_init(&pThreadData->mutex, NULL) != 0) {
      perror("Failed to initialize thread mutex");
      return 1;
   }

   // Copy the filename to ThreadData, so thread_a knows
   // which file to read data from.
   strcpy(pThreadData->file_name, fileName);

   // Create thread a
   if (pthread_create(&threadA, NULL, thread_A, (void*) pThreadData) != 0) {
      perror("Could not create thread A");
      exit(EXIT_FAILURE);
   }

   // Create thread b
   if (pthread_create(&threadB, NULL, thread_B, (void*) pThreadData) != 0) {
      perror("Could not create thread B");
      exit(EXIT_FAILURE);
   }

   if (pthread_join(threadA, NULL) != 0) {
      perror("Could not join thread A");
      exit(EXIT_FAILURE);
   }

   if (pthread_join(threadB, NULL) != 0) {
      perror("Could not join thread B");
      exit(EXIT_FAILURE);
   }
 
   // Free threaddata and set pointer to NULL.
   // Not necessary, but feels like its good practise.
   free(pThreadData);
   pThreadData = NULL;

   return EXIT_SUCCESS;
}