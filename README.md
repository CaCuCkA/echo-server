
# ECHO Server

❯ CMake

❯ GCC

❯ Boost

❯ Git

❯ C++ IDE (optional). CLion is recommended.

> **Note**
>
> CMake and compiler is a mandatory requirements. If your CMake version is incompatible with the project,
> please, consider updating it. For more information, please, refer to the official CMake documentation.
> However, if you are using macOS or Linux, you can use the following command to install the latest version of CMake:
>
> ```bash
> macOS
> $ brew install cmake
> ```
>
> ```bash
> Ubuntu
> $ sudo apt install cmake
> ```
>
> ```bash
> Arch based distros
> $ sudo pacman -S cmake
> ```
>
> For compiler installation, please, refer to the official documentation of your compiler.


## Compilation

1. You should compile our project run the `compile.sh` file like this:
```bash
$ ./compile.sh -c # compile project
$ ./compile.sh -r # download requirements
```
2. If you want to get rid of executable files, run the `compile.sh file like this:
```bash
$ ./compile.sh --clean
```

## Installation

To install our project, you need to clone the repository first:
```bash
$ mkdir ~/workdir
$ cd ~/workdir
$ git clone https://github.com/CaCuCkA/echo-server.git
$ cd echo-server
```

> Replace `~/workdir` with the path to your workspace.

## Important 
> **Note**
> 
>To run this project first of all you should build it with `compile.sh` script. After that run in <mark>TWO CONSOLES</mark>
> server and python script. For example:
> ```bash
> $ ./sync_server
> ```
> ```bash
> $ time python3 auto_runner.py
> ```
> 
> After python script will finish, you should close server session on your compute `Ctrl + C`.
> 
## Results
* Async Server Results:
```bash
real    36m51.981s
user    32m0.084s
sys     9m27.448s
```

* Sync Server Results:
```bash
real    38m5.389s
user    32m53.829s
sys     10m36.177s
```

for this function 
```python
async def echo():
    for x in range(Constants.RANGE):
        async with websockets.connect(Constants.URL) as websocket:
            await websocket.send(str())
            name = await websocket.recv()
            print(name, end=" ", flush=True)
            sys.stdout.flush()
```
