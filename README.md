# indexer

	198:211   SYSTEMS PROGRAMMING
	PA4:      INDEXER

## Contents

1. [Current Use](#current-use)
1. [Data Structures](#data-structures)
1. [Runtime Analysis](#runtime-analysis)
1. [Testcase Analysis](#testcase-analysis)
	1. [Testcase 1](#testcase-1): Directory sweeping
	1. [Testcase 2](#testcase-2): Frequency counting
	1. [Testcase 3](#testcase-3): Empty nested directories
	1. [Testcase 4](#testcase-4): Empty text files
	1. [Testcase 5](#testcase-5): Unfamiliar file types
	1. [Testcase 6](#testcase-6): Regular large test
	1. [Testcase 7](#testcase-7): Input is a text file


# Current Use

__Will not properly run on Windows machines due to sys/types.h__

```sh
make 
./index outputfile.txt testing 
```
Make will build and link library and object files necessary for the indexer. 
The program is invoked with two arguments:  the output text file for the JSON to be written to and the directory/file to parse for tokens.


My  implementation of the indexer utilizes the previous projects: `tokenizer` and `sortedNlist`. Modifications were made to these for use in this specific project. The source code for the indexer is specifically implemented within `indexer.c`. The `main()` function is also located  within this source file. Proper execution of my program is as follows:

```sh
make 
./index output.txt testing01
./index output.txt testing02
```

# DATA STRUCTURES 

![screenshot](images/structures.png)

The highest level data structure in use to store (token, file) tuples is the `TokenList`. This is a global `sortedNlist` structure of Token structs. Each Token struct itself, contains a `sortedNlist` structure within it.  This is a `sortedNlist` of `fileRecord` structs that correspond to the files which contain the token. This nested design lends nicely to minimizing the memory requirements of storing tokens along with their filenames and frequencies. Additionally, management of the ordering of each of the tokens, frequency, and filenames with  respect to one another can be entirely handled by the `sortedNlist` objects. Comparator and  destroyer functions for Token and `fileRecord` structs are implemented within `indexer.c`.    


# RUNTIME ANALYSIS 

Runtime is considered with respect to the size of the input. This is the number of files that need to be parsed for tokens. Sweeping through directories is done using the recursive `exploreDirectories()` function. It will be called once for each directory in the input. For each file it locates within a  directory, it calls `parseFile()`.

The `parseFile()` function instantiates a tokenizer object. The tokenizer object will fetch tokens from the input file and store them into the aforementioned data structures by  calling the helper function, `processToken()`. The tokenizer struct fetches tokens from the  input until EOF is reached.  Then the tokenizer is destroyed and the call stack returns to `exploreDirectories()`.

The `processToken()` function will run two comparisons  to check if the input tuple (token, filename) already exists within the data structures. It must first check the TokenList  `sortedNlist` to see if a Token struct has already been instantiated for the input. If so, it will  check the `fileRecord sortedNlist` of that token to see if this file has already had an instance of  this token.  If so, it will increment the frequency and resort. The runtime of this function will  depend largely on the current size of the data structures. Therefore, this runtime will scale with the number of unique tokens and the number of files in the input. The runtime is `O(tf)` where `t` is the number of unique tokens and `f` is the number of files. Worst case, there are unique tokens in every file.

Once the data structure is created, a simple function, `writeFile()`, will output the data  structure to the output file in JSON format.  This is `O(tf)` time as well since every token of  every file needs to be visited.

Therefore, as the directory navigation and file parsing is as efficient as possible ( `O(n)`  where n is the number of files to explore), the runtime of my program will largely depend on  the number of unique tokens and the number of files.  `O(tf)`. 
  

# TESTCASE ANALYSIS 

![Testcase directory](images/testDirs.png)

The following testcases (represented above) attempt  to  challenge  my  programs  functioanlity  in  a  slightly  different  way. Each testcase is a top directory denoted  by `testing0x`. 


## TESTCASE 1

__Directory sweeping__

Testing01 contains 2 directories and 1 nested subdirectory  within  the  first.    There  are  text  files  at  all  levels  of  navigation  (high level, dir1/, dir1/dir2/, and dir3/ ).  This test case is largely  meant  to  test  my  programs  ability  to  recursively  sweep  directories and visit all files at all levels.   

 
## TESTCASE 2

__Frequency counting__

![screenshot](images/token_order.png)

Testing02 contains just two text files, each of which contain  some  duplicate  words. This  test  attempts  to test the  sorting  ability of the nested sortedNlists. The image below shows the output file from this test. Tokens are properly organized alphabetically.    RecordLists  are  properly organized by frequency and then alphabetically by file path. This is further tested by the following test cases as well.    


## TESTCASE 3

__Empty nested directories__

This test case exercises the simple case in which there are no files with any tokens.  The  directories are properly swept.  When the TokenList structure is empty at time of file writing,  the program will safety exit with a short prompt claiming that there are no tokens to index. 


## TESTCASE 4

__Empty text files__

This test case is similar to test case 3.  The high level file contains empty text files.  The  program will follow an execution, just as test case 3, and exit normally with a prompt saying  there are no tokens to index. 


## TESTCASE 5

__Unfamiliar file types__

The high level directory contains one normal text file, name heyya.txt.  Additionally, it  contains a directory of two executable files.  The program will attempt to parse through these  files normally and detect any string of valid tokens.  The resulting output text file is as expected.    


## TESTCASE 6

__Regular large test__

This test case is a large directory with many text files.  This test is meant to test the full  functionality of the program.  The following two images show the proper sorting of recordLists  by first frequency and then alphabetically by file name.   

![screenshot](images/freq_order.png)

![screenshot](images/alpha_order.png)


## TESTCASE 7

__Input is a text file__

Lastly, this test case shows proper execution of my program when the input is a text file.   My program properly recognizes that the input is not a directory and will execute normally. 

 ```sh
./index output.txt svu.txt 
```
