#ifndef RAY_BLOCK_CLASSIFY_H
#define RAY_BLOCK_CLASSIFY_H

int** alloc_b2b_connection_table();
int* alloc_block_association_table();

void netlist_info_stat(int** b2b_connections, int* block_associations);
void echo_block_info(char* file, int** b2b_connections, int* block_associations); 

void run_block_classify();
#endif
