# VPN Manager API CPP

Stack: c++20, userver, postgres

# How to dev

## How to run in VS Code

- Install extensions:
    - clangd
    - DevContainers
    - RemoteSSH

- F1 -> Dev Containers: Open Folder in Container...
- Choose docker-compose
- When opened in dev container, delete all build* folders

In terminal:
- `apt-get install clangd && apt update -y && apt upgrade -y`

Set password for root:
- `passwd root`, `1234`

Then set password for user:
- `passwd user`, `1234`
- `sudo usermod -aG sudo user` - make user a sudoer
- `su -s /bin/bash user`

Specify the clangd path to the compile_commands.json file. To do this, add the following to the .vscode/settings.json config:
"clangd.arguments": [ "--compile-commands-dir=${workspaceFolder}/build_debug" ],
The compile_commands.json file is a service file that CMake creates during project configuration. The directory can be useful if you are, for example, building a release.

Restart VSCode and open the project directory again via devcontainers. Don't forget to switch to the user again using the method described above. Now you can build the project and run it from the vscode terminal:
make build-debug / make service-start-debug. Autocompletion and syntax highlighting should also work.

**To run:** make build-debug