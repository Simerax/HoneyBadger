# HoneyBadger
Unnecessary Programming Language just for the hell of it.

_no you don't need to care_

### In case you care
* The Language is far from complete even simple scripts might segfault or have [insert lexer, parser, codegenerator or whatever other]-errors
* The language is _functional_ so don't expect loops or mutable variables
* You need llvm-3.8 installed
* Right now everything is a double, no strings or custom types so far
* So far only run on linux, don't expect that this even compiles under mac or windows


#### Getting started
1. get llvm-3.8 
   * something like `apt-get install llvm-3.8` should do the trick (at least on debian based distros)
2. run `./build.sh`
3. run `./run.sh first.hb`
   * if you got some output that looks reasonable: congratulations you successfully ran a unnecessary program 👍

#### Oh, wow, your still reading?
You have problems with the language or repo? open an issue! 👷


_oh and Yes, you will probably find memory leaks_
