#include "LuaQuaternion.h"

// Lua includes.
#include "LuaContext.h"
#include "LuaVector3.h"

// GLM includes.
#include <glm/gtc/quaternion.hpp>

void LuaGameObjects::LuaQuaternion::Register(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // Push a new table to be used for static quaternion functions.
    int quaternionTable = luaContext->PushNewTable();

    // Add the constructors to the table.
    luaContext->Push(createNewFromXYZW);
    luaContext->SetField("fromXYZW", quaternionTable);

    luaContext->Push(createNewFromEuler);
    luaContext->SetField("fromEuler", quaternionTable);

    // Set the global quaternion table, but leave it on the stack.
    luaContext->CopyFrom(quaternionTable);
    luaContext->SetGlobal(QUATERNIONTYPENAME);

    // Push a new metatable to be used for quaternions.
    int metatable = luaContext->PushNewMetaTable(QUATERNIONTYPENAME);

    // Set up the operations.
    luaContext->Push(toString);
    luaContext->SetField(Lua::TOSTRINGNAME, metatable);

    luaContext->Push(multiply);
    luaContext->Push(multiply);
    luaContext->SetField(Lua::MULTIPLYNAME, metatable);
    luaContext->SetField("multiply", quaternionTable);

    // Create a metatable for the quaternion table so certain parts can be disabled.
    int quaternionMetatable = luaContext->PushNewTable();

    // Disable changing the table.
    luaContext->PushReadOnlyErrorFunction();
    luaContext->SetField(Lua::NEWINDEXERNAME, quaternionMetatable);

    // Set the metatable.
    luaContext->SetMetaTable(quaternionTable);

    // Pop the metatable and quaternion table from the stack.
    luaContext->Remove(metatable);
    luaContext->Remove(quaternionTable);

    // Stop balancing.
    luaContext->StopBalancing();
}

void LuaGameObjects::LuaQuaternion::CreateOnStack(std::shared_ptr<Lua::LuaContext> luaContext, glm::quat quaternion)
{
    // Create the userdata and set its value.
    *(glm::quat*)luaContext->PushUserData(sizeof(glm::quat)) = quaternion;
    int quaternionData = luaContext->GetTopIndex();

    // Set the metatable for the data.
    luaContext->GetMetaTable(QUATERNIONTYPENAME);
    luaContext->SetMetaTable(quaternionData);
}

int LuaGameObjects::LuaQuaternion::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // The first argument should be a quaternion.
    glm::quat left = *(glm::quat*)luaContext->CheckUserData(1, QUATERNIONTYPENAME);

    // The name of the property itself should be the second argument.
    std::string propertyName = luaContext->CheckString(2);

    // If the property name is only a single character, switch on it.
    if (propertyName.length() == 1)
    {
        switch (propertyName[0])
        {
        case 'x':
            luaContext->Push(left.x);
            break;
        case 'y':
            luaContext->Push(left.y);
            break;
        case 'z':
            luaContext->Push(left.z);
            break;
        case 'w':
            luaContext->Push(left.w);
            break;
        default:
            luaContext->PushNil();
            break;
        }
    }
    // Otherwise; if the property is for euler angles, push it.
    else if (propertyName == EULERNAME)
        LuaGameObjects::LuaVector3::CreateOnStack(luaContext, glm::eulerAngles(left));
    // Finally; return nil if none of these are true.
    else luaContext->PushNil();

    return 1;
}

int LuaGameObjects::LuaQuaternion::setIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // The first argument should be a quaternion.
    glm::quat* left = (glm::quat*)luaContext->CheckUserData(1, QUATERNIONTYPENAME);

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
            left->x;
            break;
        case 'y':
            left->y;
            break;
        case 'z':
            left->z;
            break;
        case 'w':
            left->w;
            break;
        default:
            luaContext->PushNil();
            break;
        }
    }

    return 0;
}

int LuaGameObjects::LuaQuaternion::toString(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // There should be one argument.
    int argumentCount = luaContext->GetTopIndex();
    if (argumentCount != 1) luaContext->Error("tostring operation was expecting a single quaternion argument, instead was given %d arguments", argumentCount);

    // The first argument should be a quaternion.
    glm::quat left = *(glm::quat*)luaContext->CheckUserData(1, QUATERNIONTYPENAME);

    // Push the string onto the stack and return.
    luaContext->Push(std::string("{ x: " + std::to_string(left.x) + " y: " + std::to_string(left.y) + " z: " + std::to_string(left.z) + " w: " + std::to_string(left.w) + " }"));
    return 1;
}

int LuaGameObjects::LuaQuaternion::createNewFromXYZW(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // There should be four arguments.
    int argumentCount = luaContext->GetTopIndex();
    if (argumentCount != 4)  luaContext->Error("constructor was expecting 4 numerical arguments, instead was given %d arguments", argumentCount);

    // Create a quaternion with all 4 arguments and push it onto the stack.
    CreateOnStack(luaContext, glm::quat(luaContext->CheckFloat(1), luaContext->CheckFloat(2), luaContext->CheckFloat(3), luaContext->CheckFloat(4)));
    return 1;
}

int LuaGameObjects::LuaQuaternion::createNewFromEuler(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // There should be one argument.
    int argumentCount = luaContext->GetTopIndex();
    if (argumentCount != 1) luaContext->Error("constructor was expecting a single vector argument, instead was given %d arguments", argumentCount);

    // The first argument should be a vector3.
    glm::vec3 eulerAngles = *(glm::vec3*)luaContext->CheckUserData(1, VECTOR3TYPENAME);

    // Create a quaternion with the euler angles and push it onto the stack.
    CreateOnStack(luaContext, glm::quat(eulerAngles));
    return 1;
}

int LuaGameObjects::LuaQuaternion::multiply(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // There should be two arguments.
    int argumentCount = luaContext->GetTopIndex();
    if (argumentCount != 2)  luaContext->Error("multiply operation was expecting left and right arguments, instead was given %d arguments", argumentCount);

    // The first argument should be a quaternion.
    glm::quat left = *(glm::quat*)luaContext->CheckUserData(1, QUATERNIONTYPENAME);

    // If the second argument is a number, use that.
    glm::quat result;
    if (luaContext->IsDouble(2))
        result = left * luaContext->ToFloat(2);
    // Otherwise; if the second argument is another quaternion, use that.
    else if (luaContext->IsUserData(2))
        result = left * *(glm::quat*)luaContext->CheckUserData(2, QUATERNIONTYPENAME);
    // Otherwise; cause an error.
    else return luaContext->Error("cannot multiply quaternion with %s", luaContext->GetType(2).c_str());

    // Push the result onto the stack and return 1.
    CreateOnStack(luaContext, result);
    return 1;
}
