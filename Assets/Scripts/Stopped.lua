function OnThink(self)
  if(self:GetLinearVelocity() == hkvVec3.ZeroVector()) then
    self:Remove()
  end
end