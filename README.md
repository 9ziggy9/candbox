
# Table of Contents

1.  [Contents](#orgee0564a):TOC_4:
    1.  [DISCLAIMER](#org568d066)
    2.  [Preliminaries](#orga308dde)
        1.  [WTF do I need to install?](#org773aae5)
        2.  [Makefiles](#org73220fe)
        3.  [Building a C project](#org4859524)
    3.  [References](#orgee9ea9b)
        1.  [Beej's Guide to C Programming](#org6af3145)
        2.  [Beej's other guides](#org2e70ba0)
        3.  [K&R C](#org9269c81)



<a id="orgee0564a"></a>

# Contents     :TOC_4:

-   
-   -   -   
        -   
        -   
        -
    -   
    -
-   -   
    -   
    -


<a id="org568d066"></a>

## DISCLAIMER

> **DISCLAIMER**:  
> 
> The following is a set of notes on low-level system
> programming targetting programmers more accustomed
> to the higher-level world of interpreted languages.  
> 
> I am **NOT** an expert in C programming and I can barely
> follow any assembler language that is not nasm. This
> document may contain many incorrect statements but how
> about you and I go toe-to-toe on Bird Law and see who
> comes out the victor?  
> 
> <img src="./media/expert.gif" alt="lawyerings" />
> 
> Each day we've worked on this repo will be separated into
> branches, if you want to follow sequentially as this repo
> has evolved, select the branch by day, day-one being the
> branch corresponding to the state of the repo at the time
> of writing this message.  
> 
> Have fun and please feel free to absolutely roast me for
> any and all statements.  


<a id="orga308dde"></a>

## Preliminaries

We are not going to talk C at all until we cover some basic
UNIX OS concepts and discuss build tools. Even if you don't
care about C, this stuff might be useful.


<a id="org773aae5"></a>

### WTF do I need to install?

You need a standard compiler and *make*.  

You have either *gcc* or *clang* pre-installed on UNIX-like OSs,
so the only thing you will need is *make*.  

1.  Linux

    If it isn't installed by default then consult your distro's
    package docs. This should return something if it's installed:   
    
        which make

2.  macOS

        brew install make

3.  BSD

    As with Linux, this is going to depend on your BSD flavor
    but obviously if you are using BSD you probably shouldn't
    be reading this. If you use FreeBSD and you are somehow
    unaware, core *make* in FreeBSD (i.e. what you use for ports)
    is not the same thing as the commie GNU *make*
    that most people are familiar with. You would need to install
    [gmake](https://www.freshports.org/devel/gmake/) to be perfectly
    consistent with these notes but who cares?

4.  Windows

    The best starting place is to [install gentoo](https://upload.wikimedia.org/wikipedia/commons/2/28/Richard_Stallman_at_LibrePlanet_2019.jpg)
    first.  
    
    Okay fine, then you should probably use because
    literally nobody but game devs have the sanity or motivation
    necessary to learn C using Windows APIs.  
    
    If you ARE using WSL, you are most likely using Ubuntu:
    
        sudo apt-install make
    
    There is also [cygwin](https://www.cygwin.com/install.html) but I haven't
    the slightest idea how that works, so good luck.  


<a id="org73220fe"></a>

### Makefiles

**IMPORTANT**: You can't copy paste this code because Makefiles
are whitespace sensitive and this org file doesn't preserve
tabs. :(   

[Makefiles](https://en.wikipedia.org/wiki/Make_(software)) are the most
convenient build tool ever created. They have been
around for almost 50 years. You can use them for almost
anything. Start by creating a file titled *Makefile*
and give it a *target* "hello"

    hello:
      echo hello world

If you run this via *make*:

    $ make hello
    echo hello world
    hello world

That is, for a specified *target* the set of **tabbed lines**
directly beneath are the commands which will be ran (called a *recipe*)).
**Makefiles are tab sensitive!**
If you don't want to see the command you invoked, provide
a **@** symbol.

    hello:
      @echo hello world

    $ make hello
    hello world

You can provide any number of *targets*.

    hello:
      @echo hello world
    
    goodbye:
      @echo goodbye moon

    $ make hello
    hello world
    $ make goodbye
    goodbye moon

You can also provide any number of *recipes* to each target.

    hello:
      @echo hello world
      @echo hello earth
    
    goodbye:
      @echo goodbye moon
      @echo goodbye sun

    $ make hello
    hello world
    hello earth
    $ make goodbye
    goodbye moon
    goodbye sun

*Targets* can be composed with other *targets* as *dependencies*.
What this means is that the other *targets* specified to the 
direct right of the \`:\` symbol will be evaluated before the
indented *target* recipes fire.

    hello_goodbye: hello goodbye
      @echo all done
    
    hello:
      @echo hello world
    
    goodbye:
      @echo goodbye moon

    $ make hello_goodbye
    hello world
    goodbye moon
    all done

Incidentally, the top-most *target* is taken as a default value
if no *target* is given as an argument to *make*. **NOTE THAT THE**
**TARGET NAMES ARE COMPLETELY ARBITRARY AND THE TOP-MOST WILL**
**ALWAYS SERVE AS THE DEFAULT**:

    $ make
    hello world
    goodbye moon
    all done

Like shell scripts, we can bind identifiers to expressions. *make*
will literally inject these values wherever it encounters them within
*$()*. i.e.,

    HELLO=hello world
    GOODBYE=goodbye moon
    CAN_BE_TARGET_TOO_LOL=i literally dont matter
    
    $(CAN_BE_TARGET_TOO_LOL): hello goodbye
      @echo $(CAN_BE_TARGET_TOO_LOL)
    
    hello:
      @echo $(HELLO)
    
    goodbye:
      @echo $(GOODBYE)

    $ make
    hello world
    goodbye moon
    i literally dont matter

Sometimes in shell scripting we want the output of an evaluated
shell expression, for instance:

    $ echo today is $(date | awk -F: '{ print $1}')
    today is Thu Apr 4 01

Of course, this couldn't quite work in Makefile as is, how would
the parser distinguish between subtitution and evaluation? Solution:
just add another *$*:

    HELLO=hello world
    GOODBYE=goodbye moon
    CAN_BE_TARGET_TOO_LOL=i literally dont matter
    
    $(CAN_BE_TARGET_TOO_LOL): hello goodbye
      @echo $(CAN_BE_TARGET_TOO_LOL)
      @echo but at least its $$(date | awk -F: '{ print $1 }')
    
    hello:
      @echo $(HELLO)
    
    goodbye:
      @echo $(GOODBYE)

    $ make
    hello world
    goodbye moon
    i literally dont matter
    but at least its Thu Apr 4 01

That's enough for now, we're actually ready to start a C project.


<a id="org4859524"></a>

### Building a C project

Here is a boilerplate Makefile:

    CC=clang
    CFLAGS=-Wall -Wextra -pedantic -Wconversion \
    			 -Wunreachable-code -Wswitch-enum -Wno-gnu
    EXE=run
    
    all: main.c
    	$(CC) main.c -o $(EXE) $(CFLAGS)
    
    clean:
    	rm -rf $(EXE)

And at last, perhaps the simplest C program imaginable:

    int main(void) {
      return 0;
    }


<a id="orgee9ea9b"></a>

## References

There is an infinite supply of C programming resources
and I'll note a few here in order of what I feel is the
most helpful.  


<a id="org6af3145"></a>

### [Beej's Guide to C Programming](https://beej.us/guide/bgc/html/)

[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html)
is super famous but
this one is just as incredible in my opinion. I wish I had
been aware of thes guides' existence when I first started
writing C. Beej's writing style is incredibly easy to parse
and he has an incredible sense of what students of the C
language tend to struggle with. This guide can be read cover
to cover without any boredom or dullness arising.


<a id="org2e70ba0"></a>

### [Beej's other guides](https://beej.us/guide/)

As I mentioned before, Beej's most famous guide is the
Network Programming one but I might as well link his page.
My dude has a way of explaing the things.


<a id="org9269c81"></a>

### K&R C

Obviously&#x2026;

