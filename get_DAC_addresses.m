try
    aNamesOut = NET.createArray('System.String', 2);
    aNamesOut(1) = 'DAC0';  %address for 167Hz sine wave
    aNamesOut(2) = 'DAC1';  %address for 223Hz sine wave
    aAddressesOut = NET.createArray('System.Int32', 2);
    aTypesOut = NET.createArray('System.Int32', 2);    
    LabJack.LJM.NamesToAddresses(2, aNamesOut, aAddressesOut, aTypesOut);
    
    DAC0_addy = aAddressesOut(1);
    DAC1_addy = aAddressesOut(2);
catch
    msgbox("You forget to grabjack?","Error","error");
end