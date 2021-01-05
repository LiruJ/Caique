#include "LuaContext.h"

// Output includes.
#include <iostream>

bool Lua::LuaContext::validateLua(int returnValue)
{
	// If the return code is invalid, print the error and return false.
	if (returnValue != Lua::OK)
	{
		// Get the error message from the top of the stack.
		std::string errorMessage = lua_tostring(luaState, -1);

		// Print the error message and output false.
		std::cout << "[LUA] " << errorMessage << std::endl;
		return false;
	}
	// Otherwise; return true.
	else return true;
}

int Lua::LuaContext::interceptFunctionCall(lua_State* calledState)
{
	// Get the context immediately and ensure it's valid.
	std::shared_ptr<Lua::LuaContext> luaContext = *(std::shared_ptr<Lua::LuaContext>*) lua_touserdata(calledState, lua_upvalueindex(1));

	if (luaContext->luaState != calledState)
		throw std::exception("Lua state mismatch.");

	// Get the lua function to be called.
	luaContextFunction luaFunction = *(luaContextFunction*)luaContext->ToUserData(lua_upvalueindex(2));

	// Call the function and return its output.
	return luaFunction(luaContext);
}

Lua::LuaContext::LuaContext() : balanceIndex(INVALIDSTACKCOUNT), namesToRegistryIndices(std::map<std::string, int>())
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
}

Lua::LuaContext::~LuaContext()
{
	if (luaState != nullptr)
	{
		lua_close(luaState);
		luaState = nullptr;
	}
}

int Lua::LuaContext::GetTopIndex()
{
	return lua_gettop(luaState);
}

void Lua::LuaContext::BeginBalancing()
{
	// Check that the stack is not already being balanced.
	if (balanceIndex != INVALIDSTACKCOUNT) throw std::exception("Two calls to BeginBalancing without any to StopBalancing.");

	// Set the balance index to the current stack count.
	balanceIndex = GetTopIndex();
}

void Lua::LuaContext::StopBalancing()
{
	// Check that the stack is being balanced.
	if (balanceIndex == INVALIDSTACKCOUNT) throw std::exception("Calls to StopBalancing without any call to BeginBalancing.");

	// Check that the stack is balanced.
	int endStackSize = GetTopIndex();
	if (balanceIndex != endStackSize)
		throw std::exception(std::string("Stack was left unbalanced. Started with " + std::to_string(balanceIndex) + " and ended with " + std::to_string(endStackSize) + ".").c_str());

	// Reset the balance index.
	balanceIndex = INVALIDSTACKCOUNT;
}

bool Lua::LuaContext::LoadFile(const std::string& filePath)
{
	return validateLua(luaL_loadfile(luaState, filePath.c_str()));
}

bool Lua::LuaContext::LoadString(const std::string& source)
{
	return validateLua(luaL_loadstring(luaState, source.c_str()));
}

void Lua::LuaContext::Push(const std::string& value)
{
	lua_pushstring(luaState, value.c_str());
}

void Lua::LuaContext::Push(const bool value)
{
	lua_pushboolean(luaState, value);
}

void Lua::LuaContext::Push(const int value)
{
	lua_pushinteger(luaState, value);
}

void Lua::LuaContext::Push(const double value)
{
	lua_pushnumber(luaState, value);
}

void Lua::LuaContext::Push(const luaContextFunction value)
{
	// Push the shared pointer to this context.
	std::shared_ptr<Lua::LuaContext>* thisUserdataPointer = (std::shared_ptr<Lua::LuaContext>*)PushUserData(sizeof(std::shared_ptr<Lua::LuaContext>));

	// Bear in mind that Lua has given us a raw chunk of random memory to store this shared pointer into. If we just do =, the assignment operator is called and C++ tries to treat this random block of memory as a shared pointer.
	// Instead, we use the placement operator to just force the data in there without any functions running.
	new(thisUserdataPointer) std::shared_ptr<Lua::LuaContext>(shared_from_this());

	// Store the pointer to the function to call.
	luaContextFunction* luaContextFunctionPointer = (luaContextFunction*)PushUserData(sizeof(luaContextFunction));
	*luaContextFunctionPointer = value;

	// Push the whole closure so that it calls the interception function.
	lua_pushcclosure(luaState, Lua::LuaContext::interceptFunctionCall, 2);
}

