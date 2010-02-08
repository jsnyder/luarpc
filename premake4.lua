-- A solution contains projects, and defines the available configurations
solution "LuaRPC"
   configurations { "serial", "socket" }
   platforms { "native" }
 
   -- A project defines one build target
   project "LuaRPC"
      kind "SharedLib"
      language "C"
      defines { "DEBUG", "LUARPC_STANDALONE", "BUILD_RPC" }
      flags { "Symbols" }
      links { "lua" }
      files { "**.h", "luarpc.c", "luarpc_socket.c", "luarpc_serial.c" }
      targetname "rpc"
      targetprefix ""
         
      configuration "serial"
        defines { "LUARPC_ENABLE_SERIAL" }

      configuration "socket"
        defines { "LUARPC_ENABLE_SOCKET" }
        
      configuration "not windows and serial"
        files { "serial_posix.c" }

      configuration "macosx"
        targetextension ".so"

      configuration "windows and serial"
        files { "serial_win32.c" }
        defines { "WIN32_BUILD" }