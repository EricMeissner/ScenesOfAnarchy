function InitInput(self)

  self.map = Input:CreateMap("InputMap")
  local w, h = Screen:GetViewportSize()

  -- Put the camera control button in the top right corner
  self.screenMask = Game:CreateScreenMask(16, 16, "Textures\\GUI\\freeCamera.dds")
  self.screenMask:SetBlending(Vision.BLEND_ALPHA)
  local smw, smh = self.screenMask:GetTextureSize()
  self.screenMask:SetPos( w - smw - 16, 16 );
  
  -- Touch input
  self.map:MapTrigger("TOUCH_ANY", { w - smw - 16, 16, w - 16, smh + 16 }, "CT_TOUCH_ANY", {once = true})

  -- Mouse
  self.map:MapTrigger("TOUCH_ANY", "MOUSE", "CT_MOUSE_LEFT_BUTTON", {once = true})
  
  -- WiiU
  if (Application:GetPlatformName() == "WIIU") then
    self.map:MapTrigger("TOUCH_ANY", "DRC1", "CT_PAD_WIIU_TOUCH_ANY", {once = true})
  end
  
end

function Init(self) 
  InitInput(self)
  
  self.contextcam = Game:GetCamera()
  self.pathcam = Game:GetEntity("pathcam")
  self.freecam = Game:CreateEntity(self.pathcam:GetPosition(), "VFreeCamera", nil, "freecam")
  self.fpscam = Game:CreateEntity(self.pathcam:GetPosition(), "VFpsCameraEntity", nil, "fpscam")
  self.showInfo = false
 
  SetPathCamera(self)
end

function DrawCursor(minX, minY, maxX, maxY)
  local heightOffset = (maxY - minY) / 3
  local widthOffset = (maxX - minX) / 3
  Debug.Draw:Line2D( minX, minY, minX + widthOffset, maxY )
  Debug.Draw:Line2D( maxX, minY + heightOffset, minX, minY )
  Debug.Draw:Line2D( minX + widthOffset, maxY, maxX, minY + heightOffset )  
end

function SetMouseCamera(self)
	Debug:PrintLine("Activating mouse camera")
--  if (Application:GetPlatformName() == "ANDROID") or (Application:GetPlatformName() == "IOS") or (Application:GetPlatformName() == "TIZEN") then
    local tex = Game:CreateTexture("Textures\\GUI\\pathCamera.dds")
    self.screenMask:SetTextureObject(tex)
--  end

  self.freecam:SetPosition(self.contextcam:GetPosition());
  self.freecam:SetOrientation(self.contextcam:GetOrientation());                        
  self.pathcam:Stop();
  self.fpscam:SetThinkFunctionStatus(false);
  self.freecam:SetThinkFunctionStatus(true);
  self.contextcam:AttachToEntity(self.freecam);
  self.contextcam:Set(self.freecam:GetRotationMatrix(), self.freecam:GetPosition());
  self.cameraState = 0;
end

function SetPathCamera(self)
	Debug:PrintLine("Activating path camera")
--  if (Application:GetPlatformName() == "ANDROID") or (Application:GetPlatformName() == "IOS") or (Application:GetPlatformName() == "TIZEN") then
    local tex = Game:CreateTexture("Textures\\GUI\\playerCamera.dds")
    self.screenMask:SetTextureObject(tex)
--  end
  
  self.freecam:SetThinkFunctionStatus(false);          
  self.fpscam:SetThinkFunctionStatus(false);
  self.pathcam:Start()
  self.contextcam:AttachToEntity(self.pathcam);
  self.contextcam:Set(self.pathcam:GetRotationMatrix(), self.pathcam:GetPosition());
  self.cameraState = 1;
end

function SetFpsCamera(self)
	Debug:PrintLine("Activating FPS camera")
--  if (Application:GetPlatformName() == "ANDROID") or (Application:GetPlatformName() == "IOS") or (Application:GetPlatformName() == "TIZEN") then
    local tex = Game:CreateTexture("Textures\\GUI\\freeCamera.dds")
    self.screenMask:SetTextureObject(tex)
--  end

  self.pathcam:Stop();
  self.freecam:SetThinkFunctionStatus(false);
  self.fpscam:SetThinkFunctionStatus(true);
  -- self.contextcam:AttachToEntity(self.fpscam);
  -- self.contextcam:Set(self.fpscam:GetRotationMatrix(), self.fpscam:GetPosition());
  self.cameraState = 2;
end


function OnAfterSceneLoaded(self)
  Init(self);
end

function OnBeforeSceneUnloaded(self)
  if (self.map ~= nil) then
    Input:DestroyMap(self.map)
    self.map = nil
  end
  Game:DeleteAllUnrefScreenMasks()
end

function OnThink(self)
  
  -- Mouse cursor
  --if (Application:GetPlatformName() ~= "ANDROID") and (Application:GetPlatformName() ~= "IOS") and (Application:GetPlatformName() ~= "TIZEN") then
  --  local w, h = Screen:GetViewportSize()
  --  local mouseX, mouseY = Input:GetMousePosition()
  --  DrawCursor(mouseX, mouseY, mouseX+16,mouseY+16)
  --end

  -- Switch camera mode
  if (self.map:GetTrigger("TOUCH_ANY") > 0) then
    if (self.cameraState == 0) then
      SetPathCamera(self)
    elseif (self.cameraState == 1) then
      SetFpsCamera(self)
    elseif (self.cameraState == 2) then
      SetMouseCamera(self)
    end
  end

end

