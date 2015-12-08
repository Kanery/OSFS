#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct dirent_q
{
	int inode_index;
	char type;
	int length;
	//This designates the file's offset from the root
	off_t offset;
	mode_t mode;
	char name[256];
	pthread_mutex_t mutex;
	
	int refcount;
	int open_flag_count;
	
	struct dirent_q *next;
};

//This will construct a list of references to dirents thaat are in an inode;
struct child_ref
{

	struct dirent_q* point;
	struct child_ref *next;

};

struct inode_q
{

	//This points to the corresponding head of the data in the superblock.
	char* data;
	off_t offset;
	//Just another reference to the inode's/file's type.
	char type;
	//This is an array of references to child dirent pointers.
	struct child_ref *children;
	
};

struct dirent_q *create_dirent(int index, char type, int length, off_t offset, char *name)
{
	/*
	Search dirent_q linked list to find the end and set the final dirent_q node to point to this newly created node.
	*/
	struct dirent_q *seed=(struct dirent_q*) malloc(sizeof(struct dirent_q));
	
	
	seed->inode_index=index;
	seed->type=type;
	strcpy(seed->name, name);
	seed->offset=offset;
	seed->next=NULL;
	
	return seed;

}

struct inode_q *initialize()
{

	//This is our array of inodes, shartened down to 256 for testing.
	struct inode_q *inode_array= malloc(1000000 * sizeof(struct inode_q));
	return inode_array;
	
}

//This function searches through a list of dirents and returns the one with a matching name to the name requested.
int retindex (dirent_q *root, char* name)
{
	struct dirent_q* temp=root;
	while(temp!=NULL)
	{
		if(strcmp(temp->name, name)==0)
		{
			return temp->inode_index;
		}
	temp=temp->next;
	}
	return -1;
}

//This function searches through a list of dirents and returns the one with a matching type to the type requested.
char rettype (struct dirent_q *root, char* name)
{
	struct dirent_q* temp=root;
	while(temp!=NULL)
	{
		if(strcmp(temp->name, name)==0)
		{
			return temp->type;
		}
	temp=temp->next;
	}
	return -1;
}

//This function searches through a list of dirents and returns the one with a matching offset to the offset requested.
struct dirent_q* retdirent (struct dirent_q *root, off_t offset)
{
	struct dirent_q* temp=root;
	while(temp!=NULL)
	{
		if(temp->offset==offset)
		{
			return temp;
		}
	temp=temp->next;
	}
	return NULL;
}

struct dirent_q* finddirent(char *path, struct dirent_q* root)
{
	struct dirent_q *temp=root;
	while(temp!=NULL)
	{
		if(strcmp(path, temp->name)==0)
		{
			return temp;
		}
		temp=temp->next;
	}
	return NULL;

}
	

int capture_spot(struct inode_q array[])
{
	int count=0;
	int length=(sizeof(array)/sizeof(inode_q));
	
	while(count<length)
	{
		if(array[count]!=NULL)
		{
			return count;
		}
		count++;
	}
	return -1;
}

struct dirent_q * lobotomy(char * path){
	
	for(int i = (strlen(path)-1); int i >= 0; i--)
	{
		if(path[i] == "\")
		{
			char temp[strlen(path)];
			strcpy(temp, path)
			temp[i] = "\0";
			return followPath(temp);
		}
	}
	return NULL;
}


