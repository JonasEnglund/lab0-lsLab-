#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* dirpath = "./";
	DIR *path;
	struct dirent *dir;
	char* h;
	int lCheck = 0;
	int buf;

	if(argc == 2) {
		if(buf = getopt(argc, argv, "l") != -1) {
			lCheck = 1;
		}			
		else {
			dirpath = argv[1];
		}
	}

	if(argc == 3) {
		while((buf = getopt(argc, argv, ":l:")) != -1) {
			if(buf == ':') {
				dirpath = argv[1];
			}
			if(buf == 'l') {
				dirpath = optarg;
			}
		}
		lCheck = 1;
	}
	path = opendir(dirpath);
	if(!path) {
		perror("open");
		exit(1);
	}

	void show() {
		dir = readdir(path);
		while(dir != NULL) {
			if(dir->d_name[0] != '.') {
				printf("%s ", dir->d_name);	
			}
			dir = readdir(path);
		}
		printf("\n");
	}	

	struct stat fileInf;

	void showl() {
		char bufPath[300];
		struct passwd *psd;
		struct group *grp;
		while((dir = readdir(path)) != NULL) {
			if(dir->d_name[0] != '.') {
				snprintf(bufPath, 300, "%s%s", dirpath, dir->d_name);
				lstat(bufPath, &fileInf);
				printf((fileInf.st_mode & S_IFDIR)?"d":"-");
				printf((fileInf.st_mode & S_IRUSR)?"r":"-");
				printf((fileInf.st_mode & S_IWUSR)?"w":"-");
				printf((fileInf.st_mode & S_IXUSR)?"x":"-");
				printf((fileInf.st_mode & S_IRGRP)?"r":"-");
				printf((fileInf.st_mode & S_IWGRP)?"w":"-");
				printf((fileInf.st_mode & S_IXGRP)?"x":"-");
				printf((fileInf.st_mode & S_IROTH)?"r":"-");
				printf((fileInf.st_mode & S_IWOTH)?"w":"-");
				printf((fileInf.st_mode & S_IXOTH)?"x":"-");
				printf(" %hu", fileInf.st_nlink);

				psd = getpwuid(fileInf.st_uid);
				if(&(psd->pw_name) == NULL) 
					printf(" %d", fileInf.st_uid);
				else 
					printf(" %s", psd->pw_name);

				grp = getgrgid(fileInf.st_gid);
				printf(" %s", grp->gr_name);
				printf(" %ld", fileInf.st_size);

				printf(" %12.12s", &ctime(&fileInf.st_mtime)[4]);
				printf(" %s", dir->d_name);
				printf("\n");
			}
		}
	}
	if(lCheck) {
		showl();
	}
	else { 
		show();
	}
	closedir(path);
}
