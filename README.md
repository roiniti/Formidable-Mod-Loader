# Formidable-Mod-Loader
## Building 
If you want to build it yourself you have the build instructions in the main.c file
# For azur lane devs 
This project is intened to enchance the game with things like BetterFPS mod or to load translations to languages which Azur Lane doesn't support right now like spanish, if you have trouble with cheaters (which I don't support and allow), implement more defenses on the server side because every cliend side patch to prevent this can be avoided.
If having reasoned this you still want me to take down the project please join the [discord server](https://discord.gg/Wg3pNmmS5J), and DM me @roiniti.




> [!NOTE]
> If you have Azur Lane version 5.1-7.x its recomended to use the version 0.1.1 of the modloader


A mod loader for Azur Lane, if you want some mods, or learn a bit of the mod structure you can check [my mods repo](https://github.com/roiniti/Azur-Lane-Mods/tree/main) or you can join the [discord server](https://discord.gg/Wg3pNmmS5J)  
Also I want to thank Egoist (https://github.com/Egoistically) for helping me with this project (also I'm using code from one of his projects)
> [!NOTE]
> ~~Only works for arm64-v8a!!, I'm working on a armeabi-v7a and x86 release~~
> Armeabi-v7a and x86 is implemented >=0.1.1 but not tested

> [!IMPORTANT]
> This is NOT INTENDED to hack/cheat the game, my goal was to implement QOF mods (like BetterFPSControl) or something like that, I do not support hackers and cheaters and I am not responsible for improper use of this tool

> [!WARNING]
> This is a modification of the game and can lead to a ban, but some mods should be harmless, use it at your own risk

# Adding mods
After you install FormidableML create a folder named mods inside Android/data/com.YostarEN.AzurLane/files and put inside all the mods folder you want

## Installing FormidableML using the autopatcher
1. Download the library zip and AzurLane-Autopatcher.zip from [here](https://github.com/roiniti/AZLAutopatcher)
2. Extract AzurLane-Autopatcher.zip
3. In the same folder as AZLAutopatcher.exe put the Azur Lane apk and inside a folder named libs put the arm64-v8a armeabi... folders
4. Open a terminal/cmd/powershell in the folder AZLAutopatcher.exe -a "apk-name"
5. Install the apk and then (if you hadn't) add the obb files

## Installing FormidableML yourself
1. Download ApkTool https://apktool.org/
2. Download zipalign and apksigner
3. Create a keystore
4. Extract the Azur Lane apk using apktool
```
java -jar apktool.jar d ./azur-lane.apk -o extracted/apk -f
```
5. Add the following method to `smali_classes2\com\unity3d\player\UnityPlayerActivity` or `smali\com\unity3d\player\UnityPlayerActivity` (depends on the AL version), anywhere above its `onCreate`:
```smali
.method private static native init(Landroid/content/Context;)V
.end method
```
And these lines to `onCreate`:
```smali
	const-string v0, "FormidableML"

	invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

	invoke-static {p0}, Lcom/unity3d/player/UnityPlayerActivity;->init(Landroid/content/Context;)V
```
(Preferably without replacing other variables, such as between `.locals 2` and `const/4 v0, 0x1`.)  
  
Also you can enable debug mode (you'll need debug mode on verson 0.1.0, in later versions its recomended), go to AndroidManifest.xml and add `android:debuggable="true"` after application, should look like this:
```xml
<application android:debuggable="true"...
```
6. Put the libFormidableML inside `lib/arm64-v8a`/`lib/armeabi-v7a`/`lib/x86`
7. Rebuild the apk
```
java -jar apktool.jar b extracted/apk -o extracted/recompiled.apk -f
```
8. Align the apk and sign it
```
zipalign.exe -f 4 extracted/recompiled.apk extracted/aligned.apk
java -jar apksigner.jar sign -ks my-release-key.keystore -ks-pass pass:password --v1-signing-enabled true --v2-signing-enabled true extracted\aligned.apk
```
9. Install the apk
