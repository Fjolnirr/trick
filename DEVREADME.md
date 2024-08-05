# A Guide for the developers who have no idea how to debug trick

I have managed to debug whole trick code. Trick sources(services), trick ICG, and simulation model source codes. 
I will explain what you need to do to debug trick step by step. 

Before making any changes we will set few env variables:      

- Open your terminal at home dir.      
- type `nano .bashrc` hit enter and go to the end of the page.     
- add this lines;

```
export CFLAGS="$CFLAGS $1 -O0 -g"
export CXXFLAGS="$CXXFLAGS $1 -O0 -g"
export LDFLAGS="$LDFLAGS $1 -O0 -g"
export TRICK_CFLAGS="$TRICK_CFLAGS $1 -O0 -g"
export TRICK_CXXFLAGS="$TRICK_CXXFLAGS $1 -O0 -g"
export TRICK_LDFLAGS="$TRICK_LDFLAGS $1 -O0 -g"
export TRICK_SYSTEM_CFLAGS="$TRICK_SYSTEM_CFLAGS $1 -O0 -g"
export TRICK_SYSTEM_CXXFLAGS="$TRICK_SYSTEM_CXXFLAGS $1 -O0 -g"
export TRICK_SYSTEM_LDFLAGS="$TRICK_SYSTEM_LDFLAGS $1 -O0 -g"
```
save and exit.

1. First you need to add this lines to the `Makefile` of the trick located at `trick/Makefile` 
``` 
CXX = g++
CXXFLAGS = -std=c++11 -g
```
2. `./configure` and then `make` the trick at TRICK_HOME dir. (You can use make -j8 for faster compilation time)     

3. You need to have simulation like `SIM_cannon_analytic` (I have used the one located in `trick/trick_sims/Cannon/SIM_cannon_analytic`

4. Open `S_overrides.mk` and paste this lines:

```
TRICK_CFLAGS += -I../models -I/home/backend/trick/trick_source -g 
TRICK_CXXFLAGS += -I../models -I/home/backend/trick/trick_source -g 
```

5. In terminal,  use `trick-CP -g` command:      
`backend@backend-VirtualBox:~/trick/trick_sims/Cannon/SIM_cannon_analytic$ trick-CP -g`

6. Open vscode at TRICK_HOME dir.

7. Create a launch.json file. This is for attaching to process on launch.     
```
{
"version": "0.2.0",
    "configurations": [
        {   
            "name": "(gdb) Launch SimExe",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/trick_sims/Cannon/SIM_cannon_analytic/S_main_Linux_11.4_x86_64.exe",  
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "/usr/bin/gdb",  
            "additionalSOLibSearchPath": "${workspaceFolder}/lib",
            
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]            
        }
    ]
}
```
8. make sure you have makefile tools extension installed.
9. In extension editor, select launch target  `"${workspaceFolder}/trick_sims/Cannon/SIM_cannon_analytic/S_main_Linux_11.4_x86_64.exe"`
10. Open "Run&Debug" menu.
11. select your launch in this case mine was "(gdb) Launch SimExe"
12. place your break points all over your model source, or trick sources.
13. Finally, click the Start Debugging(F5) button.

This is how I have managed to debug trick source and my own model sources. I hope it works for you guys as well. If trick people approve I can turn this answer into tutorial. 