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

	// Pop the vsync table off the stack.
	luaContext->SetGlobal(WINDOWVSYNCNAME);

	// Create a userdata that holds a pointer to the GraphicsContext. We get a pointer to the pointer back. Also store its stack position.
	Graphics::GraphicsContext** pointerUserData = (Graphics::GraphicsContext**)luaContext->PushUserData(sizeof(graphicsContext.get()));
	int userData = luaContext->GetTopIndex();

	// Set the userdata pointer to the GraphicsContext.
	*pointerUserData = graphicsContext.get();

	// Create a metatable for the userdata.
	int metaTable = luaContext->PushNewMetaTable(GLOBALGRAPHICSCONTEXTNAME);

	// Bind the read indexer to call the getter functions.
	luaContext->Push(GetIndex);
	luaContext->SetField(Lua::INDEXERNAME, metaTable);

	// Bind the assign indexer to call the setter functions.
	luaContext->Push(SetIndex);
	luaContext->SetField(Lua::NEWINDEXERNAME, metaTable);

	// Set the metatable of the userdata to the created metatable.
	luaContext->SetMetaTable(userData);

	// Add the userdata to the global namespace so that any script can access and use it.
	luaContext->SetGlobal(GLOBALGRAPHICSCONTEXTNAME);

	// Stop balancing.
	luaContext->StopBalancing();
}

int LuaGameObjects::LuaGraphicsContext::SetIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the GraphicsContext, this should be the first argument.
	Graphics::GraphicsContext* graphicsContext = *static_cast<Graphics::GraphicsContext**>(luaContext->ToUserData(1));
	
	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->ToString(2);

	// Handle the property type.
	if (propertyName == WINDOWWIDTHNAME)
		graphicsContext->SetOutputWidth(luaContext->CheckInt(-1));
	else if (propertyName == WINDOWHEIGHTNAME)
		graphicsContext->SetOutputHeight(luaContext->CheckInt(-1));
	else if (propertyName == WINDOWTITLENAME)
		graphicsContext->SetWindowTitle(luaContext->CheckString(-1).c_str());

	return 0;
}

int LuaGameObjects::LuaGraphicsContext::GetIndex(std::shared_ptr<Lua::LuaContext> luaContext)
{
	// Get the GraphicsContext, this should be the first argument.
	Graphics::GraphicsContext* graphicsContext = *static_cast<Graphics::GraphicsContext**>(luaContext->ToUserData(1));

	// The name of the property itself should be the second argument.
	std::string propertyName = luaContext->ToString(2);

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
