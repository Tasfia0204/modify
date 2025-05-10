#include "vsfs.h"
#include <stdio.h>
#include <stdint.h>

int main() {
    FILE *fp = fopen("vsfs.img", "rb+");
    if (!fp) {
        perror("Failed to open image");
        return 1;
    }

    Superblock sb;
    Inode inodes[INODE_COUNT];
    uint8_t inode_bitmap[BLOCK_SIZE], data_bitmap[BLOCK_SIZE];

    read_superblock(fp, &sb);
    read_bitmaps(fp, inode_bitmap, data_bitmap);
    read_inodes(fp, inodes);

    validate_superblock(&sb);
    validate_inode_bitmap(inodes, inode_bitmap);
    validate_data_bitmap(inodes, data_bitmap);
    check_duplicate_blocks(inodes);
    check_bad_blocks(inodes);

    fix_invalid_inodes(fp, inodes, inode_bitmap);  // Add this line

    fclose(fp);
    return 0;
}
