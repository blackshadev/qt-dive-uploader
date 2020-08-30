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
        component.addOperation("CreateShortcut", "@TargetDir@/dive-uploader.exe", "@StartMenuDir@/Little Dive Log Uploader.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icon.ico",
            "description=Open the Little Dive Log Uploader");

        component.addOperation("CreateShortcut",
            "@TargetDir@/dive-uploader.exe",
            "@DesktopDir@/Little Dive Log Uploader.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/icon.ico",
            "description=Open the Little Dive Log Uploader",
            "name=Littlelog Dive Log Uploader"
        );
    }

    if (installer.value("os") === "x11") {
        component.addOperation("CreateDesktopEntry",
            "/usr/share/applications/LittleDiveLogUploader.desktop",
            "Version=1.0\nType=Application\nTerminal=false\nExec=@TargetDir@/dive-uploader.sh\nName=Littlelog Dive Log Uploader\nIcon=@TargetDir@/resources/icon.ico\nName[en_US]=Little Dive Log Uploader"
        );
        component.addElevatedOperation("Copy",
            "/usr/share/applications/LittleDiveLogUploader.desktop",
            "@HomeDir@/Desktop/LittleDiveLogUploader.desktop"
        );
    }
}

Component.prototype.installationFinishedPageIsShown = function()
{
//    try {
//        if (installer.isInstaller() && installer.status == QInstaller.Success) {
//            installer.addWizardPageItem( component, "AfterInstallationForm", QInstaller.InstallationFinished );
//        }
//    } catch(e) {
//        console.log(e);
//    }
}

Component.prototype.installationFinished = function()
{
//    try {
//        if (installer.isInstaller() && installer.status == QInstaller.Success) {
//            var isChecked = component.userInterface("AfterInstallationForm").checkbox.checked;
//            var fileName = installer.value("TargetDir") + "/dive-uploader";
//            switch (installer.value("os")) {
//                case "x11": fileName += ".sh"; break;
//                case "windows": fileName += ".exe"; break;
//            }

//            if (isChecked) {
//                QProcess.startDetached(fileName);
//            }
//        }
//    } catch(e) {
//        console.log(e);
//    }
}
