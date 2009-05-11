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
	RPC_on_error (error_handler);

	local slave,err = RPC_open ("localhost",12345);
	if not slave then
		io.write ("error: " .. err .. "\n");
		exit();
	end

	-- set the error handler for a specific handle
	--RPC_on_error (slave,error_handler);

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

	RPC_close (slave);

	-- RPC_call (slave,"a");		-- should trigger an error

end


doStuff();