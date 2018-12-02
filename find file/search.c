#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
//#include <conio.h>
#include <stdlib.h>

void find(char *name, char *way, /*struct dirent str,*/ int* indec, /*DIR *ptk, */struct stat sb, int depth) {
	
	struct dirent str1;
	DIR *ptk1;
	int depth1 = depth;
	char way1[] = "", waybuf[] = "", buf[] = "/", path[] = "";
	strcpy(way1, way);
	strcpy(waybuf, way);
	strcpy(path, way);

	ptk1 = opendir(way);
	str1 = *(readdir(ptk1));
	while ((str1.d_name != NULL)&&(depth >= 0)){
                //str1 = *(readdir(ptk));
		//str1 = str;
                if (strcmp(str1.d_name, name) == 0) {
                        *indec = *indec + 1;
                        strcat(waybuf, buf);
			strcat(waybuf, name);
                        puts(waybuf);
			strcpy(waybuf, way1);
                } else {
			strcat(path, buf);
                        strcat(path, str1.d_name);
                        stat(path, &sb);
                        if (S_ISDIR(sb.st_mode)){
				strcat(waybuf, buf);
				strcat(waybuf, str1.d_name);
				//str1 = *(readdir(ptk1));
                                find(name, waybuf, /* str1, */ indec, /*ptk1, */sb, depth1 - 1); //ooooooo
				strcpy(waybuf, way1);
                        }
			strcpy(path, way1);
                }
		str1 = *(readdir(ptk1));
        }
}

int main(int argc, char *argv[]) { //для текущей директории программа запускается

	if ((argc != 4)||(atoi(argv[1]) < 0)){
	printf("Программа запущена неверно. Правильный формат запуска:\n./<имя программы> <глубина сканирования> <имя искомого файла> <путь до директории, в которой следует искать>. \nЕсли глубина задана отрицательным числом, то данноe сообщение также может выводиться. \nДля поиска исключительно в текущей директории используйте вместо имени директории, где следует искать, \'.\'.\nПример: ./search 4 FILE.txt /home/alex/Desktop/find_file/kkk. Обратите внимание! Слеш в конце адреса до директории поиска не нужен!\n");
	exit(-1);
	}

	int depth;
       	depth = atoi(argv[1]) + 1;
	char name[] = "";
	strcpy(name, argv[2]);
	struct dirent str;
	int *indec;
	char way[] = "";
	strcpy(way, argv[3]);	
	strcat(way, "/");
	struct stat sb;
	*indec = 0;
	find(name,  way,/* str,*/ indec, /*ptk, */sb, depth);
	if (indec = 0) {
                printf("Can\'t find file with this name: %s", name);
	}
	return 0;

        /*struct dirent str;
        DIR *ptk;
        ptk = opendir("kkk");
        for (int i=0; i<2; i++){
                str = *(readdir(ptk));
                printf("%s\n", str.d_name);
        }
        return 0;*/
}
