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