#include "LuaGraphicsContext.h"

// Lua includes.
#include "LuaContext.h"

void LuaGameObjects::LuaGraphicsContext::Register(std::shared_ptr<Lua::LuaContext> luaContext, std::shared_ptr<Graphics::GraphicsContext> graphicsContext)
{
	// Start balancing.
	luaContext->BeginBalancing();

	// Create a table to hold the vsync constants.
	int vsyncTable = luaContext->PushNewTable();

	// Push the vsync values into the table.
	luaContext->Push((int)Graphics::VSyncMode::None);
	luaContext->SetField("none", vsyncTable);
	luaContext->Push((int)Graphics::VSyncMode::VSync);
	luaContext->SetField("vsync", vsyncTable);
	luaContext->Push((int)Graphics::VSyncMode::AdaptiveVsync);
	luaContext->SetField("adaptiveVsync", vsyncTable);

	// Put the vsync table into the global table.
	luaContext->SetGlobal(WINDOWVSYNCNAME);

	// Create a userdata that holds a pointer to the GraphicsContext. We get a pointer to the pointer back. Also store its stack position.
	std::weak_ptr<Graphics::GraphicsContext>* graphicsPointer = (std::weak_ptr<Graphics::GraphicsContext>*)luaContext->PushUserData(sizeof(std::weak_ptr<Graphics::GraphicsContext>));
	int graphicsContextPointerData = luaContext->GetTopIndex();

	// Placement operator, see Lua::LuaContext::Push(luaContextFunction) for more info.
	new(graphicsPointer) std::weak_ptr<Graphics::GraphicsContext>(graphicsContext);

	// Create a metatable for the userdata.
	int metatable = luaContext->PushNewMetatable(GLOBALGRAPHICSCONTEXTNAME);

	// Bind the read indexer to call the getter functions.
	luaContext->Push(getIndex);
	luaContext->SetField(Lua::INDEXERNAME, metatable);

	// Bind the assign indexer to call the setter functions.
	luaContext->Push(setIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, metatable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetatable(graphicsContextPointerData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(GLOBALGRAPHICSCONTEXTNAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaGraphicsContext::setIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the GraphicsContext, this should be the first argument.
	std::shared_ptr<Graphics::GraphicsContext> graphicsContext = ((std::weak_ptr<Graphics::GraphicsContext>*)luaContext->CheckUserData(1, GLOBALGRAPHICSCONTEXTNAME))->lock();
	
	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// Handle the property type.
	if (propertyName == WINDOWWIDTHNAME)
		graphicsContext->SetOutputWidth(luaContext->CheckInt(-1));
	else if (propertyName == WINDOWHEIGHTNAME)
		graphicsContext->SetOutputHeight(luaContext->CheckInt(-1));
	else if (propertyName == WINDOWTITLENAME)
		graphicsContext->SetWindowTitle(luaContext->CheckString(-1).c_str());

	return 0;
}

int LuaGameObjects::LuaGraphicsContext::getIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the GraphicsContext, this should be the first argument.
	std::shared_ptr<Graphics::GraphicsContext> graphicsContext = ((std::weak_ptr<Graphics::GraphicsContext>*)luaContext->CheckUserData(1, GLOBALGRAPHICSCONTEXTNAME))->lock();

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->CheckString(2);

	// Handle the property type.
	if (propertyName == WINDOWWIDTHNAME)
	{
		luaContext->Push(graphicsContext->GetOutputWidth());
		return 1;
	}
	else if (propertyName == WINDOWHEIGHTNAME)
	{
		luaContext->Push(graphicsContext->GetOutputHeight());
		return 1;
	}
	else if (propertyName == WINDOWTITLENAME)
	{
		luaContext->Push(graphicsContext->GetWindowTitle());
		return 1;
	}
	else if (propertyName == WINDOWVSYNCNAME)
	{
		luaContext->Push((int)graphicsContext->GetVSyncMode());
		return 1;
	}

	return 0;
}
