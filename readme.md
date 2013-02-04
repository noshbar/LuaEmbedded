From http://iwasdeportedandalligotwasthislousydomain.co.uk/index.php?entry=entry130131-152758 :

#Purpose

I tried to find a simple example that showed me:
* How to call some Lua script from within C
* How to call a C function from within the Lua script
* How to pass an instance of a structure/class to the script and have it call functions on it

Being naive I thought finding something like this would be easy. We all know how stories starting with "Being naive..." end (in road-side ditches surrounded by police tape).
I used the latest Lua release (5.2) and stumbled across errors where functions were deprecated (lua_open no longer exists), examples where there was so much going on I simply couldn't understand the basics, examples where a script.lua file was simply executed, examples where in order to pass a C object over I had to create something called a MetaTable and and...

So here it is, a simple example that pretends it's a game. The idea would be that you have an enemy object and it's intelligence would be stored in a Lua script that is loaded at the start of the game, and the "onUpdate" function in that script is called every frame wherein it can change the position, state, colour, happiness, whatever of the object.

#Note

This contains no error checking to keep things simple. The Lua documentation on how to do error checking is great, and you should check the result of every call you make, as well as checking to see if variables are actually the type you expect them to be before using them.
Also, I started using Lua for the first time today, so this is simply an example that works for me, your mileage and joy in ignorance may vary.
