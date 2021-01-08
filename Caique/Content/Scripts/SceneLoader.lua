-- Seed the randomiser
math.randomseed(os.time())

-- Create the camera
local camera = scene:addNewGameObject(vector3.new(0, 10, 21))
camera:addComponent("Camera", graphicsContext)
camera:addComponent("ScriptInstance", "Scripts\\CameraController")

-- FPS tracker
local fpsTracker = scene:addNewGameObject():addComponent("ScriptInstance", "Scripts\\FPSTracker")
fpsTracker.skipFrames = 40

-- The terrain
scene:addNewGameObject(vector3.new(0, 0, 0)):addComponent("MeshRenderer", "Models\\Terrain")

-- Rows of lamps
for i=-30,30,10 do
	scene:addNewGameObject(vector3.new(-30, 0, i)):addComponent("MeshRenderer", "Models\\Lamp")
	scene:addNewGameObject(vector3.new(-22.5, 0, i)):addComponent("MeshRenderer", "Models\\Lamp")
end


scene:addNewGameObject(vector3.new(-2, 0, -10)):addComponent("MeshRenderer", "Models\\Desk")
scene:addNewGameObject(vector3.new(-4, 0, -15)):addComponent("MeshRenderer", "Models\\Bookshelf1")

-- Tree/book circle
local treesInCircle = 5
local treeRadius = 7
local treeCircleCentre = vector3.new(-5, 0, 15)
local spinningBooks = scene:addNewGameObject(treeCircleCentre)
local booksSpinner = spinningBooks:addComponent("ScriptInstance", "Scripts\\ObjectSpinner")
booksSpinner.radiansPerSecond =	math.rad(45)

for r = -math.pi, math.pi * 0.99, (math.pi * 2) / treesInCircle do
	scene:addNewGameObject(treeCircleCentre + vector3.new(math.cos(r), 0, math.sin(r)) * treeRadius, quaternion.fromAngleAxis((math.random() * math.pi * 2) - 1, vector3.up)):addComponent("MeshRenderer", "Models\\Tree1")

	local book = spinningBooks:addNewGameObject(vector3.new(math.cos(r), 0, math.sin(r)) * (treeRadius / 4))
	book:addComponent("MeshRenderer", "Models\\BookOpen")
	local bookSpinner = book:addComponent("ScriptInstance", "Scripts\\ObjectSpinner")
	bookSpinner.radiansPerSecond = math.rad(-45)
end

scene:addNewGameObject(vector3.new(0, 0, 0), quaternion.fromAngleAxis((math.random() * math.pi * 2) - 1, vector3.up)):addComponent("MeshRenderer", "Models\\Tree1")