int Lua::LuaContext::PushNil()
{
	lua_pushnil(luaState);
	return GetTopIndex();
}

int Lua::LuaContext::PushReadOnlyErrorFunction()
{
	Push(readOnlyErrorFunction);
	return GetTopIndex();
}

int Lua::LuaContext::PushNewTable()
{
	lua_newtable(luaState);
	return GetTopIndex();
}

void* Lua::LuaContext::PushUserData(const int size)
{
	return lua_newuserdata(luaState, size);
}

int Lua::LuaContext::PushNewMetatable(const std::string& typeName)
{
	luaL_newmetatable(luaState, typeName.c_str());
	return GetTopIndex();
}

void Lua::LuaContext::SetMetatable(const int stackIndex)
{
	lua_setmetatable(luaState, stackIndex);
}

int Lua::LuaContext::GetMetaTable(const std::string& typeName)
{
	luaL_getmetatable(luaState, typeName.c_str());
	return GetTopIndex();
}

void Lua::LuaContext::CopyFrom(const int stackIndex)
{
	lua_pushvalue(luaState, stackIndex);
}

int Lua::LuaContext::MoveTop(const int stackIndex)
{
	lua_insert(luaState, stackIndex);
	return GetTopIndex();
}

void Lua::LuaContext::Remove(const int stackIndex)
{
	lua_remove(luaState, stackIndex);
}

int Lua::LuaContext::GetField(const std::string& name, const int stackIndex)
{
	lua_getfield(luaState, stackIndex, name.c_str());
	return GetTopIndex();
}

void Lua::LuaContext::SetField(const std::string& name, const int stackIndex)
{
	lua_setfield(luaState, stackIndex, name.c_str());
}

int Lua::LuaContext::GetGlobal(const std::string& name)
{
	lua_getglobal(luaState, name.c_str());
	return GetTopIndex();
}

void Lua::LuaContext::SetGlobal(const std::string& name)
{
	lua_setglobal(luaState, name.c_str());
}

void Lua::LuaContext::GetEnv(const int stackIndex)
{
	lua_getfenv(luaState, stackIndex);
}

bool Lua::LuaContext::SetEnv(const int stackIndex)
{
	lua_setfenv(luaState, stackIndex);
	return true;
}

std::string Lua::LuaContext::GetType(const int stackIndex)
{
	return std::string(lua_typename(luaState, lua_type(luaState, stackIndex)));
}

int Lua::LuaContext::AddTo(const int stackIndex)
{
	return luaL_ref(luaState, stackIndex);
}

bool Lua::LuaContext::AddToRegistry(const std::string& name)
{
	// If the map already contains this name, return false.
	if (namesToRegistryIndices.count(name) > 0) return false;

	// Save the object at the top of the stack and save the index.
	int registryIndex = luaL_ref(luaState, Lua::REGISTRYINDEX);
	
	// If the returned index is invalid, return false.
	if (registryIndex == INVALIDREFERENCE || registryIndex == NILREFERENCE) return false;

	// Add the index to the map along with the name and return true.
	namesToRegistryIndices.emplace(name, registryIndex);
	return true;
}

void Lua::LuaContext::RetrieveFrom(const int stackIndex, const int tableIndex)
{
	lua_rawgeti(luaState, stackIndex, tableIndex);
}

bool Lua::LuaContext::RetrieveFromRegistry(const std::string& name)
{
	// If the name has an associated index, use it.
	std::map<std::string, int>::iterator foundItem = namesToRegistryIndices.find(name);
	int registryIndex = foundItem == namesToRegistryIndices.end() ? INVALIDREFERENCE : (*foundItem).second;

	// If the index is invalid, return false.
	if (registryIndex == INVALIDREFERENCE || registryIndex == NILREFERENCE) return false;

	// Get the object then return true.
	lua_rawgeti(luaState, Lua::REGISTRYINDEX, registryIndex);
	return true;
}

void Lua::LuaContext::RemoveFrom(const int stackIndex, const int tableIndex)
{
	luaL_unref(luaState, stackIndex, tableIndex);
}

