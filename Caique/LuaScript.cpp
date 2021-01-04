#include "LuaScript.h"

// Lua includes.
#include "LuaContext.h"

// Exception includes.
#include <exception>

Lua::LuaScript::~LuaScript()
{
    if (luaContext != nullptr)
    {
        // Retrieve the environments table.
        if (!luaContext->RetrieveFromRegistry(ENVIRONMENTSTABLENAME)) throw std::exception("Could not retrieve environments table.");
        int environmentsTable = luaContext->GetTopIndex();

        // Remove this environment from the table and invalidate any pointers and indices.
        luaContext->RemoveFrom(environmentsTable, environmentID);
        luaContext->Remove(environmentsTable);
        luaContext = nullptr;

        environmentID = Lua::INVALIDREFERENCE;
    }
}

int Lua::LuaScript::SaveInstanceFunction(std::shared_ptr<Lua::LuaContext> luaContext, const std::string& luaSource)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // Retrieve the functions table.
    if (!luaContext->RetrieveFromRegistry(FUNCTIONTABLENAME)) throw std::exception("Could not retrieve functions table.");
    int functionsTable = luaContext->GetTopIndex();

    // Compile the given source into bytecode so that it leaves a function on the stack.
    if (!luaContext->LoadString(luaSource)) throw std::exception("Lua source could not be compiled. Error message has been logged.");

    // Add the function to the functions table and save the ID.
    int functionID = luaContext->AddTo(functionsTable);

    // Pop the functions table off the stack.
    luaContext->Remove(functionsTable);

    // Stop balancing.
    luaContext->StopBalancing();

    // Return the function ID.
    return functionID;
}

std::shared_ptr<Lua::LuaScript> Lua::LuaScript::CreateInstance(std::shared_ptr<LuaContext> luaContext, const int functionID)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // Retrieve the environments table, add the environment to it, then remove the environments table from the stack.
    if (!luaContext->RetrieveFromRegistry(ENVIRONMENTSTABLENAME)) throw std::exception("Could not retrieve environments table.");
    int environmentsTable = luaContext->GetTopIndex();

    // Create the environment table for the script, and set the metatable for it.
    int environmentTable = luaContext->PushNewTable();
    luaContext->GetMetaTable(SCRIPTINSTANCENAME);
    luaContext->SetMetaTable(environmentTable);

    // Add the environment table to the main table and remove the main table from the stack.
    int environmentID = luaContext->AddTo(environmentsTable);
    luaContext->Remove(environmentsTable);

    // Stop balancing.
    luaContext->StopBalancing();

    // Create the lua script with the IDs.
    return std::shared_ptr<Lua::LuaScript>(new LuaScript(luaContext, environmentID, functionID));
}

void Lua::LuaScript::Register(std::shared_ptr<Lua::LuaContext> luaContext)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // Create a metatable to hold the script instance behaviours.
    int metaTable = luaContext->PushNewMetaTable(SCRIPTINSTANCENAME);

    // Get "_G" (one underscore), which is the Lua state's global context. This will then be on the top of the stack, so set the "__index" field of the metatable to it.
    // This essentially means that if the script's environment table is indexed and a value is not found, it will fall back onto the global context.
    luaContext->GetGlobal(Lua::GLOBALTABLENAME);
    luaContext->SetField(Lua::INDEXERNAME, metaTable);

    // Remove the metatable from the stack.
    luaContext->Remove(metaTable);

    // Create a new table to hold instance functions and place it in the registry.
    int functionTable = luaContext->PushNewTable();
    if (!luaContext->AddToRegistry(FUNCTIONTABLENAME))
        throw std::exception("Could not register function table.");

    // Create a new table to hold environments and place it in the registry.
    int environmentTable = luaContext->PushNewTable();
    if (!luaContext->AddToRegistry(ENVIRONMENTSTABLENAME)) throw std::exception("Could not register environment table.");

    // Stop balancing.
    luaContext->StopBalancing();
}

