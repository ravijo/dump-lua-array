#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>

typedef struct FloatArray {
  int size;
  float values[1];
} FloatArray;

static FloatArray *create_array(lua_State *L, int n) {
  // number of bytes to allocate a block of memory
  size_t nbytes = sizeof(FloatArray) + (n - 1) * sizeof(float);
  FloatArray *a = (FloatArray *)lua_newuserdata(L, nbytes);
  a->size = n;
  return a;
}

static int to_string(lua_State *L) {
  FloatArray *a = (FloatArray *)lua_touserdata(L, 1);
  luaL_argcheck(L, a != NULL, 1, "array expected");
  // convert the values into string and push it to the stack
  lua_pushlstring(L, (const char *)a->values, a->size * sizeof(FloatArray));
  return 1;
}

static void write_file(const char *file_name, lua_State *L, float *values,
                       int size) {
  // initialize binary file
  FILE *f = fopen(file_name, "wb");
  luaL_argcheck(L, f != NULL, 2, "can't open file");

  // write the array into binary file
  fwrite(values, sizeof(float), size, f);

  // make sure the data has been written successfully
  if (fclose(f) != 0)
    luaL_error(L, "can't close file %s", file_name);
}

static int dump_float_array(lua_State *L) {
  FloatArray *a = (FloatArray *)lua_touserdata(L, 1);
  luaL_argcheck(L, a != NULL, 1, "array expected");

  // get the input file name
  const char *file_name = luaL_checkstring(L, 2);

  // write values to a binary file
  write_file(file_name, L, a->values, a->size);
  return 1;
}

static int new_array_from_table(lua_State *L) {
  luaL_argcheck(L, lua_type(L, 1) == LUA_TTABLE, 1, "1D array expected");

  // get the length of input table
  int n = lua_objlen(L, 1);

  // create new userdata
  FloatArray *a = create_array(L, n);

  // fetch table data and use it to fill the userdata
  int index;
  for (index = 1; index <= n; index++) {
    lua_rawgeti(L, 1, index);
    a->values[index - 1] = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
  }
  return 1;
}

static int dump_table(lua_State *L) {
  luaL_argcheck(L, lua_type(L, 1) == LUA_TTABLE, 1, "array expected");

  // get the length of input table
  int n = lua_objlen(L, 1);

  // fetch table data and use it to fill the array
  int index;
  float values[n];
  for (index = 1; index <= n; index++) {
    lua_rawgeti(L, 1, index);
    values[index - 1] = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
  }

  // get the input file name
  const char *file_name = luaL_checkstring(L, 2);

  // write values to a binary file
  write_file(file_name, L, values, n);
  return 1;
}

static int set_element(lua_State *L) {
  FloatArray *a = (FloatArray *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  float value = luaL_checknumber(L, 3);

  luaL_argcheck(L, a != NULL, 1, "array expected");
  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  // set the value at given index
  // index starts from 0 in C where as it starts from 1 in Lua
  a->values[index - 1] = value;
  return 1;
}

static int get_element(lua_State *L) {
  FloatArray *a = (FloatArray *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);

  luaL_argcheck(L, a != NULL, 1, "array expected");
  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  lua_pushnumber(L, a->values[index - 1]);
  return 1;
}

static int get_size(lua_State *L) {
  FloatArray *a = (FloatArray *)lua_touserdata(L, 1);
  luaL_argcheck(L, a != NULL, 1, "array expected");
  lua_pushnumber(L, a->size);
  return 1;
}

static int dump_array(lua_State *L) {
  switch (lua_type(L, 1)) {
  case LUA_TUSERDATA:
    return dump_float_array(L);
  case LUA_TTABLE:
    return dump_table(L);
  default:
    luaL_argcheck(L, 0, 1, "expected 1D array or FloatArray");
  }
  return 0;
}

static int new_array(lua_State *L) {
  switch (lua_type(L, 1)) {
  case LUA_TNUMBER:
    create_array(L, luaL_checkint(L, 1));
    return 1;
  case LUA_TTABLE:
    return new_array_from_table(L);
  default:
    luaL_argcheck(L, 0, 1, "expected number or 1D array");
  }
  return 0;
}

static const struct luaL_reg array_lib[] = {{"new", new_array},
                                            {"set_element", set_element},
                                            {"get_element", get_element},
                                            {"size", get_size},
                                            {"to_string", to_string},
                                            {"dump", dump_array},
                                            {NULL, NULL}};

int luaopen_array(lua_State *L) {
  luaL_openlib(L, "array", array_lib, 0);
  return 1;
}
