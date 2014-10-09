local config = {
	Host = "localhost",
	User = "root"
	Pass = ""
	Name = "garrysmod" -- Database Name
}

require("db")

local database = db.New(config)

database:Query("SELECT 1 AS `col`", function(row)
	print(row['col']) -- 1
end)