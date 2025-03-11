#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define differentiator ';'
#define semicolon ";"
#define Trait '$'
#define equal '='
#define quotation_mark '"'
#define MAX_COMMAND_LENGTH 510
#define MAX_NUMBER_ARGUMENTS 10
#define enter '\n'
#define space ' '
#define notFoundIndex -1
#define keyPassWord 3
#define TRUE 1
#define verticalBar '|'
#define REDIRECT_OPERATOR '>'
#define MAX_READ 1024


int* savingPID;
int sizeSavingPID;
int signTypeComand;
int bgcomand;
int filecomand;
int pipSigTSTP;
void minishell();
void prompt(int cmd,int args);
void numComands(char input[],int* num_cmd);
char** arrayInitialization(char input[] , int num_cmd,char* sign);
char** fixComand(char* argument,char** variables,int* maxVariabl,int* countVariabl,int* lengthOfArrayComand,int* countSpaceInQM);
char* deletQM(char* word,int size,int* countSpaceInQM);
void saveVariables(char* argument,char** variables,int* maxVariabl,int* countVariabl);
char** spaceSplitArray(char* argument,int* num);
int numOfSplitArray(char* argument);
void indexOfStart_End(char* argument,int* numStart,int* numEnd);
int findVariables(char** variablse,char* argument,int countVariable);
char* changeArray(char* argument,int indexValu,char** variables,int* maxVariabl,int* countVariabl);
int cheakPassWord(char* input,int cmd,int args);
int numOfPIPE(char* array,int size,char sign);
char ** redirect_operatorsplitaArray(char* array,int pipeNumber);
char** fixTypeOfComand(char* array,int size,int* pipeNumber,int *signTypeComand);
char* intefrationTowString(char* comand,char* sign);
char* fixPugComand(char* comand,char* sign);
int countNumPID;

void prompt(int cmd,int args) // print to the screen the path
{
    char buf[1024];
    if(getcwd(buf, 1024) == NULL)
        exit(1);
    printf("#cmd:<%d>|#args:<%d>@%s> ",cmd,args,buf);
}

void numComands(char input[],int* num_cmd) // save the number of comand by how many 
{                                             // ; we have 
    *num_cmd=1;
    int countQM=0;
    int size=strlen(input)+1;
    for(int index=0;index<size;index++)
    {
        if(input[index] == quotation_mark)
        {
            countQM++;
        }
        if(input[index] == differentiator && countQM %2 ==0)
        {
            *num_cmd=*num_cmd+1;
        }
    }
}

char* intefrationTowString(char* comand,char* sign)
{
    int size=strlen(comand)+1;
    int num=size;
    char firstComand[size];
    for(int i=0;i<size;i++)
        firstComand[i]=comand[i];
    firstComand[size -1]='\0';
    comand=strtok(NULL,sign);
    size=strlen(comand)+1;
    char* array=(char*) malloc((size+num)*sizeof(char));
    for(int i=0,j=0;i<size+num;i++)
    {
        if(i > num-1)
        {
            array[i]=comand[j];
            j++;
        }
        else if(i == num-1)
            array[i]=sign[0];
        else
            array[i]=firstComand[i];
    }
    return array;
}

char* fixPugComand(char* comand,char* sign)
{
    int size=strlen(comand)+1;
    int countOfQM=0;
    int num=size;
    for(int i=0;i<size;i++)
        if(comand[i] == quotation_mark)
            countOfQM++;
    if(countOfQM ==1)
    {
        return intefrationTowString(comand,sign);
    }
    char* array=(char*) malloc(size *sizeof(char));
    strcpy(array,comand);
    return array;
}

char** arrayInitialization(char input[] , int num_cmd,char* sign)// Initialization array according
{                                                   // how many comand we have 
    char** array=(char**) malloc((num_cmd)*sizeof(char*));
    char *token =strtok(input, sign);
    for(int index =0;index<num_cmd && token!= NULL;index++)
    {
        array[index]=fixPugComand(token,sign);
        token=strtok(NULL,sign);
    }
    return array;
}

int numOfSplitArray(char* argument)
{
    int count=0;
    char* split=strtok(argument," ");
    while (split != NULL)
    {
        count++;
        split=strtok(NULL," ");
    }
    return count;
}

char** spaceSplitArray(char* argument,int* num)
{
    int size=strlen(argument)+1;
    char* saveArgument=(char*)malloc(size*sizeof(char));
    for(int i=0;i<size;i++)
        saveArgument[i]=argument[i];
    saveArgument[size-1]='\0';
    *num=numOfSplitArray(saveArgument);
    char** comand=(char**)malloc((*num+1 )* sizeof(char*));
    char* split=strtok(argument," ");
    int index;
    for( index=0;index<*num && split !=NULL;index++)
    {
        int size1=strlen(split);
        comand[index]=(char*)malloc((size1+1)*sizeof(char));
        strcpy(comand[index],split);
        split=strtok(NULL," ");
    }
    free(argument);
    free(saveArgument);   
    comand[index]=NULL;
    return comand;
}

