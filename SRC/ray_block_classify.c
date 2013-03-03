#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "util.h"
#include "vpr_types.h"
#include "globals.h"

int** alloc_b2b_connection_table() 
{
    int **b2b_connections;
    int i;

    b2b_connections = (int**)malloc(num_blocks * sizeof(int*));
    for (i = 0; i < num_blocks; i++) {
        b2b_connections[i] = (int*)malloc(num_blocks * sizeof(int));
    }
    /*b2b_connections = alloc_matrix(0, num_blocks - 1, 0, num_blocks -1, sizeof(int));*/
    for (i = 0; i < num_blocks; i++) {
        memset(b2b_connections[i], 0, num_blocks * sizeof(int));
    }
    return b2b_connections;
}

int* alloc_block_association_table()
{
    int *block_associations = malloc(num_blocks * sizeof(int));
    memset(block_associations, 0, num_blocks * sizeof(int));
    return block_associations;
}

void netlist_info_stat(int** b2b_connections, int* block_associations)
{
    int i, j, k;
    int net_id, blk_dst;

    for (i = 0; i < num_blocks; i++) {
        for (j = 0; j < block[i].type->num_pins; j++) {
            if (block[i].nets[j] != OPEN) {
                net_id = block[i].nets[j];

                for (k = 0; k < net[net_id].num_sinks + 1; k++) {
                    blk_dst = net[net_id].node_block[k];
                    if (blk_dst == i) continue;
                    b2b_connections[i][blk_dst]++;
                }
            }
        }
    }
    for (i = 0; i < num_blocks; i++) {
        for (j = 0; j < num_blocks; j++) {
            if (b2b_connections[i][j] > 0) {
                block_associations[i]++;
            }
        }
    }

}

void echo_block_info(char* file, int** b2b_connections, int* block_associations) 
{
    FILE *fp;
    int i, j;

    fp = fopen(file, "w");
    for (i = 0; i < num_blocks; i++) {
        for (j = 0; j < num_blocks; j++) {
            if (b2b_connections[i][j] != 0) {
                fprintf(fp, "block %d and block %d connections: %d\n", i, j, b2b_connections[i][j]);
            }
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n");
    for (i = 0; i < num_blocks; i++) {
        fprintf(fp, "block %d %s associations: %d\n", i, block[i].name, block_associations[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
}

void run_block_classify() 
{
    int **b2b_connections, *block_associations;
    char outfile[] = "block_stat.txt";

    b2b_connections = alloc_b2b_connection_table();
    block_associations = alloc_block_association_table();

    netlist_info_stat(b2b_connections, block_associations);
    echo_block_info(outfile, b2b_connections, block_associations);
}

