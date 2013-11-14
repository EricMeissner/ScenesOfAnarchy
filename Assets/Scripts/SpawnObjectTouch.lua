function OnAfterSceneLoaded(self)
  self.map = Input:CreateMap("VInputMap")
  self.w, self.h = Screen:GetViewportSize()
  
  self.map:MapTrigger("TouchTopLeft", {0, 0, self.w * .50, self.h * .50}, "CT_TOUCH_ANY")
  self.map:MapTrigger("TouchBottomLeft", {0, self.h*.50, self.w*.50, self.h}, "CT_TOUCH_ANY")
  self.map:MapTrigger("TouchTopRight", {self.w*.50, 0, self.w, self.h * .50}, "CT_TOUCH_ANY")

  
end


function OnThink(self)

  local touchBLeft = self.map:GetTrigger("TouchBottomLeft")>0
  local touchTRight = self.map:GetTrigger("TouchTopRight")>0
  local touchTLeft = self.map:GetTrigger("TouchTopLeft")>0
  local touchBRight = self.map:GetTrigger("TouchBottomRight")>0
  
  if touchBLeft then
   local entity = Game:CreateEntity(Vision.hkvVec3(50,100,100),"VisBaseEntity_cl","Models/Misc/Cube.model")
           entity:SetScaling(.25)
           entity:AddComponentOfType("vHavokRigidBody","RigidBody" )
  end
  
  if touchTRight then
     local entity = Game:CreateEntity(Vision.hkvVec3(50,50,50),"VisBaseEntity_cl","Models/Misc/troll.model")
           entity:SetScaling(.25)
           entity:AddComponentOfType("vHavokRigidBody","RigidBody" )
  end
  
    if touchTLeft then
     local entity = Game:CreateEntity(Vision.hkvVec3(50,100,100),"VisBaseEntity_cl","Models/Misc/Sphere.Model")
           entity:SetScaling(.25)
           entity:AddComponentOfType("vHavokRigidBody","RigidBody" )
  end

  
end
  
  
