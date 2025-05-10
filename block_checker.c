// block_checks.c (or similar file where these functions are defined)
#include "vsfs.h"
#include <stdio.h>
void check_bad_blocks(Inode *inodes);
void check_duplicate_blocks(Inode *inodes);

void check_bad_blocks(Inode *inodes) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        if (inodes[i].links_count > 0 && inodes[i].dtime == 0) {
            for (int j = 0; j < BLOCKS_PER_INODE; ++j) {
                int block = inodes[i].blocks[j];  // Access each block
                if (block < DATA_BLOCK_START || block >= TOTAL_BLOCKS) {
                    printf("ERROR: Inode %d references bad block %d\n", i, block);
                }
            }
        }
    }
}

void check_duplicate_blocks(Inode *inodes) {
    int block_map[TOTAL_BLOCKS] = {0};
    for (int i = 0; i < INODE_COUNT; ++i) {
        for (int j = 0; j < BLOCKS_PER_INODE; ++j) {
            int block = inodes[i].blocks[j];  // Access each block
            if (block >= DATA_BLOCK_START && block < TOTAL_BLOCKS) {
                if (block_map[block]) {
                    printf("ERROR: Duplicate block usage detected at block %d\n", block);
                } else {
                    block_map[block] = 1;
                }
            }
        }
    }
}
