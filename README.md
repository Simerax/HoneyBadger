# HoneyBadger
Unnecessary Programming Language just for the hell of it.

_no you don't need to care_

### In case you care
* The Language is far from complete even simple scripts might segfault or have [insert lexer, parser, codegenerator or whatever other]-errors
* The language is _functional_
* You need llvm-6.0 installed
* Right now everything is a double, no strings or custom types so far
* So far it only runs on linux, don't expect that this even compiles under mac or windows


#### Getting started
2. get llvm-6.0, cmake & clang
   * something like `apt-get install llvm-6.0 cmake clang` should do the trick (at least on debian based distros)
3. run `./build.sh`
4. run `./run.sh first.hb`
   * if you got some output that looks reasonable: congratulations you successfully ran a unnecessary program 👍

#### Oh, wow, your still reading?
You have problems with the language or repo? open an issue! 👷


_oh and Yes, you will probably find memory leaks_
