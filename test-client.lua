require("luarpc")

function error_handler (message)
	io.write ("Err: " .. message .. "\n");
end

rpc.on_error (error_handler);

if rpc.mode == "tcpip" then
	slave, err = rpc.connect ("localhost",12345);
elseif rpc.mode == "serial" then
	slave, err = rpc.connect ("/dev/ttys0");
end

-- Local Dataset

tab = {a=1.4142, b=2};

test_local = {1, 2, 3, 3.143, "234"}
test_local.sval = 23

function squareval(x) return x^2 end


--
-- BEGIN TESTS
--

-- check that our connection exists
assert( slave, "connection failed" )

-- reflect parameters off mirror
assert(slave.mirror(42) == 42, "integer return failed")
-- print(slave.mirror("012345678901234")) -- why the heck does this fail for things of length 15 (16 w/ null)?
-- assert(slave.mirror("this is a test!") == "this is a test!", "string return failed")
-- print(slave.mirror(squareval))
-- assert(string.dump(slave.mirror(squareval)) == string.dump(squareval), "function return failed")
assert(slave.mirror(true) == true, "function return failed")

-- basic remote call with returned data
assert( slave.foo1 (123,3.14159,"hello") == 456, "basic call and return failed" )

-- execute function remotely
assert(slave.execfunc( string.dump(squareval), 8 ) == 64, "couldn't serialize and execute dumped function")

-- get remote table
assert(slave.test:get(), "couldn't get remote table")


-- check that we can get entry on remote table
assert(test_local.sval == slave.test:get().sval, "table field not equivalent")

slave.yarg.blurg = 23
assert(slave.yarg.blurg:get() == 23, "not equal")

-- function assigment
slave.squareval = squareval
assert(type(slave.squareval) == "userdata", "function assigment failed")

-- remote execution of assigned function
assert(slave.squareval(99) == squareval(99), "remote setting and evaluation of function failed")

rpc.close (slave);