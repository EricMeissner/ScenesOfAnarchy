-- new script file

function OnThink(self)
	if Input:IsKeyPressed(Vision.KEY_SPACE) then
   local entity = Game:CreateEntity(Vision.hkvVec3(50,100,100),"VisBaseEntity_cl","Models/Misc/Cube.model")
   entity:SetScaling(.25)
   entity:AddComponentOfType("vHavokRigidBody","RigidBody" )
   end
end
               
function OnCreate(self)
	Input:SetKeyAsSingleHit(Vision.KEY_SPACE,true)
end
