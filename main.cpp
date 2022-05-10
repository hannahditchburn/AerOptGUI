/*********************************************
**
**	Created on: 	28/12/2013 2013
**	Author: 	matt - Matt Edmunds
**	File:		main.cpp
**  This is the English version of this programme
**
**********************************************/

#include <iostream>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QSplashScreen>
#include <QMessageBox>

#include "DebugOutput.h"
#include "MainWindow.h"
#include "OptimisationModel.h"
#include "windowsizing.h"

void firstTimeSetup(QString AerOptWorkDir) {
    QSettings settings;
    settings.clear();

    // Spaces in directory path mess with the MeshGenerator executable - have to exclude from AerOpt path.
    if(AerOptWorkDir.indexOf(' ') >= 0) {
        QMessageBox::critical(nullptr, "Error", "Invalid Path: AerOpt executables will not function properly if path contains spaces. Please move to a directory without spaces in the path.", QMessageBox::Ok);
        exit(-1);
    }

    AerOptWorkDir = QDir::toNativeSeparators(AerOptWorkDir);
    settings.setValue("AerOpt/workingDirectory", AerOptWorkDir);
    QDir().mkdir(AerOptWorkDir);

    settings.setValue("AerOpt/startLoadPath", AerOptWorkDir);

    // setup executables
    QString exeDir = QDir::toNativeSeparators(AerOptWorkDir + "Executables/");
    QDir().mkdir(exeDir);

    QString ext = ".exe";
    QString targetext = ".exe";

#ifdef Q_OS_UNIX
        ext = ".unix";
        targetext = "";
#endif
#ifdef Q_OS_MACOS
        ext = ".mac";
        targetext = "";
#endif

    bool copySuccess;
    bool exists = false;

    QString mesherExe = QDir::toNativeSeparators(exeDir + "MeshGenerator" + targetext);
    copySuccess = QFile::copy(":/executables/MeshGenerator" + ext, mesherExe);
    settings.setValue("mesher/exe", mesherExe);
    if(copySuccess) {
        QFile(mesherExe).setPermissions(QFileDevice::ExeUser);        
    } else {
        exists = QFile::exists(mesherExe);
        if(!exists){
            qCritical() << "Mesher file copy failed - check for presence of " << mesherExe;
        }
        else {
            qInfo() << "MeshGenerator.exe already present in " << exeDir;
        }
    }

    QString aeroptExe = QDir::toNativeSeparators(exeDir + "AerOpt" + targetext);
    copySuccess = QFile::copy(":/executables/AerOpt" + ext, aeroptExe);
    settings.setValue("AerOpt/Executable", aeroptExe);
    if(copySuccess) {
        QFile(aeroptExe).setPermissions(QFileDevice::ExeUser);        
    } else {
        exists = QFile::exists(aeroptExe);
        if(!exists){
            qCritical() << "AerOpt executable file copy failed - check for presence of " << aeroptExe;
        }
        else {
            qInfo() << "AerOpt.exe already present in " << exeDir;
        }
    }

    QString exePath = QDir::toNativeSeparators(exeDir + "Delaunay_2D" + targetext);
    QFile::copy(":/executables/Delaunay_2D" + ext, exePath);
    QFile(exePath).setPermissions(QFileDevice::ExeUser);

    exePath = QDir::toNativeSeparators(exeDir + "PrePro_2D" + targetext);
    QFile::copy(":/executables/PrePro_2D" + ext, exePath);
    QFile(exePath).setPermissions(QFileDevice::ExeUser);

    exePath = QDir::toNativeSeparators(exeDir + "Solver_2D" + targetext);
    QFile::copy(":/executables/Solver_2D" + ext, exePath);
    QFile(exePath).setPermissions(QFileDevice::ExeUser);

    qDebug() << "Working Directory:" << AerOptWorkDir;
    qDebug() << "Mesher Exe Path" << mesherExe;
    qDebug() << "AerOpt Exe Path" << aeroptExe;

    bool c = true;
    QFileInfo checkMesher(mesherExe);
    QFileInfo checkAerOpt(aeroptExe);
    c &= checkMesher.exists();
    c &= checkMesher.isFile();
    c &= checkAerOpt.exists();
    c &= checkAerOpt.isFile();

    if (!c)
    {
        qCritical() << "Application executables not available!";
    }

    // setup default profiles
    QString profileDir = QDir::toNativeSeparators(AerOptWorkDir + "profiles/");
    QDir().mkdir(profileDir);
    settings.setValue("AerOpt/profilesDefaultPath", profileDir);

    settings.beginWriteArray("profiles");
    settings.setArrayIndex(0);
    QString profileFile = QDir::toNativeSeparators(profileDir + "NACA0024.prf");
    copySuccess = QFile::copy(":/profiles/NACA0024.prf", profileFile);
    settings.setValue("filepath", profileFile);
    if(!copySuccess) {
        exists = QFile::exists(profileFile);
        if(!exists){
            qCritical() << "Copying of NACA0024.prf unsuccessful - check " << profileFile;
        }
        else {
            qInfo() << "NACA0024.prf already present in " << profileDir;
        }
    }

    settings.setArrayIndex(1);
    profileFile = QDir::toNativeSeparators(profileDir + "NACA21120.prf");
    copySuccess = QFile::copy(":/profiles/NACA21120.prf", profileFile);
    settings.setValue("filepath", profileFile);
    if(!copySuccess) {
        exists = QFile::exists(profileFile);
        if(!exists){
            qCritical() << "Copying of NACA21120.prf unsuccessful - check " << profileFile;
        }
        else {
            qInfo() << "NACA21120.prf already present in " << profileDir;
        }
    }

    settings.endArray();

    // Input folder is read by AerOpt executable
    QString inFolder = AerOptWorkDir + "Input_Data/";
    inFolder = QDir::toNativeSeparators(inFolder);
    settings.setValue("AerOpt/inFolder", inFolder);

    QString AerOptInFileName = "AerOpt_InputParameters.txt";
    QString AerOptInFile = inFolder + AerOptInFileName;
    AerOptInFile = QDir::toNativeSeparators(AerOptInFile);
    settings.setValue("AerOpt/inputFile", AerOptInFile);
    settings.setValue("AerOpt/inputFileName", AerOptInFileName);

    QString AerOptNodeFile = inFolder + "Control_Nodes.txt";
    AerOptNodeFile = QDir::toNativeSeparators(AerOptNodeFile);
    settings.setValue("AerOpt/nodeFile", AerOptNodeFile);

    QString AerOptBoundaryFile = inFolder + "Boundary_Points.txt";
    AerOptBoundaryFile = QDir::toNativeSeparators(AerOptBoundaryFile);
    settings.setValue("AerOpt/boundaryFile", AerOptBoundaryFile);

    // mesh is created in a scratch directory
    QString scratchPath = QDir::toNativeSeparators(AerOptWorkDir + "Scratch/");
    settings.setValue("mesher/scratchDir", scratchPath);

    QString initialMeshFile;
    initialMeshFile = QDir(scratchPath + "Mesh.dat").absolutePath();
    initialMeshFile = QDir::toNativeSeparators(initialMeshFile);
    settings.setValue("mesher/initMeshFile", initialMeshFile);

    // Initial cluster settings - assume AerOpt cluster directory is /AerOpt/
    settings.setValue("Cluster/AerOptDir", "AerOpt/");
    settings.setValue("Cluster/Username", "");
    settings.setValue("Cluster/Account", "scw1022");
    settings.setValue("Cluster/Address", "sunbird.swansea.ac.uk");
    // Cluster interaction settings
    settings.setValue("Cluster/WaitTime", "60");
    settings.setValue("Cluster/UpdateTime", "10");
    settings.setValue("Cluster/CheckTime", "60");
}

