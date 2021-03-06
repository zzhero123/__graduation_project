
-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
end

local function main()
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    local cclog = function(...)
        print(string.format(...))
    end
    ---------------

    local enhancingEquipmentUI = MCEnhancingEquipmentUI:create()
    local currentScene = MCSceneContextManager:sharedSceneContextManager():currentContext():getScene()
    enhancingEquipmentUI:attach(currentScene)
    
end

xpcall(main, __G__TRACKBACK__)
