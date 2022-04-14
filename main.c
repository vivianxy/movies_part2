#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define PREFIX "movies_"
void onidDic();

struct movie
{
    char *Title;
    int Year;
    char* Language[5];
    double Rating_value;
    struct movie *next;
};

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->Title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->Title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->Year = atoi(token);

    // The next token is the Language
    //remove the title and year part
    token = strtok_r(NULL, ",", &saveptr);
    char*saveptr_2;
    //remove the [] and ;, and append each token to the language array.
    token = strtok_r(token, "[", &saveptr_2);
    token = strtok_r(token, "]", &saveptr_2);
    char*current_language=strtok(token,";");
    
    //the movie has up to 5 languages
    int i;
    for(i=0;i<=5;++i)
    {
        //currMovie->Language[i]=current_language;
        //if the pointer is NULL, it will break the loop
        if (current_language==NULL)
        {
            currMovie->Language[i]=NULL;
            break;
        }
        else{
            currMovie->Language[i]=(char*) malloc(21);
            memset(currMovie->Language[i],'\0',21);
            strcpy(currMovie->Language[i],current_language);
        }
        current_language=strtok(NULL,";");
    }
   
    // The last token is the rating value
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->Rating_value = atof(token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    int i=0;
    int movie_num=0;
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {   //jump the firt line
        if(i==0)
        {
            i+=1;
            continue;
        }
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        movie_num++;
    }
    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, movie_num);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* amovie){
    printf("%s, %d", amovie->Title, amovie->Year);
    int i;
    for(i=0;i<5;++i)
    {
        char*lan=amovie->Language[i];
        if (lan!=NULL)
        {
            printf(" %s,", lan);
        }
        else
        {
            break;
        }
    }
    printf("%.1f\n",amovie->Rating_value);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}
int intro_1()
{
    int user_input1;
    printf("\n");
    printf("1.Select file to process\n");
    printf("2.Exit the program\n");
    printf("\n");
    printf("Enter choice 1 or 2: ");
    scanf("%i", &user_input1);
    printf("\n");
    return user_input1;
}
int intro_2()
{
    int user_input2;
    printf("Which file you want to process?\n");
    printf("1.Enter 1 to pick the largest file\n");
    printf("2.Enter 2 to pick the smallest file\n");
    printf("3.Enter 3 to specify the name of a file\n");
    printf("\n");
    printf("Enterc a choice from 1 to 3: ");
    scanf("%i", &user_input2);
    printf("\n");
    return user_input2;
}

char* findlargest()
{
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;
    int i = 0;
    char* entryName=malloc(sizeof(char)*256);
    int l;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL)
    {

        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0)
        {
            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
    
            // Use the difftime function to get the time difference between the current value of lastModifTime and the st_mtime value of the directory entry
            if(i == 0 || dirStat.st_size > l)
            {
                l= dirStat.st_size;
                memset(entryName, '\0', sizeof(char)*256);
                strcpy(entryName, aDir->d_name);
            }
            i++;
        }
    }
    // Close the directory
    closedir(currDir);
    printf("The largest file is %s\n", entryName);
    struct movie* a=processFile(entryName);
    onidDic(a);
	return entryName;
}

char* findsmallest()
{
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;
    int i = 0;
    char* entryName=malloc(sizeof(char)*256);
    int l;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL)
    {

        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0)
        {
            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat);  
    
            // Use the difftime function to get the time difference between the current value of lastModifTime and the st_mtime value of the directory entry
            if(i == 0 || dirStat.st_size < l)
            {
                l= dirStat.st_size;
                memset(entryName, '\0', sizeof(char)*256);
                strcpy(entryName, aDir->d_name);
            }
            i++;
        }
    }
    // Close the directory
    closedir(currDir);
    printf("The smallest file is %s\n", entryName);
    //parse entryName return movie*
    struct movie* a=processFile(entryName);
    onidDic(a);
	return entryName;
}

void submenu();
char* findbyname(int*input)
{
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;
    int i = 0;
    char* entryName=malloc(sizeof(char)*256);
    int file_descriptor;
	char* newFilePath=malloc(sizeof(char)*256);
    printf("Enter the file name: ");
    scanf("%s",newFilePath);
    file_descriptor = open(newFilePath, S_IRUSR | S_IWUSR);
	if (file_descriptor == -1){
		printf("Cannot open \"%s\", try again\n", newFilePath);
        *input=file_descriptor;
        printf("\n");
        /*int choice2;
        choice2=intro_2();
        if (choice2==1)
            {
                findlargest();
            }
            else if (choice2==2)//if the input is 2, find smallest file
            {
                findsmallest();
            }
            else if (choice2==3)//if the input is 3, find file by name
            {
                findbyname(&choice2);
            }
            else if (choice2<1||choice2>3)
            {
                printf("Wrong input, try again!\n");
            }*/
        submenu();
	}
    else
    {
        printf("You are processing: %s\n", newFilePath);
        struct movie* a=processFile(newFilePath);
        onidDic(a);
    }
	return newFilePath;
}

void onidDic(struct movie *curr)
{
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;
    int i = 0;
    char* entryName=malloc(sizeof(char)*256);
    int file_descriptor;
	char* newDicPath=malloc(sizeof(char)*256);
    char *dir = malloc(sizeof(char)*30);
    int rand_num=random()%10000;
    sprintf(dir,"songx2.movies.%d",rand_num);
    mkdir(dir, 0750);
    printf("You are processing: %s\n", dir);

    //while looping through a list of movies
    while(curr!=NULL){
        //newDicpath = dir path + /year of the move.txt
        sprintf(newDicPath,"%s/%d.txt", dir, curr->Year);
        //printf("Looking at path: %s:\n", newDicPath);
        int new_descriptor;
        new_descriptor=open(newDicPath, O_WRONLY | O_CREAT | O_APPEND, 0640);
        char*cmp=malloc(sizeof(char)*256);
        memset(cmp,'\0',256);
        sprintf(cmp,"%s\n", curr->Title);
        //movie title,length of the movie title string
        write(new_descriptor,cmp, strlen(cmp));
        curr=curr->next;
    }
    
    //file_descriptor = open(newDicPath, O_WRONLY | O_CREAT | O_APPEND, 0640);
    //write to the file path with the movie name
    
}
void submenu(){
    int choice2;
    choice2=intro_2();
    if (choice2==1)
    {
        findlargest();
    }
    else if (choice2==2)//if the input is 2, find smallest file
    {
        findsmallest();
    }
    else if (choice2==3)//if the input is 3, find file by name
    {
        findbyname(&choice2);
    }
    else if (choice2<1||choice2>3)
    {
        printf("Wrong input, try again!\n");
        printf("\n");
        submenu();

    }

}


int main(){
    srandom(time(NULL));
    int choice1;
    while(choice1!=2)//if input is not 2, go to next choose section
    {   choice1=intro_1();
        if (choice1==1)//if the input is 1, find largest file
        {
            /*int choice2;
            choice2=intro_2();
            if (choice2==1)
            {
                findlargest();
            }
            else if (choice2==2)//if the input is 2, find smallest file
            {
                findsmallest();
            }
            else if (choice2==3)//if the input is 3, find file by name
            {
                findbyname(&choice2);
            }
            else if (choice2<1||choice2>3)
            {
                printf("Wrong input, try again!\n");

            }*/
            submenu();

        }
        else if (choice1<1||choice1>2)
        {
            printf("wrong input, try again!\n");
        }
    }

    return EXIT_SUCCESS;//if input is 2, exit
}