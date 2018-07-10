# dump-lua-array
Dump 1-dimensional Lua array into a file efficiently

Environmental Details
---------------------
1. Ubuntu 14.04 LTS 64 Bit OS
1. Lua v5.1
1. GCC v4.8.4

Install Lua v5.1
-----------------
1. Open terminal or press <kbd>CTRL</kbd>+<kbd>ALT</kbd>+<kbd>T</kbd> then use following command-
```
sudo apt-get install lua5.1
```
2. Install Lua headers by following command-
```
sudo apt-get install liblua5.1-0-dev
```

Compilation
-------------
Before proceding further make sure to download the code from the repository.
1. Open terminal or press <kbd>CTRL</kbd>+<kbd>ALT</kbd>+<kbd>T</kbd>
1. Compile C code using the following command-
```
gcc -I/usr/include/lua5.1 -o array.so -shared array.c -fPIC
```
The above command is going to produce shared object. 

Read files
-------------
1. Compile C code using the following command-
```
gcc -o read read.c
```
2. Execute `read` from terminal

Usage
-----
We can use the above shared object in Lua code as shown below-
```
-- add the shared object to our Lua code
-- make sure to keep 'array.so' along with this code
require "array"

-- define number of elements in our array
local n = 1000000

-- create array
local my_array = {}
for i=1,n do
  my_array[i] = i+1
end

-- now we have an array called 'my_array', 
-- which we want to dump into a file. there
-- are two ways to do it

-- case 1
-- create new array first
-- then call dump function
local arr = array.new(my_array)
array.dump(arr, "out.bin")

-- case 2
-- directly call dump function
array.dump(my_array, "out.bin")

-- if we use 'array.new', we can do much more as shown below
local i=1
print(string.format("size of arr:%d, my_array[%d]:%d, arr[%d]:%d", array.size(arr), i, my_array[i], i, array.get_element(arr, i)))
```
Check out my blog post [here](https://ravijo.github.io/lua/) for more information about it.
