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

int cpSubdirs(char *src, char *dst)
{
  DIR *srcDir;
  DIR *dstDir;
  struct dirent* dirEntry;
  
  srcDir = opendir(src);
  dstDir = opendir(dst);
  if(srcDir == NULL)
  {
    printf("Cannot open directory: '%s'\n", src);
    exit(EXIT_FAILURE);
  }
  if(dstDir == NULL)
  {
    printf("Cannot open directory: '%s'\n", dst);
    exit(EXIT_FAILURE);
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
  

  printf("args left %i\n",globalArgs.numOfDirs);
  return 0;
}
