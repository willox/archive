require("mysqloo")

assert(mysqloo, "[DATABASE] MySQLOO is not installed")

local assert, tonumber, mysqloo, timer, type, tostring, print, pairs, table, debug, setmetatable, IsValid, pcall, Error =
      assert, tonumber, mysqloo, timer, type, tostring, print, pairs, table, debug, setmetatable, IsValid, pcall, Error


module("db")

local databaseMeta = {}
databaseMeta.__index = databaseMeta
databaseMeta.__gc = function(self) self:Disconnect() end

databaseMeta.DATABASE_CONNECTED = mysqloo.DATABASE_CONNECTED
databaseMeta.DATABASE_CONNECTING = mysqloo.DATABASE_CONNECTING
databaseMeta.DATABASE_NOT_CONNECTED = mysqloo.DATABASE_NOT_CONNECTED
databaseMeta.DATABASE_INTERNAL_ERROR = mysqloo.DATABASE_INTERNAL_ERROR

function databaseMeta:Init(config)
    self.config = config
    self.stored_queries = {}
    self:Connect()
end

function databaseMeta:OnConnected()
    print("[DATABASE] Connected")
end

function databaseMeta:OnConnectionFailed(err)
    Error("[DATABASE] Lost Connection\n")
    Error("[DATABASE] " .. err .. "\n")
end

function databaseMeta:OnError(err, sql, caller_info)
    Error("[DATABASE] Encountered Error\n")
    Error("[DATABASE] Query: " .. sql .. "\n")
    Error("[DATABASE] Error: " .. err .. "\n")
    Error("[DATABASE] Source: " .. caller_info.short_src .. ":" .. caller_info.currentline .. "\n")
end

function databaseMeta:Connect()
    if self.__database then
        self:Disconnect()
    end

    local config = self.config

    self.__database = mysqloo.connect(config['Host'],
                                      config['User'],
                                      config['Pass'],
                                      config['Name'],
                                      config['Port'])

    self.__database.onConnected = function()
        self:OnConnected()
    end

    self.__database.onConnectionFailed = function(_, err)
        self:OnConnectionFailed(err)
    end

    self.__database:connect()
    self.__database:wait()

    timer.Create("Database KeepAlive: " .. tostring(self), 15, 0, function()
        self:KeepAlive()
    end)
end

function databaseMeta:Disconnect()
    if self.__database then
        self.__database:abortAllQueries()
        self.__database = nil

        timer.Remove("Database KeepAlive: " .. tostring(self))
    end
end

function databaseMeta:Retry()
    print("[DATABASE] Retrying Connection")

    self:Connect()
end

function databaseMeta:CheckConnection()
    if not self.__database then
        self:Connect()
    end
end

function databaseMeta:Status()
    self:CheckConnection()

    local statusMessages = {
        [self.DATABASE_CONNECTED] = "Connected",
        [self.DATABASE_CONNECTING] = "Connecting",
        [self.DATABASE_NOT_CONNECTED] = "Not Connected",
        [self.DATABASE_INTERNAL_ERROR] = "Internal Error"
    }

    local status = self.__database:status()

    return status, statusMessages[status] or "N/A"
end

function databaseMeta:CreateQueryObject(query, callback, fetchAll, _retry)
    local caller_info = debug.getinfo(3)
    self:CheckConnection()

    local newCallback = function(queryObj, data) -- queryObj isn't passed to the callback correctly
        if callback then
            local success, err = pcall(callback, data, queryObj:lastInsert())

            if not success then
                Error("[DATABASE] Callback Error\n")
                Error(err .. "\n")
            end
        end
    end

    local queryObj = self.__database:query(query)

    if queryObj then
        if fetchAll then
            queryObj.onSuccess = newCallback
        else
            queryObj.onData = newCallback
        end

        queryObj.onError = function(_, err, sql)
            self:OnError(err, sql, caller_info)

            if self:Status() == self.DATABASE_NOT_CONNECTED then
                self:Connect()
                
                queryObj:start()
            end
        end

        queryObj:setOption(mysqloo.OPTION_NAMED_FIELDS)

        return queryObj
    else
        local err, errMessage = self:Status()

        print("[DATABASE] Query Failed: " .. errMessage .. " (" .. err .. ").")

        if err == self.DATABASE_NOT_CONNECTED then
            self:Retry()
        end
    end
end

function databaseMeta:Query(...)
    local queryObj = self:CreateQueryObject(...)

    if queryObj then
        queryObj:start()

        return queryObj
    end

    return false
end

function databaseMeta:QueryWait(...)
    local queryObj = self:CreateQueryObject(...)

    if queryObj then
        queryObj:start()
        queryObj:wait()

        return queryObj
    end

    return false
end

function databaseMeta:KeepAlive()
    self:CheckConnection()

    self:Query("SELECT 1")
end

function databaseMeta:Escape(str)
    self:CheckConnection()

    return self.__database:escape(tostring(str))
end


function databaseMeta:Register(id, query)
    self.stored_queries[id] = query
end

function databaseMeta:Execute(id, ...)
    assert(self.stored_queries[id], "attempt to execute non-existent query (" .. tostring(id) .. ")")

    local args = {...}

    local query = self.stored_queries[id]:gsub("%{(%d)%}", function(capture)
        local i = tonumber(capture)

        assert(args[i], "undefined query argument (" .. i .. ") in " .. tostring(id))
        
        if type(args[i]) == "Player" then
            args[i] = args[i]:SteamID()
        end

        return "'" .. self:Escape(args[i]) .. "'"
    end)

    local callback = args[#args]

    if type(callback) == "function" then
        self:Query(query, callback)
    else
        self:Query(query)
    end
end

----------------

function New(config)
    local dbObj = {}
    setmetatable(dbObj, databaseMeta)
    dbObj:Init(config)

    return dbObj
end
