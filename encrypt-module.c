#include "encrypt-module.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

FILE *input_file;
FILE *output_file;
int input_counts[256];
int output_counts[256];
int input_total_count; //keeps track of the total count of characters in input file
int output_total_count; //keeps track of total count of characters in output file 
int key = 1;

//get assigned the buffer size when inputted by user
int read_buffer_size, write_buffer_size; 

//character buffers used for processsing input and output
char *read_buffer, *write_buffer;

//all locks used throughout the program, get initalized in main function
sem_t in_mutex, out_mutex, read_in, write_out, count_in, count_out, 
		reset, encrypt_s, encrypt_d; 

//Reader Thread
void *reader()
{
    int c;
	int index;

    while (c != EOF) {

		//Call Read Input for next character
        c = read_input();

		sem_wait(&read_in);
		sem_wait(&in_mutex);

        read_buffer[index] = c;

		sem_post(&in_mutex);
		sem_post(&encrypt_s);
		sem_post(&count_in);

        index = (index + 1) % read_buffer_size;

    }

    pthread_exit(NULL);
}

//Writer Thread
void *writer() {
	char c = 1;
	int index; 

	while(1) {
		sem_wait(&write_out);
		sem_wait(&out_mutex);

		c = write_buffer[index];

		sem_post(&out_mutex);
		sem_post(&encrypt_d);

		index = (index+1) % write_buffer_size;
		
		if (c == EOF) {
			break;
		}

		if (c != -1) {
			write_output(c);
		}
	}
}

void clear_counts() {
	memset(input_counts, 0, sizeof(input_counts));
	memset(output_counts, 0, sizeof(output_counts));
	input_total_count = 0;
	output_total_count = 0;
}

void *random_reset() {
	while (1) {
		sleep(rand() % 11 + 5);
		reset_requested();
		key = rand() % 26;
		clear_counts();
		reset_finished();
	}
}

void init(char *inputFileName, char *outputFileName) {
	pthread_t pid;
	srand(time(0));
	pthread_create(&pid, NULL, &random_reset, NULL);
	input_file = fopen(inputFileName, "r");
	output_file = fopen(outputFileName, "w");
}

//Read Input
int read_input() {
	usleep(10000);
	return fgetc(input_file);
}

void write_output(int c) {
	fputc(c, output_file);
}

int caesar_encrypt(int c) {

	if (c >= 'a' && c <= 'z') {
		c += key;
		if (c > 'z') {
			c = c - 'z' + 'a' - 1;
		}
	} else if (c >= 'A' && c <= 'Z') {
		c += key;
		if (c > 'Z') {
			c = c - 'Z' + 'A' - 1;
		}
	}
	return c;
}

//Input Counter Thread
void *input_counter() {
	
	char c;
	int index;

	while(c!=EOF){

		sem_wait(&count_in);
		sem_wait(&in_mutex);

		c=read_buffer[index];
		sem_post(&in_mutex);
		sem_post(&read_in);
		index=(index+1) % read_buffer_size;

		count_input(c);
	}
}

//Encryptor thread
void *encryption() {

	char c, e;
	int index;
	while(c!=EOF){
		
		sem_wait(&encrypt_s);
		sem_wait(&in_mutex);

		c=read_buffer[index];

		sem_post(&in_mutex);
		sem_post(&read_in);

		e = caesar_encrypt(c);

		sem_wait(&encrypt_d);
		sem_wait(&out_mutex);

		write_buffer[index] = e;

		sem_post(&out_mutex);
		sem_post(&count_out);
		sem_post(&write_out);

		index=(index+1) % write_buffer_size;
	}
}


//Output counter thread
void *output_counter() {

	char c;
	int index;

	while(c != EOF){

		sem_wait(&count_out);
		sem_wait(&out_mutex);

		c = write_buffer[index];

		sem_post(&out_mutex);
		sem_post(&encrypt_d);

		index = (index+1) % write_buffer_size;

		count_output(c);
	}
}

void count_input(int c) {
	input_counts[toupper(c)]++;
	input_total_count++;
}

void count_output(int c) {
	output_counts[toupper(c)]++;
	output_total_count++;
}

int get_input_count(int c) {
	return input_counts[toupper(c)];
}

int get_output_count(int c) {
	return output_counts[toupper(c)];
}

int get_input_total_count() {
	return input_total_count;
}

int get_output_total_count() {
	return output_total_count;
}

//outputs the count of each letter, as well the total count of (any) characters in both input and output files to the console
void display_counts(){ 
	printf("%s %d\n", "Total input count with current key is ", input_total_count);
	for(int i=0; i<256; i++){
		if(i>='A' && i<='Z'){
			printf("%c:%d ", i, get_input_count(i));
		}
	}
	printf("\n");

	printf("%s %d\n", "Total output count with current key is ", output_total_count);
	for(int i=0; i<256; i++){
		if(i>='A' && i<='Z'){
			printf("%c:%d ", i, get_output_count(i));
		}
	}
	printf("\n");
}

void reset_requested(){ 
	printf("%s\n", "Reset requested");
	display_counts();
	sem_post(&reset);
}


void reset_finished(){ 
	sem_wait(&reset);
	printf("%s\n\n", "Reset finished");
}

//takes in input and output file, initializes all the semaphores, allocates memory for buffers, creates all the threads and synchronizes them 
int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Arguments provided are incorrect. \nMust include an input file and an output file.\n");
		exit(1);
	}

	char *input_file_name = argv[1];
	char *output_file_name = argv[2];
	
	sem_init(&reset, 0, 1);
    sem_init(&in_mutex, 0, 1);
	sem_init(&out_mutex, 0, 1);
	sem_init(&write_out, 0, 0);
	sem_init(&count_in, 0, 0);
	sem_init(&count_out, 0, 0);
	sem_init(&encrypt_s, 0, 0);

	//Call init on input and output files
	init(input_file_name, output_file_name);

	printf("Enter a read buffer size: ");
	scanf("%d", &read_buffer_size);

	printf("Enter a write buffer size: ");
	scanf("%d", &write_buffer_size);

	read_buffer = malloc(read_buffer_size); //Allocate memory for the read buffer
    write_buffer = malloc(write_buffer_size); //Allocate memory for the write buffer

	
	sem_init(&read_in, 0, read_buffer_size);
	
	sem_init(&encrypt_d, 0, write_buffer_size);

	pthread_t reader_thread, writer_thread, encryption_thread, input_counter_thread, output_counter_thread;

	pthread_create(&reader_thread, NULL, reader, NULL);
	pthread_create(&writer_thread, NULL, writer, NULL);
	pthread_create(&encryption_thread, NULL, encryption, NULL);
	pthread_create(&input_counter_thread, NULL, input_counter, NULL);
	pthread_create(&output_counter_thread, NULL, output_counter, NULL);

	//Join Threads
	pthread_join(reader_thread, NULL);
	pthread_join(writer_thread, NULL);
	pthread_join(encryption_thread, NULL);
	pthread_join(input_counter_thread, NULL);
	pthread_join(output_counter_thread, NULL);

	//Display Output
	display_counts();
	printf("%s\n", "End of file reached");
	return 0;

}
