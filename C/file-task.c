#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <dirent.h>

struct globalArgs_t
{
  int verbosity;       // -v option
  char **directories;  // directories parameters
  int numOfDirs;       // # of directories specified
} globalArgs;

static const char *optString = "vh";

static const char *usage = "usage: file-task [-v] source_directory target_directory";

void help()
{
  puts("Copies all subdirectories from source directory to target one without overwriting");
  puts(usage);
  puts(" -v: verbose output");
  puts(" -h: help");
  exit(EXIT_FAILURE);
}

int stringArrayOfSizeContainsString(char **array, int size, char *string)
{
  int i;
  for(i=0; i<size; i++)
    if(strcmp(array[i],string)==0)
       return 1;
  return 0;
}

int recursiveCopy(char *from, char *where, char *what, int file)
{
  if(file)
  {
    //implement file copy
    printf("file: %s/%s\n",from,what);
  }
  else
  {
    char * newDirPath = malloc(snprintf(NULL, 0, "%s/%s", where, what) + 1);
    sprintf(newDirPath, "%s/%s", where, what);
    if(mkdir(newDirPath,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH))
    {
      printf("Failed to copy directory: %s\n",newDirPath);
      return 1;
    }
    DIR *thisDir;
    struct dirent* dirEntry;
    char * thisDirPath = malloc(snprintf(NULL, 0, "%s/%s", from, what) + 1);
    sprintf(thisDirPath, "%s/%s", from, what);
    if(thisDir = opendir(thisDirPath))
    {
      while(dirEntry = readdir(thisDir))
      {
	if( strcmp(dirEntry->d_name, ".") != 0 &&
	    strcmp(dirEntry->d_name, "..") != 0 )
	{
	   recursiveCopy(thisDirPath, newDirPath, dirEntry->d_name, dirEntry->d_type==DT_REG);
	}
      }
      closedir(thisDir);
    }
    return 0;
  }
}

int cpSubdirs(char *src, char *dst)
{
  DIR *srcDir;
  DIR *dstDir;
  struct dirent* dirEntry;
  char **destDirSubdirs;
  int destDirSubdirsThreshold = 10;
  destDirSubdirs = malloc(destDirSubdirsThreshold*sizeof(char *));
  int amountOFSubdirsInDstDir = 0;
  

  
  if(dstDir = opendir(dst))
  {
    while(dirEntry = readdir(dstDir))
    {
      if( dirEntry->d_type == DT_DIR &&
	  strcmp(dirEntry->d_name, ".") != 0 &&
	  strcmp(dirEntry->d_name, "..") != 0 )
      {
	if(amountOFSubdirsInDstDir==destDirSubdirsThreshold)
	{
	  destDirSubdirsThreshold*=2;
	  destDirSubdirs = realloc(destDirSubdirs,destDirSubdirsThreshold);
	}
	destDirSubdirs[amountOFSubdirsInDstDir]=dirEntry->d_name;
	amountOFSubdirsInDstDir++;
      }
    }
    if(srcDir = opendir(src))
    {
      while(dirEntry = readdir(srcDir))
      {
	if( dirEntry->d_type == DT_DIR &&
	    strcmp(dirEntry->d_name, ".") != 0 &&
	    strcmp(dirEntry->d_name, "..") != 0 &&
	    !stringArrayOfSizeContainsString(destDirSubdirs,
					     amountOFSubdirsInDstDir,
					     dirEntry->d_name))
	{
	  recursiveCopy(src,dst,dirEntry->d_name,0);
	}
      }
    }
    else
    {
      printf("Cannot open directory: '%s'\n", src);
      return 1;
    }
  }
  else
  {
    printf("Cannot open directory: '%s'\n", dst);
    return 1;
  } 

  return 0;
}

int main(int argc, char *argv[])
{
  int opt = 0;
  
  //glogalArgs initialization
  globalArgs.verbosity = 0;
  globalArgs.directories = NULL;
  globalArgs.numOfDirs = 0;

  opt = getopt(argc, argv, optString);
  while(opt != -1)
  {
    switch(opt)
    {
      case 'v':
	globalArgs.verbosity = 1; //true
	break;
      case 'h':
	help();
        break;
      default:
	abort();
    }
    opt = getopt(argc, argv, optString);
  }

  globalArgs.directories = argv + optind;
  globalArgs.numOfDirs = argc - optind;
  
  if(globalArgs.numOfDirs<2)
  {
    puts(usage);
    exit(EXIT_FAILURE);
  }
  

  cpSubdirs(globalArgs.directories[0],globalArgs.directories[1]);
  return 0;
}
