#include <stdint.h>

#define EXT2_NDIR_BLOCKS 12
#define EXT2_IND_BLOCK EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS (EXT2_TIND_BLOCK + 1)

struct ext2_inode
{
    uint16_t i_mode;        /* File mode */
    uint16_t i_uid;         /* Low 16 bits of Owner Uid */
    uint32_t i_size;        /* Size in bytes */
    uint32_t i_atime;       /* Access time */
    uint32_t i_ctime;       /* Creation time */
    uint32_t i_mtime;       /* Modification time */
    uint32_t i_dtime;       /* Deletion Time */
    uint16_t i_gid;         /* Low 16 bits of Group Id */
    uint16_t i_links_count; /* Links count */
    uint32_t i_blocks;      /* Blocks count */
    uint32_t i_flags;       /* File flags */
    union
    {
        struct
        {
            uint32_t l_i_reserved1;
        } linux1;
        struct
        {
            uint32_t h_i_translator;
        } hurd1;
        struct
        {
            uint32_t m_i_reserved1;
        } masix1;
    } osd1;                       /* OS dependent 1 */
    uint32_t i_block[EXT2_N_BLOCKS]; /* Pointers to blocks */
    uint32_t i_generation;           /* File version (for NFS) */
    uint32_t i_file_acl;             /* File ACL */
    uint32_t i_dir_acl;              /* Directory ACL */
    uint32_t i_faddr;                /* Fragment address */
    union
    {
        struct
        {
            uint8_t l_i_frag;  /* Fragment number */
            uint8_t l_i_fsize; /* Fragment size */
            uint16_t i_pad1;
            uint16_t l_i_uid_high; /* these 2 fields    */
            uint16_t l_i_gid_high; /* were reserved2[0] */
            uint32_t l_i_reserved2;
        } linux2;
        struct
        {
            uint8_t h_i_frag;  /* Fragment number */
            uint8_t h_i_fsize; /* Fragment size */
            uint16_t h_i_mode_high;
            uint16_t h_i_uid_high;
            uint16_t h_i_gid_high;
            uint32_t h_i_author;
        } hurd2;
        struct
        {
            uint8_t m_i_frag;  /* Fragment number */
            uint8_t m_i_fsize; /* Fragment size */
            uint16_t m_pad1;
            uint32_t m_i_reserved2[2];
        } masix2;
    } osd2; /* OS dependent 2 */
};