char* deletQM(char* word,int size,int* countSpaceInQM) //return new string without ""
{
    int qm=0;
    for(int i=0,j=0;i<size;i++)
    {
        if(word[i] == quotation_mark)
        {
            qm++;
            continue;
        }
        if(qm %2 != 0 && word[i] == space)
            *countSpaceInQM=*countSpaceInQM+1;

        word[j]=word[i];
        j++;
    }
    word[size-1]='\0';
    return word;
}

void saveVariables(char* argument,char** variables,int* maxVariabl,int* countVariabl)
{
    if(*maxVariabl ==*countVariabl)
    {
        variables=(char**)realloc(variables,(*maxVariabl*2)*sizeof(char*));
        *maxVariabl=*maxVariabl*2;
    }
    variables[*countVariabl]=argument;
    *countVariabl=*countVariabl+1;
}

void indexOfStart_End(char* argument,int* numStart,int* numEnd)
{
    int size=strlen(argument)+1;
    int indexOfNameValu;
    for(indexOfNameValu=0;indexOfNameValu<size;indexOfNameValu++)
    {
        if(argument[indexOfNameValu] == Trait)
            *numStart=indexOfNameValu+1;
        if(argument[indexOfNameValu] == quotation_mark)
        {
            *numEnd=indexOfNameValu;   
            return;
        }
    }
    *numEnd=size-1;
}

int findVariables(char** variablse,char* argument,int countVariable)
{
    int numStart=0;
    int numEnd=0; 
    int count;
    indexOfStart_End(argument,&numStart,&numEnd);
    int i ,j;
    for(i=0;i<countVariable;i++)
    {
        int num=strlen(variablse[i]);
        int h;
        count=0;
        for( j=0,h=numStart;j<num && h<numEnd;j++,h++)
        {
            if(variablse[i][j] ==space)
                continue;
            if(variablse[i][j] == equal)
                break;
            if(variablse[i][j] != argument[h])
                break;
            if(argument[h] == quotation_mark)
                break;
            if(variablse[i][j] == argument[h])
                count++;
        }
        if(numEnd-numStart == count && variablse[i][j] == equal)
        {
            return i;
        }
    }     
    return notFoundIndex;
}

char* changeArray(char* argument,int indexValu,char** variables,int* maxVariabl,int* countVariabl)
{
    int numStart;
    int numEnd;
    indexOfStart_End(argument,&numStart,&numEnd);
    int size=strlen(variables[indexValu])+1;
    int i;
    int count=0;
    for( i=0;i<size;i++)
    {
        if(variables[indexValu][i] == equal)
            count=i;
        if(variables[indexValu][i] == space)
            break;
    }   
    int temp=count;
    count=i;
    i=temp;
    int sizeArgu=strlen(argument);
    int num=(size-i)+sizeArgu-i-2;//-1 for $ and -1 for '\0' of variable 
    char* newArgument=(char*)malloc((num+1) * sizeof(char));
    int j;
    for(j=0;j<numStart-1;j++)
        newArgument[j]=argument[j];
    for(i=i+1;j<num && i<count;j++,i++)
        newArgument[j]=variables[indexValu][i];
    for(int h=0;j < num && h < sizeArgu;j++,h++)
        newArgument[j]=argument[numEnd+h];
    newArgument[j]='\0'; 
    int countSpaceInQM=0;
    free(argument);
    newArgument=deletQM(newArgument,num,&countSpaceInQM);
    return newArgument;
}

