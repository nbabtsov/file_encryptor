#ifndef ENCRYPT_H
#define ENCRYPT_H

/* 
 * When the function returns the encryption module is allowed to reset.
 */
void reset_requested();
/*
 * The function is called after the encryption module has finished a reset.
 */
void reset_finished();

/* Functions to perform all I/O, encryption and counting
 * operations.
 */
void init(char *inputFileName, char *outputFileName);
int read_input();
void write_output(int c);
int caesar_encrypt(int c);
void count_input(int c);
void count_output(int c);
int get_input_count(int c);
int get_output_count(int c);
int get_input_total_count();
int get_output_total_count();

#endif // ENCRYPT_H
