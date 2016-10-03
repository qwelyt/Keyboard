# Keyboard
I'm building my own keyboard.
It's as simple as that.

Why? Because I want to. And I've yet to find a keyboard that looks the way I want it to. The solution seems to be to build your own. So that's what I'm doing.

## Layout
The layout is a split keyboard with the keys in a matrix. Here's a pic
![layout](https://github.com/qwelyt/Keyboard/blob/master/Design/Layout/layout.png)

### Why split?
Every keyboard I've tried have always forced you to put your hands inwards, in a / \ way (pretend that's your arms). But I want to be able to keep my arms like | | or even \ /. With a split keyboard where you can move each module to where you want it to be you can achive this. I *think* this would give you better ergonomics, but we'll see. We might be using the one-module-fits-all approach for more than just cheaper-to-produce reasons. But if you never try, you never know.

### Why matrix layout?
As far as I've found, the only reason we use keyboards with slanted columns is because 'it has always been like that'. Like, from the typewriter-era. And then it was for mechanical reasons. But now that we no longer have metal arms that must be physically fit into the layout, we can design it just as we want. And apparently the slanted-columns-layout is bad for you. You stretch your fingers in an un-natural way which puts strain on your limbs and increases the odds of injury.
That is just bad. So I'm going for a matrix-layout because I like my fingers. They should have the best typing experiance they can have.


## Parts
This build is my first keyboard that I'm building from scratch. I've done a few mods, but never a complete build.
To build this I'm using some mechanical switches (Black ALPS) that I 'rescued' from a to-be-thrown Dell AT101. I've got 104 keys from that old keyboard. Most of which still have the click sound. And I have the caps, ofc.

The modules I will probably 3D print when I get my hands on a 3D printer.

I'm going for a NKRO, so I'll be using a lot of diods to get the matrix working. Pics will show up as work progresses as this is somewhat of a work-log.

I'm using an Arduino Uno atm because that what I had laying around. For the final build I will probably go with a Teensy or Arduino Mini.

Each module will be it's own keyboard. Meaning, each module will have it's own controller. So you don't need to plug in A to be able to use B and vice versa. This ofc means that, for 3 modules, you need 3 USB ports. So I will probably build a USB hub as well. Makes for a cleaner setup as you will only need 1 cable going to the computer. Might try building in retractable cables in everything, but well see how much of a pain that is.

# Log
Todo
* Get diods
* Print test module
* Assemble test module
* Code test module
* Get real controller board
* Refactor
* Print module A
* Print module B
* Assemble A
* Assemble B
* Code A
* Code B
* ???
* Profit

Doing
* Get hands on a 3D printer

Done
* Create basic working matrix
* Create 3D skotches for modules

