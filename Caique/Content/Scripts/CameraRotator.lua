-- How fast the camera moves around
local moveSpeed = 15

-- Two pi, uuh, two times pi... yeah
local twoPi = math.pi * 2

-- The steepest angle the camera can look up or down
local minimumY = math.rad(-89);
local maximumY = math.rad(89);

-- The rotation of the camera when the script loads
local startRotation

-- The stored rotation
local rotationX = 0
local rotationY = 0

-- How sensitive the mouse is
local xSensitivity = 0.03
local ySensitivity = 0.03

-- Control keys
local forward = "W"
local backward = "S"
local left = "A"
local right = "D"

-- Clamps the given angle between the two constraints. Might move this over to C++
function clampAngle(angle, min, max)
	if (angle < -twoPi) then angle = angle + twoPi end
	if (angle > twoPi) then angle = angle - twoPi end
	return math.max(min, math.min(angle, max))
end

-- When the script initialises, save the rotation
function Initialise()
	startRotation = transform.localRotation
end

function Update(gameTime)
	-- If the camera should rotate, do so
	if inputManager.mouseDeltaX ~= 0 or inputManager.mouseDeltaY ~= 0 then
		-- Calculate the movement in radians
		local xRads = math.rad(inputManager.mouseDeltaX) * xSensitivity
		local yRads = math.rad(inputManager.mouseDeltaY) * ySensitivity

		-- Calculate the resulting angle
		rotationX = clampAngle(rotationX - xRads, -twoPi, twoPi)
		rotationY = clampAngle(rotationY + yRads, minimumY, maximumY)

		-- Make the quaternions around the axes
		local xQuat = quaternion.fromAngleAxis(rotationX, vector3.up)
		local yQuat = quaternion.fromAngleAxis(rotationY, vector3.left)

		-- Change the rotation of the camera
		transform.localRotation = startRotation * xQuat * yQuat
	end

	-- Handle movement
	local movement = vector3.new(0, 0, 0)
    if inputManager:isKeyDown(left) then
    	movement = movement - transform.localRight
    end
    if inputManager:isKeyDown(right) then
    	movement = movement + transform.localRight
    end
    if inputManager:isKeyDown(forward) then
    	movement = movement + transform.localForward
    end
    if inputManager:isKeyDown(backward) then
    	movement = movement - transform.localForward
    end

    -- If there is movement to be made, then do so
    if movement.length > 0 then transform.localPosition = transform.localPosition + (movement.normal * gameTime.deltaSeconds * moveSpeed) end
end