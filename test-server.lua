require("rpc")

function fn_exists (funcname)
   return type(_G[funcname]) == "function"
end


function foo1 (a,b,c)
   io.write ("this is foo1 ("..a.." "..b.." "..c..")\n");
   return 456;
end


function foo2 (tab)
   io.write ("this is foo2 ".. tab.a .. "\n");
   return 11,22,33;
end


function execfunc( fstring, input )
   func = loadstring(fstring)
   return func(input)
end

function execrfunc( func, input )
   print(func)
   return func(input)
end

-- this function will fail
function foo3 (tab)
   blah();
end

function mirror( input )
   return input
end


yarg = {}

test = {1, 2, 3, 4, "234"}

test.sval = 23

io.write ("server started\n")

-- rpc.server ("/dev/ptys0"); -- use for serial mode
-- rpc.server ("/dev/ptmx"); -- use for serial mode

if rpc.mode == "tcpip" then
   io.write("TCP/IP Server Started\n")
   rpc.server(12346);
elseif rpc.mode == "serial" then
   io.write("Serial Server Started\n")
   rpc.server("/dev/ptys0");
end

-- an alternative way

-- count = 0;
-- handle = rpc.listen ("/dev/ptys0");
-- while 1 do
--   if rpc.peek (handle) then
--     io.write ("dispatch\n")
--     rpc.dispatch (handle)
--   else
--     io.write ("do dee do " .. count .. "...\n")
--   end
--   count = count + 1;
-- end
