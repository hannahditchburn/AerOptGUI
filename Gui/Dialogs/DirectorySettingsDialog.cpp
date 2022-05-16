#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include "DirectorySettingsDialog.h"
#include "ui_DirectorySettingsDialog.h"

DirectorySettingsDialog::DirectorySettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirectorySettingsDialog)
{
    QSettings settings;
    QString mClusterDirectory = settings.value("Cluster/AerOptDir").toString();
    QString mLocalDirectory = settings.value("AerOpt/workingDirectory").toString();
    ui->setupUi(this);
    ui->clusterdir->setText(mClusterDirectory);
    ui->localdir->setText(mLocalDirectory);    
    ui->waittime->setValue(settings.value("Cluster/WaitTime").toInt());
    ui->checktime->setValue(settings.value("Cluster/CheckTime").toInt());
    ui->updatetime->setValue(settings.value("Cluster/UpdateTime").toInt());
    // Set minimum of 5 seconds on checktime, 15 on wait time and 5 on update time.
    ui->waittime->setMinimum(15);
    ui->checktime->setMinimum(5);
    ui->updatetime->setMinimum(5);
}

DirectorySettingsDialog::~DirectorySettingsDialog()
{
    delete ui;
}

void DirectorySettingsDialog::accept()
{
    QSettings settings;

    bool ok = true; // Perform checks on inputs before committing.
    // Check that the client updates fast enough for the cluster to keep AerOpt alive.
    int waittime = ui->waittime->value();
    int checktime = ui->checktime->value();
    int updatetime = ui->updatetime->value();
    ok &= (waittime > updatetime);
    if(!ok)
    {
        QMessageBox timemsgBox;
        timemsgBox.setText("Proposed cluster wait time is smaller than update interval - AerOpt on the cluster will terminate early. Please specify a wait time larger than the update interval.");
        timemsgBox.exec();
        return;
    }
    // Check for presence of specified directory.
    QString workdir = QDir::toNativeSeparators(QDir::cleanPath(ui->localdir->text()));
    ok &= QDir(workdir).exists();
    if (!ok){
        QMessageBox dirmsgBox;
        dirmsgBox.setText("Could not find specified local working directory. Please check the path and try again.");
        dirmsgBox.exec();
        return;
    }
    // Check for spaces in path.
    ok &= (workdir.indexOf(' ') == 0);
    if(!ok)
    {
        QMessageBox spacemsgBox;
        spacemsgBox.setText("Invalid Path: AerOpt executables will not function properly if path contains spaces. Please specify a directory without spaces in the path.");
        spacemsgBox.exec();
        return;
    }
    if(ok)
    {
        DirectorySetup(workdir);
        settings.setValue("Cluster/AerOptDir", ui->clusterdir->text());
        settings.setValue("Cluster/WaitTime", waittime);
        settings.setValue("Cluster/CheckTime", checktime);
        settings.setValue("Cluster/UpdateTime", updatetime);
        QDialog::accept();
        return;
    }
}

void DirectorySettingsDialog::DirectorySetup(QString AerOptWorkDir) {
    QSettings settings;

    AerOptWorkDir = QDir::toNativeSeparators(AerOptWorkDir+"/");
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
    QDir().mkdir(inFolder);

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
}
