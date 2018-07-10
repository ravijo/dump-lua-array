require "array"

-- caution: turn ONLY one case to true at a time
local case_1 = false
local case_2 = false
local case_3 = false
local case_4 = false
local case_5 = false

local case_number = tonumber(arg[1])
if (case_number == 1) then
  case_1 = true
elseif (case_number == 2) then
  case_2 = true
elseif (case_number == 3) then
  case_3 = true
elseif (case_number == 4) then
  case_4 = true
elseif (case_number == 5) then
  case_5 = true
else
  print(string.format("usage: lua %s case_number", arg[0]))
  os.exit()
end

-- define number of elements in array
local n = 2*800000

-- number of samples to collect for taking average
local count = 10000

-- we are going to generate random numbers below
math.randomseed(os.time())

if case_1 then
  -- ############################ CASE 1 ############################ ---
  -- we create array using 'array.new' and dump directly
  local a = array.new(n) -- array of size 'n' created

  -- set elements of the array
  for i=1,n do
    array.set_element(a, i, math.random())
  end

  local avg_time = 0
  for i=1,count do
    local start_time = os.clock()
    -- dump to a file
    array.dump(a, "out.bin")
    local end_time = os.clock()
    avg_time = avg_time + (end_time - start_time)
  end
  print(string.format("[Case 1] elapsed time: %.4f", (avg_time/count)))
  -- ################################################################ ---

elseif case_2 then
  -- ############################ CASE 2 ############################ ---
  -- in reality the Lua array is returned from an external API
  -- hence we have a conventional Lua array which we want to dump

  local function dump_conventional(c, file)
    -- make a copy of input array
    local n = #c
    local a = array.new(n)
    for i=1,n do
      array.set_element(a, i, c[i])
    end

    -- dump it now
    array.dump(a, file)
  end

  -- generate a random conventional Lua array of size 800,000
  local my_array = {}
  for i=1,n do
    my_array[i] = math.random()
  end

  local avg_time = 0
  for i=1,count do
    local start_time = os.clock()
    dump_conventional(my_array, "out.bin")
    local end_time = os.clock()
    avg_time = avg_time + (end_time - start_time)
  end
  print(string.format("[Case 2] elapsed time: %.4f", (avg_time/count)))
  -- ################################################################ ---

elseif case_3 then
  -- ############################ CASE 3 ############################ ---
  -- in previous case, we were making copy of input here, however
  -- in this case we are going to dump conventional array directly
  -- without making any copy
  local my_array = {}
  for i=1,n do
    my_array[i] = math.random()
  end

  local avg_time = 0
  for i=1,count do
    local start_time = os.clock()
    array.dump(my_array, "out.bin")
    local end_time = os.clock()
    avg_time = avg_time + (end_time - start_time)
  end
  print(string.format("[Case 3] elapsed time: %.4f", (avg_time/count)))
  -- ################################################################ ---

elseif case_4 then
  -- ############################ CASE 4 ############################ ---
  -- standard Lua IO
  local my_array = {}
  for i=1,n do
    my_array[i] = math.random()
  end

  local avg_time = 0
  for i=1,count do
    local start_time = os.clock()
    local file = io.open('out.txt','w')
    for i=1,n do
      file:write(my_array[i]..'\n')
    end
    file:close()
    local end_time = os.clock()
    avg_time = avg_time + (end_time - start_time)
  end
  print(string.format("[Case 4] elapsed time: %.4f", (avg_time/count)))
  -- ################################################################ ---

elseif case_5 then
  -- ############################ CASE 5 ############################ ---
  -- improved formulation

  -- create array of 'n' random elements
  local my_array = {}
  for i=1,n do
    my_array[i] = math.random()
  end

  local function dump_conventional(tbl, file)
    local arr = array.new(tbl)
    array.dump(arr, file)
  end

  local avg_time = 0
  for i=1,count do
    local start_time = os.clock()
    dump_conventional(my_array, "out.bin")
    --local f = assert(io.open("out.bin", "wb"))
    --f:write(array.to_string(fast_array))
    --assert(f:close())
    local end_time = os.clock()
    avg_time = avg_time + (end_time - start_time)
  end
  print(string.format("[Case 5] elapsed time: %.4f", (avg_time/count)))
else
  print('All cases are disabled. Please make sure ONLY one case is enabled.')
end

--[=====[
OLD RESULTS
  [Case 1] elapsed time: 0.01
  [Case 2] elapsed time: 0.15
  [Case 3] elapsed time: 0.03
  [Case 4] elapsed time: 3.26

Ordered in increasing elapsed time: Case 1 < Case 3 < Case 2 < Case 4

NEW RESULTS
  [Case 1] elapsed time: 0.0087
  [Case 2] elapsed time: 0.1417
  [Case 3] elapsed time: 0.0343
  [Case 4] elapsed time: 3.1897
  [Case 5] elapsed time: 0.0367

NOTE
Case 1 is the best in which we are creating array using 'array.new' function
HOWVER, in reality the Lua array is returned from an external API which is
a conventional Lua array
--]=====]
