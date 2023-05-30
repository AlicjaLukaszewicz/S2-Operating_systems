# Operating Systems Course Projects

This repository contains the code for the laboratory assignments and projects of the "Operating systems" course taken during the second semester of 2022. 


## Firefox-browser-manager

### Description

The "Firefox browser manager" is a command-line program that allows users to perform the following operations:

- **Browser History Management:** Users can choose to delete all browser history or specify a time range for deletion.
- **Cookie Management:** Users can delete all cookies stored in the Firefox browser.
- **Phrase Search:** Users can search for a phrase within the browser history, with options to match case and match entire word only.

The program interacts with the Firefox browser databases located in the user's home directory.

### Installation

The following packages are installed to run the program:

- `zenity`
- `sqlite3`

If any package is missing, the program will attempt to install it using the appropriate package manager (apt-get, yum, pacman, zypper).

To install the required packages manually, refer to the documentation provided by the package's respective distribution.

### Usage

To run the program, execute the `firefox_browser_manager.sh` script. The program provides the following command-line options:

- `-v, --version`: Displays version and author information.
- `-h, --help`: Displays help on command-line options.
- `-a, --author`: Displays author information.
- `--display-history`: Displays current browser history.
- `--display-cookies`: Displays current browser cookies.
- `--display-all`: Displays both history and cookies.

When running the program, ensure that Firefox browser is closed. The program will prompt to close the browser if it's open.

---

## LS Command implementation

This code implements a simplified version of the `ls` command in Linux. It lists the contents of a directory, including files and subdirectories, and provides various options for displaying additional information.

### Usage

To compile and run the code, use the following command:
```bash
gcc ls.c -o ls
./ls [options] [directory]
```
Replace `[options]` with one or more of the following flags:

- `l`: Print long format information for each entry.
- `R`: Recursively list subdirectories.

Replace `[directory]` with the path to the directory you want to list. If no directory is specified, the current directory will be used.

### Examples

List the contents of the current directory:
```bash
./ls
```
List the contents of the /home/user/Documents directory in long format:
```bash
./ls -l /home/user/Documents
```
Recursively list the contents of the /home/user/Documents directory:
```bash
./ls -R /home/user/Documents
```
List the contents of the current directory with both options:
```bash
./ls -l -R
```