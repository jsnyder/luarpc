require("luarpc")

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

-- this function will fail
function foo3 (tab)
	blah();
end

testvar = 23


io.write ("server started\n")

rpc_server (12345);


-- an alternative way

--count = 0;
--handle = rpc_listen (12345);
--while 1 do
--	if rpc_peek (handle) then
--		io.write ("dispatch\n")
--		rpc_dispatch (handle)
--	else
--		io.write ("do dee do " .. count .. "...\n")
--	end
--	count = count + 1;
--end
