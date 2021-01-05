#include "LuaVector3.h"

// Lua includes.
#include "LuaContext.h"

void LuaGameObjects::LuaVector3::Register(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Push a new table to be used for static vector3 functions.
	int vectorTable = luaContext->PushNewTable();

	// Add the constructor to the table.
	luaContext->Push(createNew);
	luaContext->SetField("new", vectorTable);

	// Add any static operations to the table.
	luaContext->Push(distance);
	luaContext->SetField(DISTANCENAME, vectorTable);

	luaContext->Push(normalise);
	luaContext->SetField(NORMALISENAME, vectorTable);
	// Take into account that some people spell this function wrong, register both spellings.
	luaContext->Push(normalise);
	luaContext->SetField(NORMALIZENAME, vectorTable);

	luaContext->Push(dot);
	luaContext->SetField(DOTNAME, vectorTable);

	// Set the global vector3 table, but leave it on the stack.
	luaContext->CopyFrom(vectorTable);
	luaContext->SetGlobal(VECTOR3TYPENAME);

	// Push a new metatable to be used for vectors.
	int metatable = luaContext->PushNewMetaTable(VECTOR3TYPENAME);

	// Set up the operations.
	luaContext->Push(toString);
	luaContext->SetField(Lua::TOSTRINGNAME, metatable);

	luaContext->Push(add);
	luaContext->Push(add);
	luaContext->SetField(Lua::ADDNAME, metatable);
	luaContext->SetField("add", vectorTable);

	luaContext->Push(subtract);
	luaContext->Push(subtract);
	luaContext->SetField(Lua::SUBTRACTNAME, metatable);
	luaContext->SetField("subtract", vectorTable);

	luaContext->Push(multiply);
	luaContext->Push(multiply);
	luaContext->SetField(Lua::MULTIPLYNAME, metatable);
	luaContext->SetField("multiply", vectorTable);

	luaContext->Push(divide);
	luaContext->Push(divide);
	luaContext->SetField(Lua::DIVIDENAME, metatable);
	luaContext->SetField("divide", vectorTable);

	luaContext->Push(length);
	luaContext->Push(length);
	luaContext->SetField(LENGTHNAME, metatable);
	luaContext->SetField(LENGTHNAME, vectorTable);

	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, metatable);

	luaContext->Push(setIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, metatable);

	// Add some vector constants to the vector table, since it was impossible to do so before defining what a vector is.
	CreateOnStack(luaContext, glm::vec3(0, 1, 0));
	luaContext->SetField(UPNAME, vectorTable);
	CreateOnStack(luaContext, glm::vec3(0, -1, 0));
	luaContext->SetField(DOWNNAME, vectorTable);
	CreateOnStack(luaContext, glm::vec3(-1, 0, 0));
	luaContext->SetField(LEFTNAME, vectorTable);
	CreateOnStack(luaContext, glm::vec3(1, 1, 0));
	luaContext->SetField(RIGHTNAME, vectorTable);
	CreateOnStack(luaContext, glm::vec3(0, 0, 1));
	luaContext->SetField(FORWARDNAME, vectorTable);
	CreateOnStack(luaContext, glm::vec3(0, 0, -1));
	luaContext->SetField(BACKWARDNAME, vectorTable);

	// Create a metatable for the vector table so certain parts can be disabled.
	int vectorMetatable = luaContext->PushNewTable();

	// Disable changing the table.
	luaContext->PushReadOnlyErrorFunction();
	luaContext->SetField(Lua::NEWINDEXERNAME, vectorMetatable);

	// Set the metatable.
	luaContext->SetMetaTable(vectorTable);

	// Pop the metatable and vector table from the stack.
	luaContext->Remove(metatable);
	luaContext->Remove(vectorTable);

	// Stop balancing.
	luaContext->StopBalancing();
}

void LuaGameObjects::LuaVector3::CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, glm::vec3 vector3)
{
	// Create the userdata and set its value.
	*(glm::vec3*)luaContext->PushUserData(sizeof(glm::vec3)) = vector3;
	int vectorData = luaContext->GetTopIndex();

	// Set the metatable for the data.
	luaContext->GetMetaTable(VECTOR3TYPENAME);
	luaContext->SetMetaTable(vectorData);
}

int LuaGameObjects::LuaVector3::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// If the property name is only a single character, switch on it.
	if (propertyName.length() == 1)
	{
		switch (propertyName[0])
		{
		case 'x':
		case 'r':
		case 's':
			luaContext->Push(left.x);
			break;
		case 'y':
		case 'g':
		case 't':
			luaContext->Push(left.y);
			break;
		case 'z':
		case 'b':
		case 'p':
			luaContext->Push(left.z);
			break;
		default:
			luaContext->PushNil();
			break;
		}
	}
	// Otherwise; if the name is distance, return the distance function.
	else if (propertyName == DISTANCENAME)
	{
		int vectorTable = luaContext->GetGlobal(VECTOR3TYPENAME);
		luaContext->GetField(DISTANCENAME, vectorTable);
		luaContext->Remove(vectorTable);
	}
	// Otherwise; if the name is length, return the length value.
	else if (propertyName == LENGTHNAME) luaContext->Push(glm::length(left));
	// Otherwise; if the name is normal, return the normal value.
	else if (propertyName == NORMALNAME) CreateOnStack(luaContext, glm::normalize(left));
	// Finally; return nil if none of these are true.
	else luaContext->PushNil();

	return 1;
}

