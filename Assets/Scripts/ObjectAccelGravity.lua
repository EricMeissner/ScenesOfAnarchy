gravForce = Vision.hkvVec3(1,1,1)

function OnAfterSceneLoaded(self)
  self.map = Input:CreateMap("objectInputMap")
  
  self.map:MapTrigger("MotionX", "MOTION", "CT_MOTION_ACCELERATION_X")
  self.map:MapTrigger("MotionY", "MOTION", "CT_MOTION_ACCELERATION_Y")
  self.map:MapTrigger("MotionZ", "MOTION", "CT_MOTION_ACCELERATION_Z")
end

function OnThink(self)
  self.t = Timer:GetTimeDiff()
  local motionX = self.map:GetTrigger("MotionX")
  local motionY = self.map:GetTrigger("MotionY")
  local motionZ = self.map:GetTrigger("MotionZ")
  
  Debug:PrintLine(motionX .. "," .. motionY .. "," .. motionZ)
  
  gravForce:set(motionX*9810, motionY*9810, motionZ*9810)
  self.RigidBody:ApplyForce(gravForce, self.t)
end

function OnCreate(self)
  self:AddComponentOfType("vHavokRigidBody", "RigidBody")
  self.RigidBody:SetMass(10)
  self.RigidBody:SetRestitution(.81)
end