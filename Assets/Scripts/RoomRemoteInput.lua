G.useRemoteInput = true

function OnAfterSceneLoaded(self)
  if G.useRemoteInput then
    RemoteInput:StartServer('RemoteGui')
    RemoteInput:InitEmulatedDevices()
  end
end
