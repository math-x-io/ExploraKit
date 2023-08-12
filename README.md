# ExploraKit
<img src="https://www.pokebip.com/pages/icones/objets/explorakit.png" style="width:80px;">
Explorakit is an open-source C++ program that allows users to search for a specific file on storage drives for Windows. Users can choose to search all available drives or specify specific drives to explore. The program provides a visual progress bar during the search and displays search results along with the search duration.

# Usage
The program asks the user to provide the name of the file they want to find. Users can decide to look for the file on all the drives in their computer or pick certain drive letters to check. The program creates a list of drives to look through, based on what the user chose. It keeps searching for the file, and checking inside folders as well. You can see a bar that shows the progress of the search. When the search is done, the program tells if the file was found or not, and how long the search took.

### Make a Windows executable  
Note: If you open the project with VS Code or Visual Studio you won't need to type the following command. 

```PowerShell
g++.exe -Wall -Wextra -g3 ExploraKit_V1.cpp -o .\output\ExploraKit_V1.exe
```

### Make source code files

```powershell
g++.exe -S .\ExploraKit_V1.cpp
```
or if you want only look at the assembly code : 
```powershell
objdump.exe -S .\output\ExploraKit_V1.exe
```
# Future Improvements:

- Implementing multi-threading for faster searching.
- Adding options for advanced search filters (file extensions, modified dates, etc.).
- Enhancing error handling and user-friendly messages.
- Creating a graphical user interface for ease of use.
- Add benchmark
- Add Graph theory algorithm