char** fixComand(char* argument,char** variables,int* maxVariabl,int* countVariabl,int* lengthOfArrayComand,int* countSpaceInQM) 
{
    int size=strlen(argument)+1;
    for(int j=0;j<size;j++)
    {
        if(argument[j]== quotation_mark)               
        {
            argument=deletQM(argument,size,countSpaceInQM);
            char** comand=spaceSplitArray(argument,lengthOfArrayComand);//fix it if will change something
            *lengthOfArrayComand=*lengthOfArrayComand+1;
            return comand;
        }
        if(argument[j]== equal)
        {
            saveVariables(argument,variables,maxVariabl,countVariabl);
            *lengthOfArrayComand=1;
            char** arrayComand=(char**)malloc(sizeof(char*));
            arrayComand[0]=NULL;
            return arrayComand;
        }
        if(argument[j]== Trait)
        {
            int indexValu=findVariables(variables,argument,*countVariabl);
            if( indexValu == notFoundIndex)
            {
                free(argument);
                *lengthOfArrayComand=1;
                char** arrayComand=(char**)malloc(sizeof(char*));
                arrayComand[0]=NULL;
                return arrayComand;
            }
            char* comand=changeArray(argument,indexValu,variables,maxVariabl,countVariabl);
            char** arrayComand=spaceSplitArray(comand,lengthOfArrayComand);//fix it if will change something
            *lengthOfArrayComand=*lengthOfArrayComand+1;
            return arrayComand;
        }
        
    }
    if(argument[0] == 'c' && argument[1] == 'd')
    {
        free(argument);
        *lengthOfArrayComand=1;
        perror("cd is not suported\n");
        char** arrayComand=(char**)malloc(sizeof(char*));
        arrayComand[0]=NULL;
        return arrayComand;
        
    }
    char** comand=spaceSplitArray(argument,lengthOfArrayComand);
    *lengthOfArrayComand=*lengthOfArrayComand+1;// +1 for NULL , +1 for pipe
    return comand;

}

int cheakPassWord(char* input,int cmd,int args)
{
    int passWord=1;
    while (passWord != keyPassWord)
    {
        prompt(cmd,args);
        char input[MAX_COMMAND_LENGTH];
        fgets(input,MAX_COMMAND_LENGTH,stdin);
        if(input[0] == enter)
            passWord++;
        else
            return 0;
    }
    return 1;
}

int numOfPIPE(char* array,int size,char sign)
{
    int num=1;
    for(int i=0;i<size;i++)
    {
        if(array[i] == sign)
            num++;
    }
    return num;
}

char ** redirect_operatorsplitaArray(char* array,int pipeNumber)
{
    char** comand=(char**)malloc((pipeNumber+1 )* sizeof(char*));
    char* split=strtok(array,">");
    for(int index=0;index<pipeNumber ;index++)
    {
        int size1=strlen(split);
        comand[index]=(char*)malloc((size1+1)*sizeof(char));
        strcpy(comand[index],split);
        split=strtok(NULL," ");
    }
    free(array);
    return comand;
}

char** fixTypeOfComand(char* array,int size,int* pipeNumber,int *signTypeComand)
{
    for(int i=0;i<size;i++)
    {    
        if(array[i] == verticalBar)
        {
            *pipeNumber=numOfPIPE(array,size,verticalBar);
            char** typeComand=arrayInitialization(array,*pipeNumber,"|");
            free(array);
            *signTypeComand=0;
            return typeComand; 
        }
        if(array[i] == REDIRECT_OPERATOR)
        {
            *pipeNumber=2;
            char** typeComand=redirect_operatorsplitaArray(array,*pipeNumber);
            *signTypeComand=1;
            return typeComand;
        }
    }
    return NULL;
}

void sigeChild(int num)
{
    int status;
    for(int i=0;i<countNumPID;i++)
    {
        if(savingPID[i] != -1)
        {
            int pid=waitpid(savingPID[i],&status,WNOHANG);
            if(pid == savingPID[i])
                savingPID[i]=-1;
        }
    }
}



void IfComandWorkInBG(char* argument)
{
    int size=strlen(argument);
    for(int j=0;j<size;j++)
        if(argument[j] == '&')
        {
            argument[j]='\0';
            bgcomand=2;
        }
}

