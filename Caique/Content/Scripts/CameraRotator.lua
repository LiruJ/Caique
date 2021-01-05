local distance = 21
local rotationPerSecond = math.rad(25)

local moveSpeed = 3

function Update(gameTime)
	local movement = vector3.new(0, 0, 0)

    if inputManager:isKeyDown("A") then
    	--transform.localRotation = quaternion.fromEuler(vector3.new(0, rotationPerSecond * gameTime.deltaSeconds, 0)) * transform.localRotation
    	movement = movement - transform.localRight
    end
    if inputManager:isKeyDown("D") then
    	movement = movement + transform.localRight
    end
    if inputManager:isKeyDown("W") then
    	movement = movement + transform.localForward
    end
    if inputManager:isKeyDown("S") then
    	movement = movement - transform.localForward
    end

    if movement.length > 0 then transform.localPosition = transform.localPosition + (movement.normal * gameTime.deltaSeconds * moveSpeed) end
end