# GLCall(x);

- It's not a function at all, it's a define. It checks if any errors accured when running a gl specific function passed in the place of an 'x'.

- It's "argument" is the gl specific function, that it will run and check errors for.
- Example of it being used: GLCall(glGenBuffers(x,y));
