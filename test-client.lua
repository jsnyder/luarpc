require("luarpc")
require("showtable")

function error_handler (message)
	io.write ("MY ERROR: " .. message .. "\n");
end


io.write ("client started\n")

xxx = nil;

tab = {a=1.4142, b=2};
-- tab.foo = tab;		-- bad

function doStuff()
	-- set the default error handler for all handles
	rpc.on_error (error_handler);
	io.write ("error set\n")

	if rpc.mode == "tcpip" then
		slave,err = rpc.connect ("localhost",12345);
	elseif rpc.mode == "serial" then
		slave,err = rpc.connect ("/dev/ttys0");
	end

  
	-- local slave,err = rpc.connect ("/dev/ttys0");
	-- local slave,err = rpc.connect ("/dev/tty.usbserial-ftCYPMYJ");
	-- local slave,err = rpc.connect ("/dev/pts/4");
	if not slave then
		io.write ("error: " .. err .. "\n");
		exit();
	end

	io.write ("connected\n")

	-- set the error handler for a specific handle
	--rpc.on_error (slave,error_handler);

	-- trigger some errors
	slave.a_bad_function(1,2,3,4,5);

	slave.foo3();

	ret = slave.foo1 (123,3.14159,"hello");
	io.write ("return value = " .. ret .. "\n");

	-- slave.exit (0);		-- trigger socket error at next call

	slave.foo2 (tab);
		
	function squareval(x) return x^2 end
	
	print(slave.execfunc( string.dump(squareval), 8 ))
	
	if not slave.fn_exists ("blah") then
		io.write ("blah does not exist\n");
	else
		slave.blah();
	end
	
	xxx = slave.foo2;
	xxx (tab);

	slave.print ("hello there\n");
	
	val = slave.math.cos(2.1)
	
	print(val)
	
	
	testval = slave.test:get()

	print(table.show(testval,"testval"))
		
	slave.yarg.blug = {23}
	
	print( table.show(slave.yarg:get(), "slave.yarg") )
	
	slave.yurg = tab
	
	print( slave.execrfunc( squareval, 9 ) )
	
	slave.squareval = squareval

	print( table.show(slave.squareval:get(), "slave.squareval") )
	
	print(slave.squareval(99))
	
	-- function printglobals(x) for i,v in pairs(_G) do print(i,v) end end
	
	-- slave.execfunc( string.dump( printglobals ), nil)
	
	-- for i,v in pairs(testval) do print(i,v) end
	
	rpc.close (slave);
end


doStuff();
