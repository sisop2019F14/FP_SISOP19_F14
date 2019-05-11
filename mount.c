
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#define FUSE_USE_VERSION 28
#include <fuse.h>

static const char *dirpath = "/home/anwar/fp"; 

static int xmp_getattr(const char *path, struct stat *stbuf);
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi);
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi);
int copyfile(char source_file[],char target_file[]);


static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

void findsong(char path[900]){

    DIR *dp;
	struct dirent *de;
    int j,i;
    char file[100];

	dp = opendir(path);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            
            char ext[4];
            memset(ext,0,sizeof(ext));
            strcpy(file,de->d_name);
                    
            for(i=strlen(file)-3,j=0; i<=strlen(file); j++ ,i++){
              ext[j]=file[i];
            }
            
            struct stat st;
            char fpath[999];
            char target[999];
            memset(&st, 0, sizeof(st));
            memset(fpath,0,sizeof(fpath));
            memset(target,0,sizeof(target));
            strcpy(fpath,path);
            strcat(fpath,de->d_name);
            
            strcpy(target,"/home/anwar/fp/");
            strcat(target,de->d_name);

            //printf("path : %s\n",path);
            //printf("d_name : %s\n",de->d_name);
            //printf("fpath : %s\n",fpath);
            //printf("target : %s\n",target);
            //printf("ext : %s\n",ext);
            
            if(strcmp(ext,"mp3") == 0){
                copyfile(fpath,target);
            }

            if(lstat(fpath,&st) < 0){
                printf("Error Stating \n\n\n\n");
            }

            strcat(fpath,"/");
            //printf("fpath with / : %s\n",fpath);

            if(strcmp(fpath,"/home/anwar/fp/") != 0){
                findsong(fpath);
            }
            
        }    
    }

    closedir(dp);
}


int main(int argc, char *argv[])
{
    //pid_t child_id;
    //char *arg[4] = {"mv","**/*.mp3","/home/anwar/fp/",NULL};

    //child_id = fork();

    //if(child_id == 0){
       // execvp("mv", arg);
    //}

    //system("mv **/*.mp3 /home/anwar/fp");
    findsong("/home/anwar/");

    int a;
    while(wait(a) > 0);

	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

     
int copyfile(char source_file[],char target_file[])
{
    char ch;
    FILE *source, *target;

    source = fopen(source_file, "r");
    
    if (source == NULL)
    {
        printf("Press any key to exit...\n");
        exit(EXIT_FAILURE);
    }
    
    target = fopen(target_file, "w");
    
    if (target == NULL)
    {
        fclose(source);
        printf("Press any key to exit...\n");
        exit(EXIT_FAILURE);
    }
    
    while ((ch = fgetc(source)) != EOF)
        fputc(ch, target);
    
    //printf("File copied successfully.\n");
    
    fclose(source);
    fclose(target);
    remove(source_file);
    
    return 0;
}


static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}