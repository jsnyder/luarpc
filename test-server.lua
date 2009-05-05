
function fn_exists (funcname)
	return (type (getglobal (funcname)) == "function");
end


function foo1 (a,b,c)
	io.write ("this is foo1 ("..a.." "..b.." "..c..")\n");
	return 456;
end


function foo2 (tab)
	io.write ("this is foo2 ".. tab.a .. "\n");
	return 11,22,33;
end


-- this function will fail
function foo3 (tab)
	blah();
end


io.write ("server started\n")

RPC_server (12345);


-- an alternative way

--count = 0;
--handle = RPC_listen (12345);
--while 1 do
--	if RPC_peek (handle) then
--		io.write ("dispatch\n")
--		RPC_dispatch (handle)
--	else
--		io.write ("do dee do " .. count .. "...\n")
--	end
--	count = count + 1;
--end
