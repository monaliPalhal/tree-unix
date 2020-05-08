#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>

typedef struct data{
	char name[100];
	char dir_name[2000];
}data;

typedef struct node{
	struct data d;
	struct node **children;
	int nchildren;
	int level;
	struct node *parent;
}node;
typedef node *tree;
int isFile(const char* name);
void formtree(node *ptr);
void printtree(node *ptr);
void dirprint(node *ptr);
void hiddenformtree(node *ptr);
void filelimit(node *ptr, int num);
void filetree(char *fc, node *ptr);
int main(int argc,char *argv[]){
	int count;
	node *ptr;
	ptr = (node*)malloc(sizeof(node));
	ptr->level = 0;
	strcpy(ptr->d.dir_name, ".");
	strcpy(ptr->d.name, ".");
	formtree(ptr);
	if(argc == 1) {
		printtree(ptr);
	}
	if(argc > 1) {
		if(!strcmp(argv[1], "-d"))
			dirprint(ptr); 
		else if(! strcmp(argv[1], "-o")) {
			char *fc;
			strcpy(fc, argv[2]);
			filetree(fc, ptr);
		}
		else if(!strcmp(argv[1], "-a")) {
			hiddenformtree(ptr);
			printtree(ptr);
		}
		else if(!strcmp(argv[1], "--filelimit")) {
			count = atoi(argv[2]);
			filelimit(ptr, count);
		}		
		else
			printf("error");
	}	
	return 0;
}
void formtree(node *ptr){
	node *temp;
	int i = 0, size = 150000, count = 0;
	static int k = 1;
	temp = ptr;
	temp -> children = (node **)malloc(size * sizeof(node*));
	DIR *pdir = NULL;
	struct dirent *pent = NULL;
	pdir = opendir(ptr->d.dir_name);
	if(pdir == NULL){
		exit(3);
	}
	while( pent = readdir(pdir)){
		if(i >= size){
			size *= 2;
			temp->children = (node**)realloc(temp->children, size * sizeof(node*));
			
		}
		else{
			if(pent == NULL){
				exit(3);
			}
			if (!strcmp (pent->d_name, ".") || !strcmp (pent->d_name, ".."))
	        			continue;
	        		else{
				temp->children[i] = (node*)malloc(sizeof(node));
				strcpy(temp->children[i]->d.name, pent->d_name);
				temp->children[i]->parent = temp;
			}
		}
		
		i++;
	}
	closedir(pdir);
	temp->nchildren = i;
	k++;
	char str[200], str1[200];
	i = 0; 
	int dir_or_file = 0, lev = 0;
	if(temp->nchildren == 0)
		return;
	while(i < temp -> nchildren){
		strcpy(str1, temp->children[i]->d.name);
		strcpy(str, ptr->d.dir_name);
		strcat(str, "/");
		strcat(str, str1);
		strcpy(temp->children[i]->d.dir_name, str);
		dir_or_file = isFile(str);
		if(!dir_or_file){
			temp->children[i]->level = temp->level + 1;
			formtree(temp->children[i]);
		}	
		if(dir_or_file){
			temp->children[i]->level = temp->level + 1;
			temp->children[i]->children = NULL;
		}
		i++;
	}
	return;
}
void hiddenformtree(node *ptr){
	node *temp;
	int i = 0, size = 150000, count = 0;
	static int k = 1;
	temp = ptr;
	temp -> children = (node **)malloc(size * sizeof(node*));
	DIR *pdir = NULL;
	struct dirent *pent = NULL;
	pdir = opendir(ptr->d.dir_name);
	if(pdir == NULL){
		exit(3);
	}
	while( pent = readdir(pdir)){
		if(i >= size){
			size *= 2;
			temp->children = (node**)realloc(temp->children, size * sizeof(node*));
			
		}
		else{
			if(pent == NULL){
				exit(3);
			}
			if (!strcmp (pent->d_name, ".") || !strcmp (pent->d_name, "..")) {
				temp->children[i] = (node*)malloc(sizeof(node));
				strcpy(temp->children[i]->d.name, pent->d_name);
				temp->children[i]->parent = temp;
			}
	        			
	        		else{
				temp->children[i] = (node*)malloc(sizeof(node));
				strcpy(temp->children[i]->d.name, pent->d_name);
				temp->children[i]->parent = temp;
			}
		}
		
		i++;
	}
	closedir(pdir);
	temp->nchildren = i;
	k++;
	char str[200], str1[200];
	i = 0; 
	int dir_or_file = 0, lev = 0;
	if(temp->nchildren == 0)
		return;
	while(i < temp -> nchildren){
		strcpy(str1, temp->children[i]->d.name);
		strcpy(str, ptr->d.dir_name);
		strcat(str, "/");
		strcat(str, str1);
		strcpy(temp->children[i]->d.dir_name, str);
		dir_or_file = isFile(str);
		if(!dir_or_file){
			temp->children[i]->level = temp->level + 1;
			formtree(temp->children[i]);
		}	
		if(dir_or_file){
			temp->children[i]->level = temp->level + 1;
			temp->children[i]->children = NULL;
		}
		if (!strcmp (str, ".") || !strcmp (str, "..")) {
			temp->children[i]->level = temp->level + 1;
			temp->children[i]->children = NULL;
		}
		i++;
	}
	printf("count%d", count);
	return;
}
void printtree(node *ptr) {
	int i = 0, k = 1;
	static int flag = 0;
	node *temp;
	temp = ptr;
	char string[10], string1[10], string2[10];
	strcpy(string, "|");
	strcpy(string1, "|___");
	strcpy(string2, "___");
	printf("%s", ptr->d.name);
	printf("\n");
	while(i < temp->nchildren){
		if(!isFile(temp->children[i]->d.dir_name)){
			k = 1;
			printf("%s", string);
			
			while(k < temp->children[i]->level){
				printf("   |");
				k++;
			}
			flag = 0;
			if(i == temp->nchildren - 1){
				flag = 1;
			}
			printf("%s", string2);
			printtree(temp->children[i]);
		}
		else{ 
			k = 1;
			printf("%s", string);
			while(k < temp->children[i]->level){
				if(flag == 1)
					printf("    ");
				else
					printf("   |");
				k++;
			}
			if(flag)
				printf("%s", string);
			printf("%s", string2);
			printf("%s", temp->children[i]->d.name);
			printf("\n");
		}	
		i++;
	}
	flag = 0;
	return;
}
void filelimit(node *ptr, int num) {
	int i = 0, k = 1, j = 1;
	static int flag = 0;
	node *temp;
	temp = ptr;
	char string[10], string1[10], string2[10];
	strcpy(string, "|");
	strcpy(string1, "|___");
	strcpy(string2, "___");
	printf("%s", ptr->d.name); 
	printf("\n");
	while(i < temp->nchildren){
		if(!isFile(temp->children[i]->d.dir_name)){
			k = 1;
			printf("%s", string);
			
			while(k < temp->children[i]->level){
				printf("   |");
				k++;
			}
			flag = 0;
			if(i == temp->nchildren - 1){
				flag = 1;
			}
			printf("%s", string2);
			if(temp->children[i]->nchildren <= num) {
				filelimit(temp->children[i], num);
			}
			else
				printf("\n");
		}
		else{ 
			k = 1;
			printf("%s", string);
			while(k < temp->children[i]->level){
				if(flag == 1)
					printf("    ");
				else
					printf("   |");
				k++;
			}
			if(flag)
				printf("%s", string);
			printf("%s", string2);
			printf("%s", temp->children[i]->d.name);
			printf("\n");
		}
		i = i+1;	
	}
	flag = 0;
	return;
}
void filetree(char *fc, node *ptr) {
	int i = 0, k = 1;
	static int flag = 0;
	node *temp;
	temp = ptr;
	char string[10], string1[10], string2[10], str4[10], str5[10];
	FILE *fp;
	fp = fopen("fc", "w");
	strcpy(string, "|");
	strcpy(string1, "|___");
	strcpy(string2, "___");
	strcpy(str4, "   |");
	strcpy(str5, "    ");
	fwrite(ptr->d.name, strlen(ptr->d.name) + 1, 1, fp);
	printf("%s", ptr->d.name);
	fwrite("\n", sizeof("\n"), 1, fp);
	printf("\n");
	while(i < temp->nchildren){
		if(!isFile(temp->children[i]->d.dir_name)){
			k = 1;
			fwrite(string, strlen(string) + 1, 1, fp);
			printf("%s", string);
			
			while(k < temp->children[i]->level){
				printf("   |");
				fwrite(str4, strlen(str4) + 1, 1, fp);
				k++;
			}
			flag = 0;
			if(i == temp->nchildren - 1){
				flag = 1;
			}
			printf("%s", string2);
			fwrite(string2, strlen(string2) + 1, 1, fp);
			filetree(fc, temp->children[i]);
		}
		else{ 
			k = 1;
			printf("%s", string);
			fwrite(string, strlen(string) + 1, 1, fp);
			while(k < temp->children[i]->level){
				if(flag == 1) {
					fwrite(str5, strlen(str5) + 1, 1, fp);
					printf("    ");
				}
				else {
					printf("   |");
					fwrite(str4, strlen(str4) + 1, 1, fp);
				}
				k++;
			}
			if(flag) {
				fwrite(string, strlen(string) + 1, 1, fp);
				printf("%s", string);
			}
			printf("%s", string2);
			fwrite(string2, strlen(string2) + 1, 1, fp);
			printf("%s", temp->children[i]->d.name);
			fwrite(temp->children[i]->d.name, strlen(temp->children[i]->d.name) + 1, 1, fp);
			printf("\n");
			fwrite("\n", sizeof("\n"), 1, fp);
		}	
		i++;
	}
	flag = 0;
	fclose(fp);
	return;
}
void dirprint(node *ptr) {
	int i = 0, k = 1;
	static int flag = 0;
	node *temp;
	temp = ptr;
	char string[10], string1[10], string2[10];
	strcpy(string, "|");
	strcpy(string1, "|___");
	strcpy(string2, "___");
	printf("%s", ptr->d.name);
	printf("\n");
	while(i < temp->nchildren){
		if(!isFile(temp->children[i]->d.dir_name)){
			k = 1;
			printf("%s", string);
			
			while(k < temp->children[i]->level){
				printf("   |");
				k++;
			}
			flag = 0;
			if(i == temp->nchildren - 1){
				flag = 1;
			}
			printf("%s", string2);
			dirprint(temp->children[i]);
		}
		i++;
	}
	flag = 0;
	return;
}	
int isFile(const char* name){
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }

    if(errno == ENOTDIR)
    {
     return 1;
    }

    return -1;
}