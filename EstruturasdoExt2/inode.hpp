
#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

struct ext2_inode {
    __u16	i_mode;		/* File mode */
    __u16	i_uid;		/* Low 16 bits of Owner Uid */
    __u32	i_size;		/* Size in bytes */
    __u32	i_atime;	/* Access time */
    __u32	i_ctime;	/* Creation time */
    __u32	i_mtime;	/* Modification time */
    __u32	i_dtime;	/* Deletion Time */
    __u16	i_gid;		/* Low 16 bits of Group Id */
    __u16	i_links_count;	/* Links count */
    __u32	i_blocks;	/* Blocks count */
    __u32	i_flags;	/* File flags */
    union {
    struct {
    __u32  l_i_reserved1;
    } linux1;
    struct {
    __u32  h_i_translator;
    } hurd1;
    struct {
    __u32  m_i_reserved1;
    } masix1;
    } osd1;				/* OS dependent 1 */
    __u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
    __u32	i_generation;	/* File version (for NFS) */
    __u32	i_file_acl;	/* File ACL */
    __u32	i_dir_acl;	/* Directory ACL */
    __u32	i_faddr;	/* Fragment address */
    union {
    struct {
    __u8	l_i_frag;	/* Fragment number */
    __u8	l_i_fsize;	/* Fragment size */
    __u16	i_pad1;
    __u16	l_i_uid_high;	/* these 2 fields    */
    __u16	l_i_gid_high;	/* were reserved2[0] */
    __u32	l_i_reserved2;
    } linux2;
    struct {
    __u8	h_i_frag;	/* Fragment number */
    __u8	h_i_fsize;	/* Fragment size */
    __u16	h_i_mode_high;
    __u16	h_i_uid_high;
    __u16	h_i_gid_high;
    __u32	h_i_author;
    } hurd2;
    struct {
    __u8	m_i_frag;	/* Fragment number */
    __u8	m_i_fsize;	/* Fragment size */
    __u16	m_pad1;
    __u32	m_i_reserved2[2];
    } masix2;
    } osd2;				/* OS dependent 2 */	
};