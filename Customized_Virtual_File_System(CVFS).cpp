////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
//#include<unistd.h>
#include <stdbool.h>
#include <string.h>
#include<fcntl.h>
#include<io.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

// Maximum file size that we allow in our project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name:     BootBlock
//  Discription:        Holds the information to boot the OS
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name:     SuperBlock
//  Discription:        Holds the information about the file system
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name:     Inode
//  Discription:        Holds the information about the file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode *PINODE;
typedef struct Inode **PPINODE;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name:     FileTable
//  Discription:        Holds the information about the  opened file
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable *PFILETABLE;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name:     UAREA
//  Discription:        Holds the information about the process
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Global Variables or objects used in project
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: InitialiseUAREA()
//      Discription:  It is used to initialise UAREA members
//      Author:       Kishor Suryabhan Handge
//      Date:         13-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for (i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS: UAREA gets initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName:   InitializseSuperBlock()
//      Discription:    It is used to initialise SuperBlock members
//      Author:         Kishor Suryabhan Handge
//      Date:           13-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitializseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS: Super block gets initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: CreateDILB()
//      Discription:  It is used to Create Linked List of inodes
//      Author:       Kishor Suryabhan Handge
//      Date:         13-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;

    PINODE newn = NULL;
    PINODE temp = head;

    for (i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName, "\0");
        newn->InodeNumber = i;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if (temp == NULL) // LL is empty
        {
            head = newn;
            temp = head;
        }
        else // LL contains at least one node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: StartAuxillaryDataInitialization()
