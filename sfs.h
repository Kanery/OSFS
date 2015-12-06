#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dirent_q
{
	int inode_index;
	char type;
	int length;
	//This designates the file's offset from the root
	off_t offset;
	
	char name[256];
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
	struct inode_q *inode_array= malloc(256 * sizeof(struct inode_q));
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
char rettype (dirent_q *root, char* name)
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
struct dirent_q* retdirent (dirent_q *root, off_t offset)
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

