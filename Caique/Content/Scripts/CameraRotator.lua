local distance = 21
local rotationPerSecond = math.rad(25)

function Update(gameTime)
    transform.localRotation = quaternion.fromEuler(vector3.new(0, rotationPerSecond * gameTime.deltaSeconds, 0)) * transform.localRotation
end