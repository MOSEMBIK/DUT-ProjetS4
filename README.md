# Bomberman sous OpenGL
## A propos
Petit projet visant à réaliser un jeu Bomberman sous OpenGL en C++

![](https://i.ytimg.com/vi/mvKBQQxtBWM/maxresdefault.jpg)
(fig. 1 - Bomberman World)

## Modalité de réalisation
Le projet doit être réalisé en 8 semaines. Il est possible d'utiliser des assets (tel que des images ou des librairies,...) pour la réalisation du projet.

## Les membres 
- RIMBERT Bastien
- COLLIGNION Alexandre
- KHADDAN Mohammad
- JEANNIN Lionel
- ROSA Ruben
- 🎃🐺 ρᎥ🅴ⓡⓇ€ 𝕓🅾ｕⓇ 😲💀

# Comment compiler et lire le projet
## Sous Windows
1. Installer MSys64
2. Lancer msys2.exe dans `C:\msys64\msys2.exe` puis écrire :
3. Pour installer MinGW:
>pacman -S mingw-w64-x86_64-gcc  
>pacman -S mingw-w64-x86_64-gdb
4. Pour installer Make :  
>pacman -S make
5. L'utilisation de VSCode est recommandé puisque les taches sont préconfigurées :
6. Aller dans `Terminal > Run Task > Build` ou `Build & Run`
7. Ouvrir l'executable compilé dans `build/Game.exe` 

*Il est possible que l'ajout à la variable d'environment "PATH" du dossier `MSys64/MGWin64/bin` et `MSys64/user/bin` soit nécessaire*
