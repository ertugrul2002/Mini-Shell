# Mini Shell

A command-line shell built in C to simulate operating system command execution. This project was developed as a second-year learning exercise to explore OS concepts like process management, input parsing, and piping.

## Key Features
- **Basic Commands**: Executes commands like `ls` (or `dir`), `cat`, `echo`, `grep`, `wc`, etc.
- **Piping**: Supports piping between commands (e.g., `ls | grep "text"`).
- **Command-Line Flags**: Handles flags like `-l`, `-n`, `-w` (e.g., `ls -l`, `wc -w`).
- **Redirection**: Supports output redirection (e.g., `echo "text" > file.txt`).
- **Command & Argument Counter**: Tracks the number of commands and arguments in real-time (e.g., `#cmd:<N>|#args:<M>`).
- **Partial Support for Quoted Commands**: Works with quoted arguments (e.g., `grep "text"`), but quoted commands with pipes (e.g., `"ls | grep"`) are under development.

## How to Run
1. **Compile**: `gcc -o shell main.c`
2. **Run**: `./shell`

## Usage Examples 1
- `ls -l`: Lists files with detailed information.
- `echo "hello there" > file.txt`: Writes "hello there" to `file.txt`.
- `wc -l file.txt`: Counts lines in a file.
- `wc -w file.txt`: Counts words in a file.
- `cat file.txt | nl`: Numbers the lines of `file.txt`.
-` head -n 5 file.txt` Displays the first 5 lines of `file.txt`: 

### Real Example 1####

#cmd:<0>|#args:<0>@/home/ertugrul/Desktop/oprating system/mini shell> cat file.txt
cat: file.txt: No such file or directory
#cmd:<1>|#args:<2>@/home/ertugrul/Desktop/oprating system/mini shell> sl
ERR
: No such file or directory
#cmd:<2>|#args:<3>@/home/ertugrul/Desktop/oprating system/mini shell> touch file.txt
#cmd:<3>|#args:<5>@/home/ertugrul/Desktop/oprating system/mini shell> ls
exe1.c  file.txt  shell
#cmd:<4>|#args:<6>@/home/ertugrul/Desktop/oprating system/mini shell> echo "hello there" > file.txt
#cmd:<5>|#args:<8>@/home/ertugrul/Desktop/oprating system/mini shell> ls
exe1.c  file.txt  shell
#cmd:<6>|#args:<8>@/home/ertugrul/Desktop/oprating system/mini shell> head -n 5 file.txt
hello there
#cmd:<7>|#args:<11>@/home/ertugrul/Desktop/oprating system/mini shell> wc -l file.txt
1 file.txt
#cmd:<8>|#args:<13>@/home/ertugrul/Desktop/oprating system/mini shell> wc -w file.txt
2 file.txt
#cmd:<9>|#args:<15>@/home/ertugrul/Desktop/oprating system/mini shell> grep "hello" file.txt
hello there
#cmd:<10>|#args:<17>@/home/ertugrul/Desktop/oprating system/mini shell> cat file.txt | nl
     1  hello there
#cmd:<12>|#args:<18>@/home/ertugrul/Desktop/oprating system/mini shell> ^C   
###########################################################################################################################

## Usage Examples 2
- `echo "afaf agga ggrwfer" > data.txt`: Writes "afaf agga ggrwfer" to `data.txt`.
- `grep "a" data.txt > filetered.txt"`: Filters lines containing "a" and saves to `filtered.txt` .
- `cat filetered.txt | nl`: Displays the content of `filetered.txt`.

### Real Example 2 ####
#cmd:<0>|#args:<0>@/home/ertugrul/Desktop/oprating system/mini shell> echo "afaf agga ggrwfer" > data.txt
#cmd:<1>|#args:<2>@/home/ertugrul/Desktop/oprating system/mini shell> ls
data.txt  exe1.c  file.txt  shell
#cmd:<2>|#args:<1>@/home/ertugrul/Desktop/oprating system/mini shell> grep "a" data.txt > filetered.txt
#cmd:<3>|#args:<2>@/home/ertugrul/Desktop/oprating system/mini shell> ls
data.txt  exe1.c  filetered.txt  file.txt  shell
#cmd:<4>|#args:<1>@/home/ertugrul/Desktop/oprating system/mini shell> cat filetered.txt
afaf agga ggrwfer
#cmd:<5>|#args:<1>@/home/ertugrul/Desktop/oprating system/mini shell> ^C
###########################################################################################################################

## Usage Examples 3
- `ls | grep "shell"`: Filters output to show lines containing "shell".
-`"ls -l" `:Attempts to execute `ls -l` within quotes (may cause issues)

### Real Example 3 ####
#cmd:<0>|#args:<0>@/home/ertugrul/Desktop/oprating system/mini shell> ls | grep "shell"
shell
#cmd:<2>|#args:<3>@/home/ertugrul/Desktop/oprating system/mini shell> "ls -l"
total 60
-rw-r--r-- 1 ertugrul ertugrul    18 Mar 11 20:32 data.txt
-rw-rw-r-- 1 ertugrul ertugrul 18796 May  7  2023 exe1.c
-rw-r--r-- 1 ertugrul ertugrul    18 Mar 11 20:32 filetered.txt
-rw-rw-r-- 1 ertugrul ertugrul    12 Mar 11 20:10 file.txt
-rwxrwxr-x 1 ertugrul ertugrul 26064 Mar 11 20:03 shell
#cmd:<3>|#args:<4>@/home/ertugrul/Desktop/oprating system/mini shell> ^C



## Notes
- Developed as a second-year project to explore OS concepts.
- Piping works, but quoted commands with pipes (e.g., `"ls | grep"`) may cause issues; fix in progress.
- No support for `cd` command (displays "cd is not supported").

## Future Improvements
- Fix parsing of quoted commands with pipes (e.g., `"ls | grep"`) to prevent segmentation faults.
- Add support for more complex piping scenarios.
