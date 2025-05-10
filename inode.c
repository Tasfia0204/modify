#include "vsfs.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Function to read the inode table from the filesystem image
void read_inodes(FILE *fp, Inode *inodes) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        fseek(fp, BLOCK_SIZE * INODE_TABLE_START + i * INODE_SIZE, SEEK_SET);
        fread(&inodes[i], sizeof(Inode), 1, fp);
    }
}

// Function to validate the inode bitmap, checking consistency
void validate_inode_bitmap(Inode *inodes, uint8_t *inode_bitmap) {
    for (int i = 0; i < INODE_COUNT; ++i) {
        // Check if the inode is marked in the bitmap
        int bitmap_set = inode_bitmap[i / 8] & (1 << (i % 8));

        // Check if the inode is valid (has links and is not deleted)
        int valid_inode = (inodes[i].links_count > 0 && inodes[i].dtime == 0);

        if (bitmap_set && !valid_inode) {
            printf("ERROR: Inode %d marked used but invalid\n", i);
        }

        if (!bitmap_set && valid_inode) {
            printf("ERROR: Inode %d valid but not marked in bitmap\n", i);
        }
    }
}

// Function to fix invalid inodes and update the bitmap accordingly
void fix_invalid_inodes(FILE *fp, Inode *inodes, uint8_t *inode_bitmap) {
    int fixed = 0;

    for (int i = 0; i < INODE_COUNT; ++i) {
        // Check if the inode is marked in the bitmap
        int bitmap_set = inode_bitmap[i / 8] & (1 << (i % 8));

        // Check if the inode is valid
        int valid_inode = (inodes[i].links_count > 0 && inodes[i].dtime == 0);

        if (bitmap_set && !valid_inode) {
            // Clear the bitmap bit
            inode_bitmap[i / 8] &= ~(1 << (i % 8));

            // Zero out the inode
            Inode empty = {0};
            fseek(fp, BLOCK_SIZE * INODE_TABLE_START + i * INODE_SIZE, SEEK_SET);
            fwrite(&empty, sizeof(Inode), 1, fp);

            printf("FIXED: Inode %d marked used but invalid — cleared\n", i);
            fixed++;
        }
    }

    if (fixed > 0) {
        // Save the updated inode bitmap
        fseek(fp, BLOCK_SIZE * INODE_BITMAP_BLOCK_NO, SEEK_SET);
        fwrite(inode_bitmap, BLOCK_SIZE, 1, fp);
    }
}
