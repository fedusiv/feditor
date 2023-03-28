# F(edusiv)editor

Feditor is GUI, raylib based fully keyboard controllable text editor.

I like neovim and vscode, but want to be this more arranged for my willings and needs

That's why I want to combine neovim and vscode:
* To have easy on-fly understandings how to use it.
* To have fully keyboard only control, but do not avoid mouse using as alternative sometimes.
* To have nice and understable configuration.

Currently I'm continuing to work on it and looking for nice contributors

## For building
You need to install sdllib:

sdl2

sdl2_ttf

On mac:
```
brew install sdl2
brew install sdl2_ttf
```

On wsl2:
```
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-dev
```
To enable GUI from WSL2 use this
https://aalonso.dev/blog/how-to-use-gui-apps-in-wsl2-forwarding-x-server-cdj

WSL2 on some reasons of apt-get package manager has old SDL2 version.

Newest SDL2 can be obtain via homebrew(brew), but after that it has issue with linking glibc and using pthread.

Project has cmake scripts to search for sdl libs.