//      Discription:  It is used to call all such functions which are used to initialise auxillary data
//      Author:       Kishor Suryabhan Handge
//      Date:         13-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialization()
{
    strcpy(bootobj.Information, "Booting Process Of Marvellous CVFS Is Done");

    printf("%s\n", bootobj.Information);

    InitializseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary data initialised succesfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: DisplayHelp()
//      Discription:  It is used to display the help page
//      Author:       Kishor Suryabhan Handge
//      Date:         14-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    int fd = 0;
    int iRet = 0;

    char Buffer[150] = {'\0'};

    fd = open("Help.txt", O_RDWR);

    while ((iRet = read(fd, Buffer, sizeof(Buffer))) != 0)
    {
        write(1, Buffer, iRet);
    }

    close(fd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: ManPageDisplay()
//      Discription:  It is used to display the man page
//      Author:       Kishor Suryabhan Handge
//      Date:         14-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if (strcmp("ls", Name) == 0)
    {
        printf("About: It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if (strcmp("man", Name) == 0)
    {
        printf("About: It is used to display the manual page\n");
        printf("Usage : man command_name\n");
        printf("command_name: It is the name of command\n");
    }
    else if (strcmp("exit", Name) == 0)
    {
        printf("About: It is used to terminate the shell\n");
        printf("Usage : exit\n");
    }
    else if (strcmp("clear", Name) == 0)
    {
        printf("About: It is used to clear the shell\n");
        printf("Usage : clear\n");
    }
    else
    {
        printf("No Manual Entry for %s\n", Name);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: IsFileExist()
//      Discription:  It is used to check wheather file already exist or not
//      Input:        It Accepts file name
//      Ouput:        It returns true or false
//      Author:       Kishor Suryabhan Handge
//      Date:         16-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
    char *name // file name
)

{
    PINODE temp = head;
    bool bFlag = false;

    while (temp != NULL)
    {
        if ((strcmp(name, temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }

        temp = temp->next;
    }

    return bFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: CreateFile()
//      Discription:  It is used to create new regular file
//      Input:        It Accepts file name permissions
//      Ouput:        It returns file discriptor
//      Author:       Kishor Suryabhan Handge
//      Date:         16-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
    char *name,    // Name of new file
    int permission // permission for that file

)
{
    PINODE temp = head;
    int i = 0;

    printf("Total Number Of Inodes Remaining : %d\n", superobj.FreeInodes);

    // if name is missing
    if (name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the permission value is wrong

    // permission-->1--head
    // permission ->2-->READ
    // permission-->3-->WRITE
    // permission-->1 + 2 -->READ + WRITE

    if (permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the Inodes are full
    if (superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // if file is already persent
    if (IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while (temp != NULL)
    {
        if (temp->FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("There is no Inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // NOTE- > 0 ,1, 2 are reserve
    for (i = 3; i < MAXOPENFILES; i++)
    {
        if (uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if (i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory file for table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect file table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode

    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate ememory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i; // file discriptor
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: LsFile()
//      Discription:  It is used to list all files.
//      Input:        Nothing
//      Ouput:        Nothing
//      Author:       Kishor Suryabhan Handge
//      Date:         16-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("--------------------------------------------------------------------------------------\n");
    printf("---------------------------Marvellous CVFS Files Information--------------------------\n");
    printf("--------------------------------------------------------------------------------------\n");

    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("%d\t%s\t%d\n", temp->InodeNumber, temp->FileName, temp->ActualFileSize);
        }

        temp = temp->next;
    }

    printf("--------------------------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: UnlinkFile()
//      Discription:  It is used to delete the file.
//      Input:        File Name
//      Ouput:        Nothing
//      Author:       Kishor Suryabhan Handge
//      Date:         22-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(

                char *name                
                )

{   
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT

    for(i = 0; i<MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer

                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                //Reset all value of inode
                //Dont Deallocate  memory of inode

                uareaobj.UFDT[i]->ptrinode-> FileSize = 0;
                uareaobj.UFDT[i]->ptrinode-> ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode-> FileType = 0;
                uareaobj.UFDT[i]->ptrinode-> ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode-> Permission = 0;
                

                memset( uareaobj.UFDT[i]->ptrinode->FileName , '\0',sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Deallocate memory of file table
                free(uareaobj.UFDT[i]);


                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // Increment free inodes count
                superobj.FreeInodes++;

                break;  //IMP
            }   //End if

        }   //End of if

    }   //End of for

    return EXECUTE_SUCCESS;

}   //End of function


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: WriteFile()
//      Discription:  It is used to write the data into the file.
//      Input:        File Discriptor
//                    Addres Of Buffer which contain data
//                    size of data that we want
//
//      Ouput:        Number Of Bytes Succesfully Written
//      Author:       Kishor Suryabhan Handge
//      Date:         22-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(

                int fd,
                char *data,
                int size

            )

{
    printf("File Discriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);


    //Invalid fd
    if(fd < 3 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //fd points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //There is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    //Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //write the data into the file

    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data,size);
     

    //update the writeOffset
    uareaobj.UFDT[fd]->WriteOffset =  uareaobj.UFDT[fd]->WriteOffset  + size;


    //update the Actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize  + size;

    return size;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      FunctionName: ReadFile()
//      Discription:  It is used to read the data from the file.
//      Input:        File Discriptor
//                    Addres Of Empty Buffer
//                    size of data that we want read
//
//      Ouput:        Number Of Bytes Succesfully Read
//      Author:       Kishor Suryabhan Handge
//      Date:         22-01-2026
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size

)
{
    //Invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //Invalid data
    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter For Permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    //Insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data , uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    //Update the read Offset
    uareaobj.UFDT[fd]->ReadOffset =  uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point Function Of The Project
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitialization();

    printf("--------------------------------------------------------------------------------------\n");
    printf("---------------------------Marvellous CVFS Started Succesfully------------------------\n");
    printf("--------------------------------------------------------------------------------------\n");

    // Infinite Listening Shell
    while (1)
    {
        fflush(stdin);

        strcpy(str, "");

        printf("\nMarvellous CVFS : > ");
        fgets(str, sizeof(str), stdin); // scanf("%s",str);

        iCount = sscanf(str, "%s %s %s %s %s", Command[0], Command[1], Command[2], Command[3], Command[4]);

        fflush(stdin);

        if (iCount == 1)
        {
            // Marvellous CVFS : > exit
            if (strcmp("exit", Command[0]) == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }

            ////Marvellous CVFS : > ls
            else if (strcmp("ls", Command[0]) == 0)
            {
                LsFile();
            }

            // Marvellous CVFS : > help
            else if (strcmp("help", Command[0]) == 0)
            {
                DisplayHelp();
            }

            ////Marvellous CVFS : > clear
            else if (strcmp("clear", Command[0]) == 0)
            {
                #ifdef _WIN32
                system("cls");

                #else

                system("clear");

                #endif
            }
        } // End of if 1
        else if (iCount == 2)
        {
            ////Marvellous CVFS : > man ls
            if (strcmp("man", Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }

            ////Marvellous CVFS : > unlink Demo.txt
            if (strcmp("unlink", Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error: Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: Unable to delete as there is no such file\n");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }


            }
            //Marvellous CVFS: > write 2
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter the data that you want to write: \n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer,strlen(InputBuffer) -1 );

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error: Invalid Parameters:\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: There is no such file:\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Unable to write as there is no permission:\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error: Unable to write as there is no space:\n");
                }
                else
                {
                    printf("%d Bytes gets succesfully written\n",iRet);
                }
            }
            else
            {
                printf("There is no such command\n");
            }
        } // End of else if 2

        
        else if (iCount == 3)
        {
            ////Marvellous CVFS : >creat Ganesh.txt 3
            if (strcmp("creat", Command[0]) == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));

                if (iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please Refer the man page\n");
                }

                if (iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create the file as there is no inode\n");
                }

                if (iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file beacause the file is already present\n");
                }

                if (iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened file limit reach\n");
                }

                printf("File Gets Succesfully Created with FD %d\n", iRet);
            }

            // Marvellous CVFS: > read 3 10
           else if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]),EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error: Invalid Parameter\n");
                }

                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: File Not Exist\n");
                }

                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Permission denied\n");
                }

                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error: Insufficient data\n");
                }
                else 
                {
                    printf("Read Operation is Succesful:\n");
                    printf("Data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);
                }

            }
            else
            {
                printf("There is no such command\n");
            }
        } // End of else if 3
        else if (iCount == 4)
        {

        } // End of else if 4
        else
        {
            printf("Command Not Found\n");
            printf("Please Refer help option to get more information\n");
        } // End of else

    } // End of while

    return 0;
} // End of main
