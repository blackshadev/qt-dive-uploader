function Component()
{
    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles !== "") {
        installer.setValue("TargetDir", programFiles + "/LittleDivelog");
    }
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
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

Component.prototype.installationFinishedPageIsShown = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "AfterInstallationForm", QInstaller.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var isChecked = component.userInterface("AfterInstallationForm").checkbox.checked;
            if (isChecked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/dive-uploader.exe");
            }
        }
    } catch(e) {
        console.log(e);
    }
}