# F(edusiv)editor

UNDER DEVELOPMENT! DO NOT EXPECT ANYTHING FROM IT NOW

Feditor is GUI, sdl based fully keyboard controllable text editor.

I like neovim and vscode, but want to be this more arranged for my willings and needs

That's why I want to combine neovim and vscode:
* To have easy on-fly understandings how to use it.
* To have fully keyboard only control, but do not avoid mouse using as alternative sometimes.
* To have nice and understable configuration.

Currently I'm continuing to work on it and looking for nice contributors

## For building
to install sdllib:

sdl2

sdl2_ttf

On mac:
```
brew install sdl2
brew install sdl2_ttf
```

On wsl2 and linux:

apt-get has older version of sdl2 lib. And seems it won't be updated. You can use homebrew as in OSX, but you need to configure CMake paths correctly, which I failed to did.

I sugget to download latest sdl2 lib, build and install from sources. Tested. It will work

To enable GUI from WSL2 use this
https://aalonso.dev/blog/how-to-use-gui-apps-in-wsl2-forwarding-x-server-cdj

I'm not suggesting to using wsl2, it has few problems with graphics, but you can.

Newest SDL2 can be obtain via homebrew(brew), but after that it has issue with linking glibc and using pthread.

Project has cmake scripts to search for sdl libs.
