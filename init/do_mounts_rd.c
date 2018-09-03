// SPDX-License-Identifier: GPL-2.0
/*
 * Many of the syscalls used in this file expect some of the arguments
 * to be __user pointers not __kernel pointers. To limit the sparse
 * noise, turn off sparse checking for this file.
 */
#ifdef __CHECKER__
#undef __CHECKER__
#warning "Sparse checking disabled for this file"
#endif

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/minix_fs.h>
#include <linux/ext2_fs.h>
#include <linux/romfs_fs.h>
#include <uapi/linux/cramfs_fs.h>
#include <linux/initrd.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "do_mounts.h"
#include "../fs/squashfs/squashfs_fs.h"

#include <linux/decompress/generic.h>

int __initdata rd_prompt = 1; /* 1 = prompt for RAM disk, 0 = don't prompt */

static int __init prompt_ramdisk(char *str)
{
	rd_prompt = simple_strtol(str, NULL, 0) & 1;
	return 1;
}

__setup("prompt_ramdisk=", prompt_ramdisk);

int __initdata rd_image_start; /* starting block # of image */

static int __init ramdisk_start_setup(char *str)
{
	rd_image_start = simple_strtol(str, NULL, 0);
	return 1;
}

__setup("ramdisk_start=", ramdisk_start_setup);

static int __init crd_load(int in_fd, int out_fd, decompress_fn deco);

/*
 * This routine tries to find a RAM disk image to load, and returns the 
 * number of blocks to read for a non-compressed image, 0 if the image
 * is a compressed image, and -1 if an image with the right magic 
 * numbers could not be found.
 *
 * We currently check for the following magic numbers:
 *  minix
 *  ext2
 *  romfs
 *  cramfs
 *  squashfs
 *  gzip
 *  bzip2
 *  lzma
 *  xz
 *  lzo
 *  lz4
 */
static int __init
identify_ramdisk_image(int fd, int start_block, decompress_fn *decompressor)
{
	const int size = 512;
	struct minix_super_block *minixsb;
	struct romfs_super_block *romfsb;
	struct cramfs_super *cramfsb;
	struct squashfs_super_block *squashfsb;
	int nblocks = -1;
	unsigned char *buf;
	const char *compress_name;
	unsigned long n;

	buf = kmalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;
    

}