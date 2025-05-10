#include "vsfs.h"
#include <stdio.h>

void read_bitmaps(FILE *fp, uint8_t *inode_bitmap, uint8_t *data_bitmap) {
    fseek(fp, BLOCK_SIZE * INODE_BITMAP_BLOCK, SEEK_SET);
    fread(inode_bitmap, 1, BLOCK_SIZE, fp);

    fseek(fp, BLOCK_SIZE * DATA_BITMAP_BLOCK, SEEK_SET);
    fread(data_bitmap, 1, BLOCK_SIZE, fp);
}

void validate_data_bitmap(Inode *inodes, uint8_t *data_bitmap) {
    int referenced[64] = {0};

    for (int i = 0; i < INODE_COUNT; ++i) {
        if (inodes[i].links_count > 0 && inodes[i].dtime == 0) {
            for (int j = 0; j < BLOCKS_PER_INODE; j++) {
                int block = inodes[i].blocks[j];
                if (block != 0) {
                    if (block >= DATA_BLOCK_START && block < TOTAL_BLOCKS) {
                    referenced[block] = 1;
                    if (!(data_bitmap[block / 8] & (1 << (block % 8)))) {
                        printf("ERROR: Block %d used by inode %d but not in bitmap\n", block, i);
                        }
                    }
            }
        }
    }
    }
    for (int i = DATA_BLOCK_START; i < TOTAL_BLOCKS; ++i) {
        int used = data_bitmap[i / 8] & (1 << (i % 8));
        if (used && !referenced[i]) {
            printf("ERROR: Block %d marked used but not referenced\n", i);
        }
    }
}
