# AntiSS

AntiSS is a program made to hide almost any window from screen recordings and screen shots! It utilizes Samuel Tulach's [windowhide](https://github.com/SamuelTulach/windowhide) code to hide any external window, making use of the SetWindowDisplayAffinity function from the ```winuser.h``` header.

## Compiling

Use Visual Studio 2022 or newer and compile this source as x64 in Release. Alternatively you can download the precompiled version from the releases tab. Note that the TrueTypeFont File I used has to be in the same folder as the executable so the ImGui Menu is shown properly, I was too lazy to merge the Font into the Sourcecode (which is possible). 

## What is this?

Originally, this Project was named "fuckveyon" because the teachers at our school always use [veyon](https://github.com/veyon/veyon) to spy on us when we use the school's laptops or PCs. So I decided to create such a program, giving the ability to play random ass games at school without the teacher seeing it on their veyon control panel. After I already knew the WDA_EXCLUDEFROMCAPTURE flag existed, I only had to find a way to apply it on any Window, not just the child windows from the process (as seen in [this](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowdisplayaffinity) Microsoft article). After I found out about SamuelTulach's very useful repo, I quickly implemented it into my code and created a little [ImGui](https://github.com/ocornut/imgui) Menu, so it everyone in our school was able to use it with ease.

It also changes the windowtitle, so if you hide your desired window, it will be renamed to "Microsoft Word". Please consider this project as abandoned and I probably wont update it again, I made this source almost a year ago. Also dont mind the messy code, I tried to clean everything up as good as possible and even commented some lines.

## Enjoy using it!