void Lua::LuaScript::Setup()
{
    // Retrieve the functions table.
    if (!luaContext->RetrieveFromRegistry(FUNCTIONTABLENAME)) 
        throw std::exception("Could not retrieve functions table.");
    int functionsTable = luaContext->GetTopIndex();

    // Retrieve the script function from the functions table, pop the functions table, then get the stack index of the function.
    luaContext->RetrieveFrom(functionsTable, functionID);
    luaContext->Remove(functionsTable);
    int instanceFunction = luaContext->GetTopIndex();

    // Get the environment for this script.
    int environment = GetEnvironment();

    // LuaJit uses Lua 5.1, which does not have the "_ENV" variable. Instead, the environment is set via this function.
    if (!luaContext->SetEnv(instanceFunction)) throw std::exception("Could not set environment of function.");

    // Run the instance function so that it fills its environment table with fields.
    if (!luaContext->CallFunction(0, 0)) throw std::exception("Failed to run lua script.");
}

bool Lua::LuaScript::HasFunction(const std::string& name)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // If the query has already been cached, use the cached value.
    bool isFunction = false;
    std::map<std::string, bool>::iterator foundItem = cachedFunctionQueries.find(name);
    if (foundItem != cachedFunctionQueries.end()) isFunction = (*foundItem).second;
    // Otherwise; query the function.
    else
    {
        // Get the field with the given name.
        int function = GetEnvironmentField(name);

        // Save the result of this query.
        isFunction = luaContext->IsFunction(function);
        cachedFunctionQueries.emplace(name, isFunction);

        // Remove the function from the stack.
        luaContext->Remove(function);
    }

    // Stop balancing.
    luaContext->StopBalancing();
    
    // Return the boolean value.
    return isFunction;
}

bool Lua::LuaScript::RunFunctionUsingStack(const std::string& name, int argumentCount)
{
    // Get the function with the given name.
    GetEnvironmentField(name);

    // So normally, the arguments will be on the top of the stack, and directly under will be the function itself. 
    // However, this is awkward to program safely, so instead the user pushes everything onto the stack then calls this function, and it automatically moves the function into the correct place, shifting the arguments.
    luaContext->MoveTop(-(argumentCount + 1));

    // If the field is a function, call it.
    return luaContext->IsFunction(-(argumentCount + 1)) && luaContext->CallFunction(argumentCount, 0);
}

bool Lua::LuaScript::RunFunction(const std::string& name)
{
    // Start balancing.
    luaContext->BeginBalancing();

    // Retrieve the function.
    int function = GetEnvironmentField(name);

    // If the field is a function, call it.
    if (luaContext->IsFunction(function)) { bool succeeded = luaContext->CallFunction(0, 0); luaContext->StopBalancing(); return succeeded; }
    else { luaContext->Remove(function); luaContext->StopBalancing(); return false; }
}

int Lua::LuaScript::GetEnvironment()
{
    // Retrieve the environments table.
    if (!luaContext->RetrieveFromRegistry(ENVIRONMENTSTABLENAME)) throw std::exception("Could not retrieve environments table.");
    int environmentsTable = luaContext->GetTopIndex();

    // Get the environment table for this script.
    luaContext->RetrieveFrom(environmentsTable, environmentID);

    // Remove the environments table from the stack and return.
    luaContext->Remove(environmentsTable);
    return luaContext->GetTopIndex();
}

int Lua::LuaScript::GetEnvironmentField(const std::string& name)
{
    // Get the environment table.
    int environmentTable = GetEnvironment();

    // Get the field from this table.
    luaContext->GetField(name, environmentTable);

    // Remove the environment table from the stack and return.
    luaContext->Remove(environmentTable);
    return luaContext->GetTopIndex();
}

void Lua::LuaScript::SetEnvironmentField(const std::string& name, const int stackIndex)
{
    // Get the environment and push it down below the existing stack item.
    GetEnvironment();
    luaContext->MoveTop(-2);
    
    // Set the field.
    luaContext->SetField(name, -2);

    // Pop the environment from the stack.
    luaContext->Remove(-1);
}