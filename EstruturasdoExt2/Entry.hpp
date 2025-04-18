#include <cstdint>
#define EXT2_NAME_LEN 255
struct ext2_dir_entry_2 {
	uint32_t	inode;			/* Inode number */
	uint16_t	rec_len;		/* Directory entry length */
	uint8_t	name_len;		/* Name length */
	uint8_t	file_type;
	char	name[EXT2_NAME_LEN];	/* File name */
};