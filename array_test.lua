require "array"

-- define number of elements in array
local n = 1000000

local my_array = {}
for i=1,n do
  my_array[i] = i+1
end

array.dump(my_array, "out.bin")

local arr = array.new(my_array)
array.dump(arr, "out.bin")

i=1
print(string.format("size of arr:%d, my_array[%d]:%d, arr[%d]:%d", array.size(arr), i, my_array[i], i, array.get_element(arr, i)))

local a = array.new(n) -- array of size 'n' created

-- set elements of the array
for i=1,n do
  array.set_element(a, i, math.random())
end

-- dump to a file
array.dump(a, "out.bin")
