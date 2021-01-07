radiansPerSecond = radiansPerSecond and radiansPerSecond or math.rad(10)

function Update(gameTime)
	transform.localRotation = quaternion.fromAngleAxis(radiansPerSecond * gameTime.deltaSeconds, vector3.up) * transform.localRotation
end