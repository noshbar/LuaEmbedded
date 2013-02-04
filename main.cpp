/* A very simple example on how to call Lua functions from C, and vice versa. */

#include <stdio.h>
#include <stdlib.h>
#include "lua.hpp"

/* This is our game object structure, specifying all the properties that may pertain to an
   object instance in our game. For this example, only the position is important. */
typedef struct GameObject
{
	int x;
	int y;
}
GameObject;

/* This function is called from Lua via GameObject.move to move our object instance.
   In Lua-land it passes 3 parameters: the instance; and x and y. */
static int GameObject_move(lua_State *Lua)
{
	GameObject *instance   = (GameObject*)lua_topointer(Lua, 1);
	const int   x          = lua_tointeger(Lua, 2);
	const int   y          = lua_tointeger(Lua, 3);

	/* adjust the position of the instance and print out that we've done something. */
	instance->x += x;
	instance->y += y;
	printf("GameObject_move(%d, %d)\n\tinstance->x = %d\n\tinstance->y = %d\n", x, y, instance->x, instance->y);

	return 0;
}

/* This is called from C-land and simply serves as a helper function to set up the update
   function and call it. */
static void scriptOnUpdate(lua_State *Lua, GameObject *Instance)
{
	/* fetch the Lua function and make it the thing currently on the stack */
	lua_getglobal(Lua, "onUpdate");
	/* push the first argument onto the stack, which is the instance we are currently updating */
	lua_pushlightuserdata(Lua, Instance);
	/* call the function on the stack telling it that it has 1 parameter and no return values */
	lua_call(Lua, 1, 0);
}

/* This sets up the Lua context and registers our functions with it. */
static lua_State* createLuaContext()
{
	/* Build up an array of functions we want to expost for the "GameObject" namespace. */
	/* Add whatever functions you want to expose to this array. */
	const luaL_Reg gameObjectFunctions[] =
	{
		{ "move", GameObject_move }, /* Lua GameObject.move() maps to GameObject_move() in C-land */
		{ 0, 0 }                     /* NULL terminator to signal the end of the array */
	};

	/* This is the Lua script we plan on running, it would probably be in a resource file somewhere. */
	/* scriptOnUpdate() above calls this function, which in turn calls the GameObject_move() function above */
	const char *script = 
		"function onUpdate(instance)\n" \
		"    GameObject.move(instance, 4, 2)\n" \
		"end\n";

	lua_State *lua = luaL_newstate();      /* Creates a new context, lua_open replacement in versions > 5 */
	luaL_openlibs(lua);                    /* Adds the default Lua libraries to the context */
	luaL_newlib(lua, gameObjectFunctions); /* Adds our functions to the context */
	lua_setglobal(lua, "GameObject");      /* Assigns a namespace to our most recently added functions */
	luaL_loadstring(lua, script);          /* Loads our script into the context */
	lua_call(lua, 0, 0);                   /* This "primes" our context, without this, things will fail! */

	return lua;
}

int main(int argc, char *argv[])
{
	/* This is our game instance that we intend on passing to the Lua script for it to have its way with. */
	GameObject instance;
	instance.x = 6;
	instance.y = 8;

	lua_State *lua = createLuaContext();      /* Set up the lua context */

	/* while (game.running) */
	{
		/* For every game object instance do: */
		{
			scriptOnUpdate(lua, &instance);   /* Call the Lua script "onUpdate" function passing our current object instance */
		}
		/* Do other game related things */
	}

	lua_close(lua);                           /* ... and we're done. clean up shop and go home! */

	printf("\nPress return to quit.\n");
	getchar();
	return 0;
}