int LuaGameObjects::LuaVector3::setIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be three arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 3)  luaContext->Error("set index was expecting 3 arguments, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3* left = (glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// The third argument should be a float.
	float value = luaContext->CheckFloat(3);

	// If the property name is only a single character, switch on it.
	if (propertyName.length() == 1)
	{
		switch (propertyName[0])
		{
		case 'x':
		case 'r':
		case 's':
			left->x = value;
			break;
		case 'y':
		case 'g':
		case 't':
			left->y = value;
			break;
		case 'z':
		case 'b':
		case 'p':
			left->z = value;
			break;
		}
	}

	return 0;
}

int LuaGameObjects::LuaVector3::toString(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be one argument.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 1) luaContext->Error("tostring operation was expecting a single vector argument, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// Push the string onto the stack and return.
	luaContext->Push(std::string("{ x: " + std::to_string(left.x) + " y: " + std::to_string(left.y) + " z: " + std::to_string(left.z) + " }"));
	return 1;
}

int LuaGameObjects::LuaVector3::createNew(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Create an empty vector.
	glm::vec3 newVec = glm::vec3(0, 0, 0);
	
	// Handle the different number of arguments.
	int argumentCount = luaContext->GetTopIndex();
	switch (argumentCount)
	{
	// If there are no arguments, do nothing with the vector.
	case 0: break;
	// If there is one argument, use it for all values.
	case 1:
	{
		float singleValue = luaContext->CheckFloat(1);
		newVec = glm::vec3(singleValue, singleValue, singleValue);
		break;
	}
	// If there are three arguments, use all of them.
	case 3:
		newVec = glm::vec3(luaContext->CheckFloat(1), luaContext->CheckFloat(2), luaContext->CheckFloat(3));
		break;
	// Otherwise; cause an error.
	default:
		return luaContext->Error("expected 0, 1, or 3 arguments, got %d instead", argumentCount);
	}

	CreateOnStack(luaContext, newVec);
	return 1;
}

int LuaGameObjects::LuaVector3::add(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("add operation was expecting left and right arguments, instead was given %d arguments", argumentCount);
	
	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// If the second argument is a number, use that.
	glm::vec3 result;
	if (luaContext->IsDouble(2))
		result = left + luaContext->ToFloat(2);
	// Otherwise; if the second argument is another vector, use that.
	else if (luaContext->IsUserData(2))
		result = left + *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);
	// Otherwise; cause an error.
	else return luaContext->Error("cannot add vector to %s", luaContext->GetType(2).c_str());
	
	// Push the result onto the stack and return 1.
	CreateOnStack(luaContext, result);
	return 1;
}

int LuaGameObjects::LuaVector3::subtract(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("subtract operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// If the second argument is a number, use that.
	glm::vec3 result;
	if (luaContext->IsDouble(2))
		result = left - luaContext->ToFloat(2);
	// Otherwise; if the second argument is another vector, use that.
	else if (luaContext->IsUserData(2))
		result = left - *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);
	// Otherwise; cause an error.
	else return luaContext->Error("cannot %s from vector", luaContext->GetType(2).c_str());

	// Push the result onto the stack and return 1.
	CreateOnStack(luaContext, result);
	return 1;
}

int LuaGameObjects::LuaVector3::multiply(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("multiply operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// If the second argument is a number, use that.
	glm::vec3 result;
	if (luaContext->IsDouble(2))
		result = left * luaContext->ToFloat(2);
	// Otherwise; if the second argument is another vector, use that.
	else if (luaContext->IsUserData(2))
		result = left * *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);
	// Otherwise; cause an error.
	else return luaContext->Error("cannot multiply vector with %s", luaContext->GetType(2).c_str());

	// Push the result onto the stack and return 1.
	CreateOnStack(luaContext, result);
	return 1;
}

int LuaGameObjects::LuaVector3::divide(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("divide operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// If the second argument is a number, use that.
	glm::vec3 result;
	if (luaContext->IsDouble(2))
		result = left / luaContext->ToFloat(2);
	// Otherwise; if the second argument is another vector, use that.
	else if (luaContext->IsUserData(2))
		result = left / *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);
	// Otherwise; cause an error.
	else return luaContext->Error("cannot divide vector by %s", luaContext->GetType(2).c_str());

	// Push the result onto the stack and return 1.
	CreateOnStack(luaContext, result);
	return 1;
}

int LuaGameObjects::LuaVector3::length(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be one argument.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 1) luaContext->Error("length operation was expecting a single vector argument, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);
	
	// Push the length onto the stack and return.
	luaContext->Push(glm::length(left));
	return 1;
}

int LuaGameObjects::LuaVector3::distance(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("distance operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

	// Both arguments should be vector3s.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);
	glm::vec3 right = *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);

	// Push the distance onto the stack and return.
	luaContext->Push(glm::distance(left, right));
	return 1;
}

int LuaGameObjects::LuaVector3::dot(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be two arguments.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 2)  luaContext->Error("dot operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

	// Both arguments should be vector3s.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);
	glm::vec3 right = *(glm::vec3*)luaContext->CheckUserData(2, VECTOR3TYPENAME);

	// Push the dot onto the stack and return.
	luaContext->Push(glm::dot(left, right));
	return 1;
}

int LuaGameObjects::LuaVector3::normalise(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// There should be one argument.
	int argumentCount = luaContext->GetTopIndex();
	if (argumentCount != 1) luaContext->Error("normalise operation was expecting a single vector argument, instead was given %d arguments", argumentCount);

	// The first argument should be a vector3.
	glm::vec3 left = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

	// Push the normal onto the stack and return.
	CreateOnStack(luaContext, glm::normalize(left));
	return 1;
}
