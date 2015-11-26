_UnitNames = {"wuka", "lamaki", "jeats", "qbi"}
_UnitHeights = {0.0, 0.0, 0.0, 4.0}

function setAIManager(AIManager)
	_AIManager = AIManager
	math.randomseed(os.time())
end

function OnCreateWarrior(name, position)
	local rUnit = math.random(1,4)
	local pos = Vector3.new(position:x(), position:y() + _UnitHeights[rUnit], position:z())
	_AIManager:CreateWarrior(_UnitNames[rUnit], pos)
    --_AIManager:CreateWarrior(name, position)
end