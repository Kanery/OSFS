/*
  Simple File System

  This code is derived from function prototypes found /usr/include/fuse/fuse.h
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  His code is licensed under the LGPLv2.

*/

#include "params.h"
#include "block.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_SYS_XATTR_H
#include <sys/xattr.h>
#endif

#include <pthread.h>
#include "log.h"
#include "sfs.h"



///////////////////////////////////////////////////////////
//
// Prototypes for all these functions, and the C-style comments,
// come indirectly from /usr/include/fuse.h
//


//This will initialize the superblock and also the root directory.
inode_q inode_array[];
dirent_q *root;


static void theFullpath(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, SFS_DATA->rootdir);
    strncat(fpath, path, PATH_MAX);
}



/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
void *sfs_init(struct fuse_conn_info *conn)
{
    fprintf(stderr, "in bb-init\n");
    log_msg("\nsfs_init()\n");
    
    log_conn(conn);
    log_fuse_context(fuse_get_context());
	
	//This sets our global variables up to default valuses, and also creates the root directory.
	inode_array=initialize();
	root=create_dirent(0,'d',0, 0,"/root");
	inode_array[0].data=NULL;
	inode_array[0].type='d';
	
	int i = 0;
	for (; i < 1000000; i++)
	{
		inode_array[0].offset = &(SFS_DATA->diskfile) + 512*i;
	}
	
	
    return SFS_DATA;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
void sfs_destroy(void *userdata)
{
    log_msg("\nsfs_destroy(userdata=0x%08x)\n", userdata);
    
    struct dirent_q* temp=root;
    if(temp->next==NULL)
    {
    	free(temp);
    	goto NEXT;
    }
    struct dirent_q *temp2=root->next;
    if(temp2->next==NULL)
    {
    	free(temp);
    	free(temp2);
    	goto NEXT;
    }
    while(temp2!=NULL)
    {
    	free(temp);
    	temp=temp2;
    	temp2=temp2->next;
    }
    if(temp!=NULL)
    {
    	free(temp);
    }
    
    
    
    
    
    NEXT:
    free(inode_array);
    return;
    
}

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int sfs_getattr(const char *path, struct stat *statbuf)
{
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nsfs_getattr(path=\"%s\", statbuf=0x%08x)\n",
	  path, statbuf);
	  
	  
	  
	  
	int inum=retindex(root, path);
	if(inum<0)
	{
		goto END;
	}
	char type=rettype(root, path);
	struct dirent_q* temp=finddir(path, root);
	
	if(type=='d')
	{
    
    //This assigns all parameters in the stat struct to their specific values.
    	       statbuf->st_dev=300;         /* ID of device containing file */
               statbuf->st_ino=inum;         /* inode number */
               statbuf->st_mode=temp->mode;        /* protection, or if dir or file */
               statbuf->st_nlink=0;       /* number of hard links */
               statbuf->st_uid=0;         /* user ID of owner */
               statbuf->st_gid=0;         /* group ID of owner */
               statbuf->st_rdev=0;        /* device ID (if special file) */
               statbuf->st_size=512;        /* total size, in bytes */
               statbuf->st_blksize=512;     /* blocksize for filesystem I/O */
               statbuf->st_blocks=64;
          }
          else
          {  
          
           //This assigns all parameters in the stat struct to their specific values.
    	       statbuf->st_dev=300;         /* ID of device containing file */
               statbuf->st_ino=inum;         /* inode number */
               statbuf->st_mode=temp->mode;        /* protection, or if dir or file */
               statbuf->st_nlink=0;       /* number of hard links */
               statbuf->st_uid=0;         /* user ID of owner */
               statbuf->st_gid=0;         /* group ID of owner */
               statbuf->st_rdev=0;        /* device ID (if special file) */
               statbuf->st_size=512;        /* total size, in bytes */
               statbuf->st_blksize=512;     /* blocksize for filesystem I/O */
               statbuf->st_blocks=64;
           } 
    
  
    //Returns a log of the newly created stat.
    log_stat(statbuf);
    
    END:

    return retstat;
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 *
 * Introduced in version 2.5
 */
int sfs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_create(path=\"%s\", mode=0%03o, fi=0x%08x)\n",
	    path, mode, fi);
	    //If the path is invalid...
	if(retindex(path)!=-1)
	{
		goto FINAL;
	}
	
	//Finds the next free inode index
	int place=capture_spot(inode_array);
	
	struct dirent_q *new=create_dirent(place, 'f', 512, offset, path):
	
	
	//If there is no space left on the array...
	if(!place){
		retstat=1;
		goto FINAL;
	}
	
	struct dirent_q *temp=root;
	
	//Places new dirent into the system
	while(temp!=NULL)
	{
		if(temp->next==NULL){
			temp->next=new;
			goto PREP;
		}
		temp=temp->next; 
	 }
	 
	 PREP:
	 
	 
	 struct dirent_q* temp2=lobotomy(path);
	 struct child_ref *temp3=inode_array[temp->inode_array]->children;
	 
	 if(temp3==NULL)
	 {
	 	temp3->point=new;
	 }
	 
	 else{
		 while(temp3->next!=NULL)
		 {
		 	if(strcmp(temp3->point->name, path)==0)
		 	{
		 		goto FINAL;
		 	}
		 	temp3=temp3->next;
		 }
		 temp3->point=new;
	}
	 
	 
	
	
    
    
     inode_array[place]->data=NULL;
	inode_array[place]->type='f';
	
    retstat=1;
    FINAL:
    return retstat;
}

/** Remove a file */
int sfs_unlink(const char *path)
{
    int retstat = 0;
    log_msg("sfs_unlink(path=\"%s\")\n", path);
    
	//The dirent and Inode we need to unlink
	struct dirent_q *unlinkDir = finddirent(path, root);
	struct inode_q *unlinkIn = inode_array[unlinkDir->inode_index];
	
	//If there is something inside it, do not delete
	if(*unlinkIn.children != NULL)
	{
		return retstat;
	}
	
	//Delete Inode
	inode_array[unlinkDir->inode_index] = NULL;
	
	//Find previous dirent to point it to next dirent (removing unlinkDir from the linked list)
	struct dirent_q *root_node = root;
	struct dirent_q * prev_node = root_node
	
	while(prev_node->next != unlinkDir)
	{
		prev_node = prev_node->next;
	}
	
	prev_node->next = unlinkDir->next;
	
	//Delete unlinkDir
	free(unlinkDir);
    	retstat=1;
    
  
    return retstat;
}

/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 *
 * Changed in version 2.2
 */
int sfs_open(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_open(path\"%s\", fi=0x%08x)\n",
	    path, fi);
	    
	    
	    
	struct dirent_q openDir = finddirent(path, root);
	if(openDir == NULL)
	{
		int success = sfs_create(path, 0755, fi);
		if(success == 0)
		{
			return retstat;
		}
	}
	struct dirent_q openDir = finddirent(path, root);
	if(openDir == NULL)
	{
		return retstat;
	}
		
	openDir->open_flag_count++;
	
	//You now have the dirent for the (possibly newly created) dirent. Open it!
	
    	retstat = 1;   

    
    return retstat;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int sfs_release(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_release(path=\"%s\", fi=0x%08x)\n",
	  path, fi);
	  
	struct dirent_q *relDir = finddir(path, root);
	if(relDir == NULL || relDir->open_flag_count = 0)
	{
		return retstat;
	}
	
	//release code here
	relDir->open_flag_count--;
	
	retstat = 1;
    	return retstat;  
	  

}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
int sfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_read(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n",
	    path, buf, size, offset, fi);

   
    return retstat;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
int sfs_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_write(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n",
	    path, buf, size, offset, fi);
    
    
    return retstat;
}


/** Create a directory */
int sfs_mkdir(const char *path, mode_t mode)
{
    int retstat = 0;
    log_msg("\nsfs_mkdir(path=\"%s\", mode=0%3o)\n",
	    path, mode);
	    
	
	    int retstat = 0;
    log_msg("\nsfs_create(path=\"%s\", mode=0%03o, fi=0x%08x)\n",
	    path, mode, fi);
	    //If the path is invalid...
	if(retindex(path)!=-1)
	{
		goto FINAL;
	}
	
	//Finds the next free inode index
	int place=capture_spot(inode_array);
	
	struct dirent_q *new=create_dirent(place, 'd', 512, offset, path):
	
	
	//If there is no space left on the array...
	if(!place){
		retstat=1;
		goto FINAL;
	}
	
	struct dirent_q *temp=root;
	
	//Places new dirent into the system
	while(temp!=NULL)
	{
		if(temp->next==NULL){
			temp->next=new;
			goto PREP;
		}
		temp=temp->next; 
	 }
	 
	 PREP:
	 
	 
	 struct dirent_q* temp2=lobotomy(path);
	 struct child_ref *temp3=inode_array[temp->inode_array]->children;
	 
	 if(temp3==NULL)
	 {
	 	temp3->point=new;
	 }
	 
	 else{
		 while(temp3->next!=NULL)
		 {
		 	if(strcmp(temp3->point->name, path)==0)
		 	{
		 		goto FINAL;
		 	}
		 	temp3=temp3->next;
		 }
		 temp3->point=new;
	}
	 
	 
	
	
    
    
     inode_array[place]->data=NULL;
	inode_array[place]->type='d';
	
    retstat=1;
    FINAL:
    return retstat;    
	    
	    
	    
	    
   
    
    return retstat;
}


/** Remove a directory */
int sfs_rmdir(const char *path)
{
    int retstat = 0;
    log_msg("sfs_rmdir(path=\"%s\")\n",
	    path);
    
    
    return retstat;
}


/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int sfs_opendir(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nsfs_opendir(path=\"%s\", fi=0x%08x)\n",
	  path, fi);
    
    
    return retstat;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
 
 
 //int fuse_filler_dir_t(void *buf, const char *name, const struct [Stat] *stbuf, off_t off)
 
 
 
 
int sfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
    int retstat = 0;
    
    //We first find a drient with an offset equal to the demanded offset.  If none exists, we return zero.
    struct dirent_q *temp=retdirent(root, offset);
    
    struct child_ref *children=temp->children;
    
    REPEAT:
    
    //If temp is not a directory, and is a file, return failure.
     if(temp->type=='f')
    	return -ENOENT;
    //If temp is NULL, or there is not dirent at offset "offset", return failure.
    if(!temp)
    	return -ENOENT;
    
    //Please look at section 5.2 of the assignment for an explantion of this function.	
    int verdict=filler(buf, path, NULL, offset);
    
    //If filler returns zero or the dirent in question has no more children in the parent directory, return zero.
    if(verdict==0 || children==NULL)
    {
    	retstat=0;
    	goto END;
    }
    
    //The following goes down the list of files the selected directory has inside of it, or the "children."
    //All definitions are in "sfs.h".
    temp=children->point;
    offset=temp->offset;
    children=children->next;
    goto REPEAT;
    
   
    
    END:
    return retstat;
}

/** Release directory
 *
 * Introduced in version 2.3
 */
int sfs_releasedir(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;

    
    return retstat;
}

struct fuse_operations sfs_oper = {
  .init = sfs_init,
  .destroy = sfs_destroy,

  .getattr = sfs_getattr,
  .create = sfs_create,
  .unlink = sfs_unlink,
  .open = sfs_open,
  .release = sfs_release,
  .read = sfs_read,
  .write = sfs_write,

  .rmdir = sfs_rmdir,
  .mkdir = sfs_mkdir,

  .opendir = sfs_opendir,
  .readdir = sfs_readdir,
  .releasedir = sfs_releasedir
};

void sfs_usage()
{
    fprintf(stderr, "usage:  sfs [FUSE and mount options] diskFile mountPoint\n");
    abort();
}

int main(int argc, char *argv[])
{
    int fuse_stat;
    struct sfs_state *sfs_data;
    
    // sanity checking on the command line
    if ((argc < 3) || (argv[argc-2][0] == '-') || (argv[argc-1][0] == '-'))
	sfs_usage();

    sfs_data = malloc(sizeof(struct sfs_state));
    if (sfs_data == NULL) {
	perror("main calloc");
	abort();
    }

    // Pull the diskfile and save it in internal data
    sfs_data->diskfile = argv[argc-2];
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;
    
    sfs_data->logfile = log_open();
    
    // turn over control to fuse
    fprintf(stderr, "about to call fuse_main, %s \n", sfs_data->diskfile);
    fuse_stat = fuse_main(argc, argv, &sfs_oper, sfs_data);
    fprintf(stderr, "fuse_main returned %d\n", fuse_stat);
    
    return fuse_stat;
}
