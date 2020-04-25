function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/dive-uploader.exe", "@StartMenuDir@/dive-uploader.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icon.ico",
            "description=Open DiveUploader");

        component.addOperation("CreateShortcut", "@TargetDir@/dive-uploader.exe", "@DesktopDir@/dive-uploader.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icon.ico",
            "description=Open DiveUploader");
    }
}