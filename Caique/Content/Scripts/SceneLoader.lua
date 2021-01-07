-- Create the camera
local camera = scene:addNewGameObject(vector3.new(0, 0, 21))
camera:addComponent("Camera", graphicsContext)
camera:addComponent("ScriptInstance", "Scripts\\CameraController")

local fpsTracker = scene:addNewGameObject():addComponent("ScriptInstance", "Scripts\\FPSTracker")
fpsTracker.skipFrames = 40

scene:addNewGameObject(vector3.new(-2, -4, -5)):addComponent("MeshRenderer", "Models\\Lamp")
scene:addNewGameObject(vector3.new(-2, -4, -10)):addComponent("MeshRenderer", "Models\\Desk")
scene:addNewGameObject(vector3.new(-20, -4, -15)):addComponent("MeshRenderer", "Models\\Bookshelf1")