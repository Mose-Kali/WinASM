# WinASM
WinASM(Or WSM,Windows ASM Langauge) Project is an unserious project.With this code,you can make a joke with a simple(or hard,it depends on you :-) ) script file on your friend's computer.It will allow the script and pop message windows,image windows,vedio windows,play background music,execuse other programs.It's easy to use.You only need about 200KB to save its runtime enviroment.\n
Let's learn something about it!

### How to install it?
Download the latest release package from release page,run it and it will extra all files in the folder that you choosed.The coreful files are WinASMExplainer.exe,WSMEncoder.dll and WindowsExLib.dll.
### How to create my script files?
WSM script can be any txt file,but you must allow its grammar or it will not runs currently.
You can create a txt file and edit it with notepad,notepad++ even Idea or Visual Studio(all depand on you),then rename it as any name that you want.
### How to run my scripts?
Drop your script file into the WinASMExplainer.exe execuseable file,it will run the script autoly,or use command line,the argument is your script file's path.
### Cool!And what about Lrc2WSM.exe?
It's a program decode lrc files and then encode them as WSM1.0 scripts.Then add ADO command at the start,you can see some windows with lyirces pop with background music.
### It must be fun,but why WSM1.0 scripts?By the way can I run WSM2.0 or WSM3.0 on 1.0 enviroment or 3.0 enviroment?
Er...No,you can only run WSM script on target enviroment.But I'll try to do it.Give me some time :-).
### What a prity!And BatLauncher.exe in v.3.0.1's pack?
When you run it,it will read the Launch.lch file in current folder,and runs commands in the file line by line.You can create a shortcut link to BatLauncher.exe on desktop and change its icon,now you just need to wait your frind click it!
### How can I copy them to my friends computer?
You just need copy WinASMExplainer.exe,WSMEncoder.dll,WindowsExLib.dll and your scripts,youcan run it everywhere!
### Said so much where is the grammar?
Scroll down...
## Grammar
### Variable Operation(3.0 Support only)
#### Define
Garmmar:[Type] [Name] [InitValue]
Type can be string,int,double,char
Name can be any name without space.
#### Operators
##### ${ValueName}
Quoting the value of ValueName.
##### ${ValueA}=${ValueB}
Move ValueB's value to ValueA.
##### ADD ${A} ${B} ${C}
A add B equals to C.
##### SUB ${A} ${B} ${C}
A sub B equals to C.
##### MUL ${A} ${B} ${C}
A mul B equals to C.
##### DIV ${A} ${B} ${C}
A div B equals to C.
##### DIVI ${A} ${B} ${C}
A % B equals to C.
##### COS ${A} ${B}
B=cos(A)
##### SIN ${A} ${B}
B=sin(A)
##### TAN ${A} ${B}
B=tan(A)
##### POW ${A} ${B} ${C}
C=A^B
##### SQRT ${A} ${B}
B=sqrt(A)
##### CMP ${A} ${B}
If A=B,goto next command,or jump over next command
##### LOG ${A} ${B}
B=log(A)
##### RAND ${A}
A=rand()
### Controls
##### JMP ${N}(3.0 Only)
Jump to the number N of commands from next command.
##### FUNC ${x^4} ${x^3} ${x^2} ${x} ${c} ${RangeMin} ${RangeMax} ${Offset} ${Timewait}(2.0 & 3.0)
It's a brick command last to the last END command.
It will enmu all value from RangeMin to RangeMax,each loop add Offset.
y=${x^4}*x^4+${x^3}*x^3+${x^2}*x^2+${x^1}*x+${c}
(3.0)The value of x and the value of y will add in value list and name as "__FUNC__N__X"(Function result),"__FUNC__N__Y"(Function result),"__FUNC__X"(Screen Point),"__FUNC__Y"(Screen Point),you can use them as value by $ operator.
(2.0)Use \# symbol to replease the value as x or y(On x value's palace will replace as x value,y as well.Only can use in MSG command)
Then it will sleep for Timewait millseconds before next loop
##### FUNC ${x^4} ${x^3} ${x^2} ${x} ${c} ${RangeMin} ${RangeMax} ${Offset} ${Msg} ${Title} ${Type} ${BoxTimewait} ${Timewait}(1.0)
It will enmu all value from RangeMin to RangeMax,each loop add Offset.
y=${x^4}*x^4+${x^3}*x^3+${x^2}*x^2+${x^1}*x+${c}
It will pop a messagebox with Msg as message and Title as box title with a type.The messagebox will wait BoxTimewait millseconds and desappear.Type as same as MSG command's type parameter.
Then it will sleep for Timewait millseconds before next loop
##### END
END command is the end symbol of the script or a code brick.
##### SLEP ${Time}
Wait Time millseconds before next command
### Windows Commands
##### MSG ${Text} ${Title} ${x} ${y} ${Type} ${Timewait}(3.0 Supports $ operator,2.0 Supports # operator,1.0 only supports const value)
It will pop a messagebox with Msg as message and Title as box title with a type on point (x,y).The messagebox will wait Timewait millseconds and desappear.
Types:1(Error Icon),2(Infomation Icon),3(Attention Icon),4(Question Icon)
##### IMG ${x} ${y} ${Timewait} ${ImgPath}
It will load a bmp image(BMP file only!!!)and show it in a dialog on point(x,y).It will disappear when Timewait millseconds past.
### Media Commands
##### ADO ${MusicPath}
It will play music in background,wav files only.
##### VDO ${x} ${y} ${cx} ${cy} ${Timewait} ${VedioPath}
It will play vedio in a vedio dialog on point(x,y) whitch height is cy and width is cx.It will disappear when Timewait millseconds past.
### System Commands
##### CMD ${Program}
It will execuse a program(full path or name).

---------
Here are the grammar have fun! :-)
## And Remember:NO ZUO NO DIE!!!
