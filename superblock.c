#include "vsfs.h"
#include <stdio.h>


void read_superblock(FILE *fp, Superblock *sb) {
    fseek(fp, 0, SEEK_SET);
    fread(sb, sizeof(Superblock), 1, fp);
    // Debugging output to verify values
    printf("Superblock read:\n");
    printf("Block Size: %d\n", sb->block_size);
    printf("Total Blocks: %d\n", sb->total_blocks);
    printf("Inode Size: %d\n", sb->inode_size);
    printf("Inode Count: %d\n", sb->inode_count);
}


void validate_superblock(Superblock *sb) {
    if (sb->block_size != BLOCK_SIZE) {
        printf("ERROR: Incorrect block size in superblock\n");
    }

    if (sb->total_blocks != TOTAL_BLOCKS) {
        printf("ERROR: Incorrect total blocks in superblock\n");
    }

    // Cast sb->inode_size to size_t and use %zu for printing
    if (sb->inode_size != INODE_SIZE) {
        printf("ERROR: Incorrect inode size in superblock. Expected %zu, got %zu\n", INODE_SIZE, (size_t)sb->inode_size);
    }

    if (sb->inode_count > INODE_COUNT) {
        printf("ERROR: Inode count exceeds the limit\n");
    }
}
