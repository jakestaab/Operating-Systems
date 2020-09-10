/*
About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  arguments.
- The program reads words (one word per line) from standard input until EOF or
  an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.
E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "smp0_tests.h"

#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct {
  char *word;
  int counter;
} WordCountEntry;


int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0;
  char buffer[50];
  char *word;

  while (fgets(buffer, 50, stdin) != NULL) {
    if (*buffer == '.')
      break;
  
	word = strtok(buffer, " \t\n");
    /* Compare against each entry */
	while(word != NULL)
	{
		for(int i=0; i < entry_count; i++)
		{
			if(!strcmp(word, entries[i].word))
				entries[i].counter++;
		}
		word = strtok(NULL, " \t\n");
	}
	line_count++;
  }
  return line_count;
}

//prints the results of the program either to the screen or to user named file
void print_result(WordCountEntry entries[], int entry_count, FILE *output)
{
  fprintf(output, "Result:\n");
  for(int i = 0; i < entry_count; i++) { // Modified this line
    fprintf(output, "%s:%d\n", entries[i].word, entries[i].counter); // Modified this line
  }
  if(output != stdout)
	  fclose(output);
}


void printHelp(const char *name)
{
  printf("usage: %s [-h] <word1> ... <wordN>\n", name);
}


int main(int argc, char **argv)
{
  char *prog_name;

  WordCountEntry *entries;
  int entryCount = 0;
  FILE* output = stdout;
  
  if((entries = malloc(sizeof(WordCountEntry)*(argc-1))) == NULL) {
	  fprintf(stderr, "Cannot allocate memory");
	  return EXIT_FAILURE;
  }

  /* Entry point for the testrunner program */
  if (argc > 1 && !strcmp(argv[1], "-test")) {
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
  }
  
  argv++;
  
  while(*argv != NULL)
  {
	  
	if (**argv == '-') {
		// "-fFILENAME" prints results to file FILENAME
		if((*argv)[1] == 'f')
		{
			prog_name = *argv+2;
			if((output = fopen(prog_name, "w")) == NULL)
			{
				fprintf(stderr, "cannot open file to write");
				free(entries);
				return EXIT_FAILURE;
			}
		} else if (**argv == '-') { //Prints help message
			    switch ((*argv)[1]) {
				case 'h':
				  printHelp(prog_name);
				  return EXIT_FAILURE;
				  break;
				default:
				  printf("%s: Invalid option %s. Use -h for help.\n",
						 prog_name, *argv);
					return EXIT_FAILURE;
				}
      }
    } else
	{
		entries[entryCount].word = *argv;
		entries[entryCount].counter = 0;
		entryCount++;
	}
	argv++;
  }
  
  fprintf(stdout, "Enter sentences. Enter . in a new line to stop\n");

  process_stream(entries, entryCount);
  if(output != stdout)
	  fprintf(stderr, "The result is saved to file %s\n", prog_name);
  print_result(entries, entryCount, output);

  return EXIT_SUCCESS;
}
