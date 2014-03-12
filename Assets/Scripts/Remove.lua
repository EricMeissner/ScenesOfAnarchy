function OnObjectEnter(self, object)
	
  if(object:GetComponentOfType("vHavokRigidBody") ~= nil) then 
    object:Remove()
  end
end