bool Lua::LuaContext::RemoveFromRegistry(const std::string& name)
{
	// If the name has an associated index, use it.
	std::map<std::string, int>::iterator foundItem = namesToRegistryIndices.find(name);
	int registryIndex = foundItem == namesToRegistryIndices.end() ? INVALIDREFERENCE : (*foundItem).second;

	// If the index is invalid, return false.
	if (registryIndex == INVALIDREFERENCE || registryIndex == NILREFERENCE) return false;

	// Unreference the object, remove the index from the map, then return true.
	luaL_unref(luaState, Lua::REGISTRYINDEX, registryIndex);
	namesToRegistryIndices.erase(name);
	return true;
}

bool Lua::LuaContext::SetUpValue(const int stackIndex, const int upValueIndex, const char*& upValueName)
{
	const char* t = lua_setupvalue(luaState, stackIndex, upValueIndex);

	return upValueName != NULL;
}

void Lua::LuaContext::Register(const std::string& libraryName, const luaL_Reg* methods)
{
	luaL_register(luaState, libraryName.c_str(), methods);
}

void Lua::LuaContext::Register(const luaL_Reg* methods)
{
	luaL_register(luaState, NULL, methods);
}

bool Lua::LuaContext::IsNil(const int stackIndex)
{
	return lua_isnil(luaState, stackIndex);
}

bool Lua::LuaContext::IsString(const int stackIndex)
{
	return lua_isstring(luaState, stackIndex);
}

bool Lua::LuaContext::IsTable(const int stackIndex)
{
	return lua_istable(luaState, stackIndex);
}

bool Lua::LuaContext::IsUserData(const int stackIndex)
{
	return lua_isuserdata(luaState, stackIndex);
}

bool Lua::LuaContext::IsUserData(const int stackIndex, const char* typeName)
{
	BeginBalancing();

	// Get the metatable for the type at the given index.
	if (!lua_getmetatable(luaState, stackIndex)) return false;
	int firstMetatable = GetTopIndex();

	// Get the metatable for the type with the given name.
	luaL_getmetatable(luaState, typeName);
	int secondMetatable = GetTopIndex();
	
	// Equality check.
	bool equal = lua_equal(luaState, firstMetatable, secondMetatable);

	// Clean up.
	Remove(secondMetatable);
	Remove(firstMetatable);

	StopBalancing();

	return equal;
}

bool Lua::LuaContext::IsFunction(const int stackIndex)
{
	return lua_isfunction(luaState, stackIndex);
}

bool Lua::LuaContext::IsDouble(const int stackIndex)
{
	return lua_isnumber(luaState, stackIndex);
}

void* Lua::LuaContext::ToUserData(const int stackIndex)
{
	return lua_touserdata(luaState, stackIndex);
}

std::string Lua::LuaContext::ToString(const int stackIndex)
{
	return lua_tostring(luaState, stackIndex);
}

const char* Lua::LuaContext::ToCString(const int stackIndex)
{
	return lua_tostring(luaState, stackIndex);
}

int Lua::LuaContext::ToInt(const int stackIndex)
{
	return lua_tointeger(luaState, stackIndex);
}

double Lua::LuaContext::ToDouble(const int stackIndex)
{
	return lua_tonumber(luaState, stackIndex);
}

std::string Lua::LuaContext::CheckString(const int stackIndex)
{
	return luaL_checkstring(luaState, stackIndex);
}

const char* Lua::LuaContext::CheckCString(const int stackIndex)
{
	return luaL_checkstring(luaState, stackIndex);
}

int Lua::LuaContext::CheckInt(const int stackIndex)
{
	return luaL_checkint(luaState, stackIndex);
}

double Lua::LuaContext::CheckDouble(const int stackIndex)
{
	return luaL_checknumber(luaState, stackIndex);
}

void* Lua::LuaContext::CheckUserData(const int stackIndex, const std::string& typeName)
{
	return luaL_checkudata(luaState, stackIndex, typeName.c_str());
}

bool Lua::LuaContext::CallFunction(const int argumentCount, const int returnValueCount)
{
	return validateLua(lua_pcall(luaState, argumentCount, returnValueCount, 0));
}

std::shared_ptr<Lua::LuaContext> Lua::LuaContext::Create()
{
	return std::shared_ptr<Lua::LuaContext>(new LuaContext());
}

int Lua::LuaContext::readOnlyErrorFunction(std::shared_ptr<Lua::LuaContext> luaContext)
{
	return luaContext->Error("attempt to write to a readonly table");
}
