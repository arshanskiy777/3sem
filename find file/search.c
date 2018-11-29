
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void find(char *name, char *path, char *way, struct dirent str, long indec, DIR *ptk, struct stat sb, int depth) {
	
	struct dirent str1;
	DIR ptk1;
	int depth1 = depth - 1;
	char way1[] = way[];
	while ((str != NULL) and (depth >= 0)){
                str = *(readdir(ptk));
                if (str.d_name == name) {
                        indec = indec + 1;
                        way = way + "/" + name;
                        printf("%s\n", way);
                } else {
                        path = path + "/" + str.d_name;
                        stat(path, &sb);
                        if (S_ISDIR(sb.st_mode)){
				way1 = way1 + "/" + str.d_name;
				str1 = *(readdir(ptk1));
                                find(name, path, way1, str1, indec, *ptk1, sb, depth1); //ooooooo
                        }
                }
        }
}






int main(int argc, char *argv[]) { //для текущей директории программа запускается ./program depth ./

	int depth = atoi(argv[1]);
	char name[] = argv[2];
	struct dirent str;
	DIR ptk;
	long indec = 0;
	char way[] = ".";
	struct stat sb;
        char path[] = way[];
	ptk = opendir(name);

	find(name, path, way, str, indec, *ptk, sb, depth);
	if (indec = 0) {
                printf("Can\'t find file with this name: %s", name);
	}



























        /*struct dirent str;
        DIR *ptk;
        ptk = opendir("kkk");
        for (int i=0; i<2; i++){
                str = *(readdir(ptk));
                printf("%s\n", str.d_name);
        }
        return 0;*/
}