void checkSettings()
{
    //Setup up default settings
    QCoreApplication::setOrganizationName("Swansea University (Engineering)");
    QCoreApplication::setOrganizationDomain("engineering.swansea.ac.uk");
    QCoreApplication::setApplicationName("AerOpt");

    // get working directory
    QString appPath = QDir::fromNativeSeparators(QCoreApplication::applicationFilePath());
    QFileInfo appPathInfo(appPath);

    QString AerOptWorkDir = appPathInfo.dir().absolutePath();

    // macOS: remove part of path inside application bundle
    AerOptWorkDir = AerOptWorkDir.remove(QRegExp("/AerOptGui.app/Contents.*"));

    AerOptWorkDir = QDir::fromNativeSeparators(AerOptWorkDir + "/AerOptFiles/");

    // if working directory exists, then no need for setup
    if(!QDir(AerOptWorkDir).exists()) {
        firstTimeSetup(AerOptWorkDir);
    }

    QString targetext = ".exe";
#ifdef Q_OS_UNIX
        ext = ".unix";
        targetext = "";
#endif
#ifdef Q_OS_MACOS
        ext = ".mac";
        targetext = "";
#endif

    // Write out expected directories for checking purposes
    QString mesherExe = QDir::toNativeSeparators(AerOptWorkDir + "Executables/MeshGenerator" + targetext);
    QString aeroptExe = QDir::toNativeSeparators(AerOptWorkDir + "Executables/AerOpt" + targetext);
    QString inFolder = QDir::toNativeSeparators(AerOptWorkDir + "Input_Data/");
    QString scratchPath = QDir::toNativeSeparators(AerOptWorkDir + "Scratch/");

    // Check current logged settings. If any are blank, missing or incorrect, perform setup
    QSettings settings;
    bool settingcheck = true;
    settingcheck &= (AerOptWorkDir==QDir::fromNativeSeparators(settings.value("AerOpt/workingDirectory").toString()));
    settingcheck &= (mesherExe==QDir::fromNativeSeparators(settings.value("mesher/exe").toString()));
    settingcheck &= (aeroptExe==QDir::fromNativeSeparators(settings.value("AerOpt/Executable").toString()));
    settingcheck &= (inFolder==QDir::fromNativeSeparators(settings.value("AerOpt/inFolder").toString()));
    settingcheck &= (scratchPath==QDir::fromNativeSeparators(settings.value("AerOpt/scratchDir").toString()));
    settingcheck &= !(settings.value("Cluster/AerOptDir").toString().isEmpty());
    if(!settingcheck) {
        firstTimeSetup(AerOptWorkDir);
    }
}

//Programme entry point.
int main(int argc, char *argv[])
{
    //Initialises the QT library application event loops.
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/AerOpt.png"));

    checkSettings();

    //Application main interaction classes.
    //DebugOutput::Instance();
    qInfo() << " *** Welcome to AerOpt ***";
    qInfo() << " ***     Have a nice day     ***";

    // Setup optimisation model and selection model
    OptimisationModel* optimisationModel = new OptimisationModel();

    //Main window setup and show.
    MainWindow w;
    centerAndResizeWindow(&w);
    w.setOptimisationModel(optimisationModel);
    w.show();
    w.setSplitterSizes();

    return app.exec();
}
