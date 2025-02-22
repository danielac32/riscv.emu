
#ifndef _FS_H_
#define _FS_H_

#include <stdio.h>
#include <kernel.h>
#include <interrupt.h>
#include <stdlib.h>
#include <string.h>
//#include <xinu.h>
#include <lfs.h>
#include <lfs_util.h>

extern unsigned int fs;
//extern struct lfs_config config;
//extern lfs_t lfs;
//extern lfs_file_t file;
//extern lfs_dir_t dir;

static int static_read(const struct lfs_config *c, lfs_block_t block,
lfs_off_t offset, void *buffer, lfs_size_t size) {
uint32_t q = disable();
//Serial.printf("    ram rd: block=%d, offset=%d, size=%d\n", block, offset, size);
uint32_t index = block * c->block_size + offset;
memcpy(buffer, (uint8_t *)(c->context) + index, size);
restore(q);
return 0;
}
static int static_prog(const struct lfs_config *c, lfs_block_t block,
lfs_off_t offset, const void *buffer, lfs_size_t size) {
uint32_t q = disable();
//Serial.printf("    ram wr: block=%d, offset=%d, size=%d\n", block, offset, size);
uint32_t index = block * c->block_size + offset;
memcpy((uint8_t *)(c->context) + index, buffer, size);
restore(q);
return 0;
}
static int static_erase(const struct lfs_config *c, lfs_block_t block) {
uint32_t q = disable();
uint32_t index = block * c->block_size;
memset((uint8_t *)(c->context) + index, 0xFF, c->block_size);
restore(q);
return 0;
}
static int static_sync(const struct lfs_config *c) {

return 0;
}


#if 0
static int static_read(const struct lfs_config *c, lfs_block_t block,
lfs_off_t offset, void *buffer, lfs_size_t size) {
//Serial.printf("    ram rd: block=%d, offset=%d, size=%d\n", block, offset, size);
uint32_t index = block * c->block_size + offset;
memcpy(buffer, (uint8_t *)(c->context) + index, size);
return 0;
}
static int static_prog(const struct lfs_config *c, lfs_block_t block,
lfs_off_t offset, const void *buffer, lfs_size_t size) {
//Serial.printf("    ram wr: block=%d, offset=%d, size=%d\n", block, offset, size);
uint32_t index = block * c->block_size + offset;
memcpy((uint8_t *)(c->context) + index, buffer, size);
return 0;
}
static int static_erase(const struct lfs_config *c, lfs_block_t block) {
uint32_t index = block * c->block_size;
memset((uint8_t *)(c->context) + index, 0xFF, c->block_size);
return 0;
}
static int static_sync(const struct lfs_config *c) {

return 0;
}


int file_open(const char *path, int flags,lfs_file_t *file);
int file_write(void *pbuf, int size, lfs_file_t *file);
int file_close(lfs_file_t *file);
int file_read(void *pbuf, int size, lfs_file_t *file);
int available(lfs_file_t *file);

#endif

#define FILE            lfs_file_t
#define DIR 			lfs_dir_t


typedef struct littlefs_s
{
	lfs_t *lfs;
    lfs_file_t *(*open)(const char *path, int flags);
    int (*write)(void *pbuf, int size, lfs_file_t *file);
    int (*read)(void *pbuf, int size, lfs_file_t *file);
    int (*close)(lfs_file_t *file);
    int (*seek)(lfs_file_t *file, int offset, int whence);
    int (*tell)(lfs_file_t *file);
    int (*rewind)(lfs_file_t *file);
    int (*truncate)(int size, lfs_file_t *file);
    int (*size)(lfs_file_t *file);
    int (*available)(lfs_file_t *file);

    int (*mkdir)(const char *path);
    lfs_dir_t *(*openDir)(const char *path);
    int (*closeDir)(lfs_dir_t *dir);
    int (*readDir)(lfs_dir_t *dir, struct lfs_info *info);
    int (*rewindDir)(lfs_dir_t *dir);
    int (*tellDir)(lfs_dir_t *dir);
    int (*seekDir)(lfs_dir_t *dir, int offset);


    int (*format)(lfs_t *_lfs, const struct lfs_config *_cfg);
    int (*mount)(lfs_t *_lfs, const struct lfs_config *_cfg);
    int (*unmount)(lfs_t *_lfs);
    int (*remove)(const char *path);
    int (*rename)(const char *oldpath, const char *newpath);
    int (*free)(void);
    int (*tree)(const char *dir_path, char *pbuf, int buf_size);
    
    int (*exist)(const char *path);
} littlefs_t;
 

extern littlefs_t *littlefs;
int littlefs_init(littlefs_t *lfs_obj, struct lfs_config *config,int size);
extern littlefs_t disk;
extern struct lfs_config config;

extern void upload_fs(uint32_t x);

extern void download_fs(uint32_t x);
/*
int                 rmdir(const char *path);
int                 mkdir(const char *path);
DIR*                opendir(const char* path);
int                 readdir(DIR *dirls, struct lfs_info *entry);
int                 closedir(DIR* dir);
void*               fopen(const char *path, const char *modifiers);
void                fclose(void *file);
int                 fflush(void *file);
int                 fgetc(void *file);
char *              fgets(char *s, int n, void *f);
int                 fputc(int c, void *file);
int                 fputs(const char * str, void *file);
int                 fwrite(const void * data, int size, int count, void *file );
int                 fread(void * data, int size, int count, void *file );
int                 fseek(void *file , long offset , int origin );
int                 fgetpos(void *file , uint32 * position);
long                ftell(void *f);
int                 feof(void *f);
int                 remove(const char * filename);
*/

#endif