try
    for pin = 0:13
        dio_name = ['DIO' num2str(pin)];
        LabJack.LJM.eWriteName(handle, dio_name, 0);
    end
    disp("Input mode set for DIO0-DIO13");
catch
    msgbox("You forget to grabjack?","Error","error");
end