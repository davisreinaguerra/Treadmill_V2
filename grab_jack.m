ljmAsm = NET.addAssembly('LabJack.LJM');
t = ljmAsm.AssemblyHandle.GetType('LabJack.LJM+CONSTANTS');
LJM_CONSTANTS = System.Activator.CreateInstance(t);
handle = 0;

try 
    [ljmError, handle] = LabJack.LJM.OpenS('ANY', 'ANY', 'ANY', handle);
    showDeviceInfo(handle)
catch
    msgbox("You forget to plug that shit in?","Error","error");
end