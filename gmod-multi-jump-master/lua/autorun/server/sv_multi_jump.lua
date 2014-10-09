local default_jumps = CreateConVar("multijump_default_jumps", "1", FCVAR_ARCHIVE, "The amount of extra jumps players should get (requires restart)")
local default_power = CreateConVar("multijump_default_power", "1", FCVAR_ARCHIVE, "Multiplier for the jump-power when multi jumping (requires restart)")

hook.Add("OnEntityCreated", "Multi Jump", function(ply)
	if ply:IsPlayer() then

		ply:SetJumpLevel(0)
		ply:SetMaxJumpLevel(default_jumps:GetInt())
		ply:SetExtraJumpPower(default_power:GetInt())

	end
end)

cvars.AddChangeCallback("multijump_default_jumps", function(_, old, new)
	new = tonumber(new)

	for k, v in pairs(player.GetAll()) do
		v:SetMaxJumpLevel(new)
	end
end)

cvars.AddChangeCallback("multijump_default_power", function(_, old, new)
	new = tonumber(new)

	for k, v in pairs(player.GetAll()) do
		v:SetExtraJumpPower(new)
	end
end)