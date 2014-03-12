function OnObjectEnter(self, object)
	
  if(object:GetComponentOfType("vHavokRigidBody") ~= nil) then 

    -- push the object off the floor
  thex = -1 * object:GetPosition().x
  they = -1 * object:GetPosition().y + 1400
  object:GetComponentOfType("vHavokRigidBody"):SetLinearVelocity(Vision.hkvVec3( thex, they, 1500))   

  end
end