void minishell()
{
    signal(SIGCHLD,sigeChild);
    signal(SIGTSTP,SIG_IGN);
    sizeSavingPID=100;
    savingPID=(int*)malloc(sizeSavingPID*sizeof(int));
    countNumPID=0;
    int num_cmd=1;
    int maxVariabl=100;
    int countVariabl=0;
    int cmd=0;
    int args=0;
    int countSpaceInQM=0;
    int lengthOfArrayComand=0;
    int bgcomand=0;
    int filecomand=0;
    pipSigTSTP=0;
    char** variables=(char**)malloc(maxVariabl* sizeof(char*));
    while(TRUE)
    {
        char input[MAX_COMMAND_LENGTH];
        prompt(cmd,args);
        fgets(input,MAX_COMMAND_LENGTH,stdin); 
        int size=strlen(input);
        if(size > MAX_COMMAND_LENGTH)
            while (size > MAX_COMMAND_LENGTH)
            {
                perror("ERR\n");
                prompt(cmd,args);
                fgets(input,MAX_COMMAND_LENGTH,stdin);
            }

        if(input[0] == enter  && cheakPassWord(input,cmd,args))
            break;
        input[size-1]='\0';
        numComands(input,&num_cmd);
        char** array= arrayInitialization(input,num_cmd,semicolon);
        for(int i=0;i<num_cmd;i++)
        {
            int size=strlen(array[i])+1;
            int pipeNumber=1;
            signTypeComand=-1;
            if(array[i][0]== 'b' && array[i][1]== 'g')
            {
                savingPID[countNumPID]=pipSigTSTP;
                countNumPID++;
                kill(pipSigTSTP,SIGCONT);
                continue;
            }
            IfComandWorkInBG(array[i]);
            char** typeComand=fixTypeOfComand(array[i],size,&pipeNumber,&signTypeComand);
            
            int pipe_fd[2];
            int savingPIPFD=-1;
            if(pipe(pipe_fd) == -1)
            {
                perror("pipe failed");
                exit(EXIT_FAILURE); 
            }
            char* inputReader;
            for(int j=0,h=0;j<pipeNumber && h==0;j++)
            {                  
                int size=0;
                if(signTypeComand == 1)
                    size=strlen(typeComand[1]);
                char file[size];
                char** arrayComand;
                if(signTypeComand == 1)
                {
                    strcpy(file,typeComand[1]);
                    free(typeComand[1]);
                }
                if(typeComand == NULL)
                    arrayComand=fixComand(array[i],variables,&maxVariabl,&countVariabl,&lengthOfArrayComand,&countSpaceInQM);
                else
                    arrayComand=fixComand(typeComand[j],variables,&maxVariabl,&countVariabl,&lengthOfArrayComand,&countSpaceInQM);
                if(signTypeComand == 1)
                {
                    h=1;
                    j=-2;
                }
                if(signTypeComand == -1)
                {
                    h=1;
                    j=-1;
                }
                int error;
                __pid_t pcopy;
                
                if(arrayComand[0] != NULL && lengthOfArrayComand <= MAX_NUMBER_ARGUMENTS+1)// +1 for NULL
                {
                    if(j !=0)
                    {
                        if(pipe(pipe_fd) == -1)
                        {
                            perror("pipe failed");
                            exit(EXIT_FAILURE); 
                        }
                    }
                    pcopy=fork();
                    if(pcopy == 0)
                    {
                        signal(SIGTSTP,SIG_DFL);
                        if(j == -2)
                        {
                            int fd = open(file, O_WRONLY | O_CREAT, 0644);
                            if (fd == -1) 
                            {
                                perror("Error opening file");
                                exit(1);
                            }
                            if(dup2(fd,STDOUT_FILENO) == -1)
                            {
                                perror("dup2");
                                exit(1);
                            }
                        }
                        if(j != -1 && j != -2 && j < pipeNumber)
                        {   
                           
                            if(j  == 0)
                            {     
                                if(dup2(pipe_fd[1],STDOUT_FILENO) == -1)
                                {
                                    perror("dup2");
                                    exit(1);
                                }
                                close(pipe_fd[0]);
                            }

                            else if(j == pipeNumber-1)
                            {                              
                                if(dup2(savingPIPFD,STDIN_FILENO) == -1)
                                {
                                    perror("dup2");
                                    exit(1);
                                }             
                            }
                            else
                            {        
                                if(dup2(savingPIPFD,STDIN_FILENO) == -1)
                                {
                                    perror("dup3");
                                    exit(1);
                                }
                                if(dup2(pipe_fd[1],STDOUT_FILENO) == -1)
                                {
                                    perror("dup4");
                                    exit(1);
                                }
                            }
                        }
                        execvp(arrayComand[0],arrayComand);
                        perror("ERR\n");
                        exit(EXIT_FAILURE);               
                    }
                }    
                if(signTypeComand == 2)
                {
                    if(countNumPID == sizeSavingPID-1)
                        savingPID=(int*)realloc(savingPID,(2*sizeSavingPID)*sizeof(int));
                    savingPID[countNumPID]=pcopy;
                    countNumPID++;
                }
                else
                {
                    savingPIPFD=pipe_fd[0];
                    close(pipe_fd[1]);
                    int status;
                    waitpid(pcopy,&status,WUNTRACED);
                    if(WIFSTOPPED(status))
                    {
                        pipSigTSTP=pcopy;  
                    }
                }
                cmd=cmd+1;
                args=args+lengthOfArrayComand-countSpaceInQM-1;
                for(int i= 0;i<lengthOfArrayComand;i++)
                    free(arrayComand[i]);
                free(arrayComand);
            }
        if(signTypeComand == 0 ||signTypeComand ==1)
            free(typeComand);
        }
        free(array);
    }
    free(savingPID);
    for(int i=0;i<countVariabl;i++)
        free(variables[i]);
    free(variables);
}

int main()
{
    minishell();
    return 0;
}

