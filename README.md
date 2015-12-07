# OSFS
Current list of tasks:
1. Seek Help
2. Seek More Help
3. Pray

==
1. int sfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
	
	Offsets from path... Where does readdir work with supplied path.?
	
char * travel(char *path){
	
	Considering the following:
	
	Traversing through multiple directories or nested directories in system
	will naturally demand that we are searching through dirents (linked list or
	other structs otherwise).

	Is this done by getindex, type, offset?
	
}

//This function searches through a list of dirents and returns the one with a matching path name

struct dirent_q retdirent_p (dirent_q root, char* name){
	struct dirent_q* temp=root;
	
	while(temp!=NULL){
	
	if(strcmp(temp->name, name) == 0)
		return temp;
		
	temp=temp->next;
	}
	
	return NULL;
}

* rmdir pseudocode
* 1) call retdirent_p
* 2)  if (dirent_q is a file) reject.
* 3) if (dirent_q is null) reject
* 4) if (dirent_q is nonempty) reject
* 5)  else delete from dirent_q linked list
* 6) free dirent_q