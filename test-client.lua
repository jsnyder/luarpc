require("luarpc")

function error_handler (message)
	io.write ("MY ERROR: " .. message .. "\n");
end


io.write ("client started\n")

xxx = nil;

tab = {a=1.4142, b=2};
-- tab.foo = tab;		-- bad

function doStuff()
	-- set the default error handler for all handles
	rpc_on_error (error_handler);
	io.write ("error set\n")

	local slave,err = rpc_open_tcp ("localhost",12345);
	if not slave then
		io.write ("error: " .. err .. "\n");
		exit();
	end

	io.write ("connected\n")

	-- set the error handler for a specific handle
	--rpc_on_error (slave,error_handler);

	-- trigger some errors
	slave.a_bad_function (1,2,3,4,5);

	slave.foo3();

	ret = slave.foo1 (123,3.14159,"hello");
	io.write ("return value = " .. ret .. "\n");

	-- slave.exit (0);		-- trigger socket error at next call

	slave.foo2 (tab);
	
	print (slave.testvar)
	
	function squareval(x) 
		print(x^2)
		return x^2
	end
	
	print(slave.execfunc( string.dump(squareval), 8 ))
	
	if not slave.fn_exists ("blah") then
		io.write ("blah does not exist\n");
	else
		slave.blah();
	end

	print("test")
	
	xxx = slave.foo2;
	xxx (tab);

	slave.print ("hello there\n");

	rpc_close (slave);

	-- RPC_call (slave,"a");		-- should trigger an error

end


doStuff();