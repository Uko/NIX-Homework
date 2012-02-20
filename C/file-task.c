#include <stdio.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <dirent.h>

struct globalArgs_t
{
  int verbose;         // -v option
  int force;           // -f option
  char **directories;  // directories parameters
  int numOfDirs;       // # of directories specified
} globalArgs;

//flag options
static const char *optString = "vfh";

static const char *usage = "usage: file-task [-vf] source_directory target_directory";

//help message
void help()
{
  puts("Copies all subdirectories from source directory to target one without overwriting");
  puts(usage);
  puts(" -v: verbose output");
  puts(" -f: force application to continue even when non critical errors appear");
  puts(" -h: help");
}

/*
 * Checks if string is in array of strings
 */
int stringArrayOfSizeContainsString(char **array, int size, char *string)
{
  int i;
  for(i=0; i<size; i++)
    if(strcmp(array[i],string)==0)
       return 1;
  return 0;
}

/*
 * Copies file specified in src path to dst path.
 */
int copyFile(char *src, char *dst)
{
  FILE *srcFile,*dstFile;
    char buffer;
 
    srcFile = fopen(src,"r");
    if(srcFile)
    {
        dstFile = fopen(dst,"w");
	if(dstFile)
	{
	  if(globalArgs.verbose) printf("%s -> %s\n",src,dst);

	  while((buffer=getc(srcFile))!=EOF)
	  {
	    putc(buffer,dstFile);
	  }

	  fclose(srcFile);
	  fclose(dstFile);
	  return 0;
	}
	else
	{
	  printf("Cannot write file: %s\n",dst);
	  fclose(srcFile);
	}	
    }
    else
    {
      printf("Cannot read file: %s\n",src); 
    }
    return 1;
}

/*
 * Copies file/directory specified with from+what path to where+what path
 * including it's contents (in case of dir)
 */
int recursiveCopy(char *from, char *where, char *what, int file)
{
  //spacifies if somefing in the function's lifecycle failed
  int fail = 0;
  //full path to sourse of copy
  char * thisPath = malloc(snprintf(NULL, 0, "%s/%s", from, what) + 1);
  sprintf(thisPath, "%s/%s", from, what);
  //full path to the destination of copy
  char * newPath = malloc(snprintf(NULL, 0, "%s/%s", where, what) + 1);
  sprintf(newPath, "%s/%s", where, what);

  //handle file
  if(file)
  {
    fail = copyFile(thisPath,newPath);
  }
  //handle directory
  else
  {
    //make the same directory in the destination place
    if(fail=mkdir(newPath,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH))
    {
      printf("Cannot copy directory: %s\n",thisPath);
    }
    //open source directory and process it's contents
    else
    {
      if (globalArgs.verbose) printf("%s/ -> %s/\n",thisPath, newPath);
      DIR *thisDir;
      struct dirent* dirEntry;
      if(thisDir = opendir(thisPath))
      {
	//continue copying while there is still stuff to copy AND
	//there were no fails OR force mode is ON
	while((globalArgs.force||!fail)&&(dirEntry = readdir(thisDir)))
	{
	  if( strcmp(dirEntry->d_name, ".") != 0 &&
	      strcmp(dirEntry->d_name, "..") != 0 &&
	      (dirEntry->d_type == DT_REG ||
	       dirEntry->d_type == DT_DIR))
	  {
	    fail=recursiveCopy(thisPath, newPath, dirEntry->d_name, dirEntry->d_type==DT_REG);
	  }
	}
	closedir(thisDir);
      }
      else
      {
	printf("Cannot read directory: %s\n",thisPath);
	fail=!globalArgs.force;
      }
    }
  }
  free(thisPath);
  free(newPath);
  return fail;
}

/*
 * Copies subdirectories of source directory to destination directory
 * if directories with the same names are not present there allready
 */
int cpSubdirs(char *src, char *dst)
{
  //spacifies if somefing in the function's lifecycle failed
  int fail = 0;
  DIR *srcDir;
  DIR *dstDir;
  struct dirent *dirEntry;
  //array of dir names present in destination directory
  char **destDirSubdirs;
  //amount of subdirectories in destination directory
  int amountOFSubdirsInDstDir = 0;
  //amount of space allocated for subdir names array
  int destDirSubdirsThreshold = 10;

  if(dstDir = opendir(dst))
  {
    destDirSubdirs = malloc(destDirSubdirsThreshold*sizeof(char *));
    while(dirEntry = readdir(dstDir))
    {
      if( dirEntry->d_type == DT_DIR &&
	  strcmp(dirEntry->d_name, ".") != 0 &&
	  strcmp(dirEntry->d_name, "..") != 0 )
      {
	//if the array if filled up, realoc more memory
	if(amountOFSubdirsInDstDir==destDirSubdirsThreshold)
	{
	  destDirSubdirsThreshold*=2;
	  destDirSubdirs = realloc(destDirSubdirs,destDirSubdirsThreshold*sizeof(char *));
	}
	destDirSubdirs[amountOFSubdirsInDstDir]=dirEntry->d_name;
	amountOFSubdirsInDstDir++;
      }
    }
    if(srcDir = opendir(src))
    {
      //continue copying while there is still stuff to copy AND
      //there were no fails OR force mode is ON
      while((globalArgs.force||!fail)&&(dirEntry = readdir(srcDir)))
      {
	if( dirEntry->d_type == DT_DIR &&
	    strcmp(dirEntry->d_name, ".") != 0 &&
	    strcmp(dirEntry->d_name, "..") != 0 &&
	    !stringArrayOfSizeContainsString(destDirSubdirs,
					     amountOFSubdirsInDstDir,
					     dirEntry->d_name))
	{
	  fail=recursiveCopy(src,dst,dirEntry->d_name,0);
	}
      }
      closedir(srcDir);
    }
    else
    {
      printf("Cannot open source directory: '%s'\n", src);
      fail = 1;
    }
    free(destDirSubdirs);
    closedir(dstDir);
  }
  else
  {
    printf("Cannot open target directory: '%s'\n", dst);
    fail = 1;
  } 

  return fail;
}

int main(int argc, char *argv[])
{
  int opt = 0;
  
  //glogalArgs initialization
  globalArgs.verbose = 0;
  globalArgs.force = 0;
  globalArgs.directories = NULL;
  globalArgs.numOfDirs = 0;

  opt = getopt(argc, argv, optString);
  while(opt != -1)
  {
    switch(opt)
    {
      case 'v':
	globalArgs.verbose = 1; //true
	break;
      case 'f':
        globalArgs.force = 1; //true
        break;
      case 'h':
	help();
      default:
	return 1;
    }
    opt = getopt(argc, argv, optString);
  }

  globalArgs.directories = argv + optind;
  globalArgs.numOfDirs = argc - optind;
  
  if(globalArgs.numOfDirs<2)
  {
    puts(usage);
    return 1;
  }
 
  return cpSubdirs(globalArgs.directories[0],globalArgs.directories[1]);;
}
