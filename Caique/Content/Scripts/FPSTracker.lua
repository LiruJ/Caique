local skipFrames = 120
local currentFrame = 0

local maxFPS = 0


function Update(gameTime)
    if currentFrame >= skipFrames then
        if gameTimeManager.frameRate > maxFPS then maxFPS = gameTimeManager.frameRate end

        graphicsContext.windowTitle = "Delta time: " .. string.format("%.2f", gameTime.deltaMilliseconds) .. "ms current fps: " .. gameTimeManager.frameRate .. " max fps: " .. maxFPS .. " (updates every " .. skipFrames .. " frames)"

        currentFrame = 0
    else
        currentFrame = currentFrame + 1
    end

end