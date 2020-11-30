//extern "C"
//{
//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
//}
//
//#ifdef _WIN32
//#pragma comment(lib, "../Libraries/Lua/lua51.lib")
//#endif
//
//#undef luaL_dostring
//#define luaL_dostring(L,s)	\
//	(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
//
//std::string cmd = "a = 7 + 11 + math.sin(50)";
//
//lua_State* L = luaL_newstate();
//luaL_openlibs(L);
//
//int r = luaL_dostring(L, cmd.c_str());
//
//if (!r)
//{
//	lua_getglobal(L, "a");
//	if (lua_isnumber(L, -1))
//	{
//		float a_in_cpp = (float)lua_tonumber(L, -1);
//		std::cout << a_in_cpp << std::endl;
//	}
//}
//else
//{
//	std::string errormsg = lua_tostring(L, -1);
//	std::cout << errormsg << std::endl;
//}
//
//
//lua_close(L);