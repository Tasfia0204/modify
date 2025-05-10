#ifndef VSFS_H
#define VSFS_H

#include <stdint.h>
#include <stdio.h>

// Constants
#define BLOCK_SIZE 1024
#define INODE_COUNT 128
#define INODE_SIZE sizeof(Inode)
#define INODE_TABLE_START 5
#define INODE_BITMAP_BLOCK_NO 3
#define TOTAL_BLOCKS 1024
#define INODE_BITMAP_BLOCK 10  // Example value, adjust according to your logic
#define DATA_BITMAP_BLOCK 20  // Example value, adjust according to your logic
#define DATA_BLOCK_START 100  // Example value, adjust according to your logic
#define BLOCK_COUNT 12
#define BLOCKS_PER_INODE 15  // Or the appropriate value based on your file system design

// Inode structure definition
typedef struct Inode{
    int mode;
    int uid;
    int gid;
    int size;
    int atime;
    int ctime;
    int mtime;
    int dtime; // deletion time
    int links_count;
    int blocks[BLOCK_COUNT]; // Direct blocks
} Inode;

// Superblock structure (with corrected fields)
typedef struct {
    int block_size;
    int total_blocks;
    int inode_size;
    int inode_count;
} Superblock;

// Function declarations
void read_superblock(FILE *fp, Superblock *sb);
void read_bitmaps(FILE *fp, uint8_t *inode_bitmap, uint8_t *data_bitmap);
void read_inodes(FILE *fp, Inode *inodes);
void validate_superblock(Superblock *sb);
void validate_inode_bitmap(Inode *inodes, uint8_t *inode_bitmap);
void validate_data_bitmap(Inode *inodes, uint8_t *data_bitmap);
void check_duplicate_blocks(Inode *inodes);
void check_bad_blocks(Inode *inodes);
void fix_invalid_inodes(FILE *fp, Inode *inodes, uint8_t *inode_bitmap); // for fixing errors

#endif // VSFS_H
