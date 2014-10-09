PrecacheParticleSystem("doublejump_smoke")

local function GetMoveVector(mv)
	local ang = mv:GetAngles()

	local max_speed = mv:GetMaxSpeed()

	local forward = math.Clamp(mv:GetForwardSpeed(), -max_speed, max_speed)
	local side = math.Clamp(mv:GetSideSpeed(), -max_speed, max_speed)

	local abs_xy_move = math.abs(forward) + math.abs(side)

	if abs_xy_move == 0 then
		return Vector(0, 0, 0)
	end

	local mul = max_speed / abs_xy_move

	local vec = Vector()

	vec:Add(ang:Forward() * forward)
	vec:Add(ang:Right() * side)

	vec:Mul(mul)

	return vec
end

hook.Add("SetupMove", "Multi Jump", function(ply, mv)
	-- Let the engine handle movement from the ground
	if ply:OnGround() then
		ply:SetJumpLevel(0)

		return
	end	

	-- Don't do anything if not jumping
	if not mv:KeyPressed(IN_JUMP) then
		return
	end

	ply:SetJumpLevel(ply:GetJumpLevel() + 1)

	if ply:GetJumpLevel() > ply:GetMaxJumpLevel() then
		return
	end

	local vel = GetMoveVector(mv)

	vel.z = ply:GetJumpPower() * ply:GetExtraJumpPower()

	mv:SetVelocity(vel)

	ply:DoCustomAnimEvent(PLAYERANIMEVENT_JUMP , -1)

	for i = 1, 4 do
		ParticleEffect("doublejump_smoke", ply:GetPos() + Vector(0, 0, 10), ply:GetAngles(), ply)
	end
end)