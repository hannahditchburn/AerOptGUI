#include <QtTest>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include "clusterManager.h"
#include "clusterTester.h"

clusterTester::clusterTester()
{

}

clusterTester::~clusterTester()
{

}

void clusterTester::test_login() // Ensure SSH login works correctly.
{
    QFETCH(QString, Address);
    QFETCH(QString, Username);
    QFETCH(QString, Password);
    QFETCH(int, SSH_result);

    QCOMPARE(SSH_result,sshVerifyPassword(Address, Username, Password));
}

void clusterTester::test_login_data()
{
    QTest::addColumn<QString>("Address");
    QTest::addColumn<QString>("Username");
    QTest::addColumn<QString>("Password");
    QTest::addColumn<int>("SSH_result");

    QTest::addRow("All correct") << "localhost" << "USERNAME" << "PASSWORD" << 0;
    QTest::addRow("Wrong address") << "bunsird.swansea.ac.uk" << "s.dummy.user" << "DuMmYPaSsWOrD" << 2;
}

void clusterTester::test_cluster_write() // Test ability to create files and directories via SSHExecute
{
    std::string Address = "localhost";
    std::string Username = "USERNAME";
    std::string Password = "PASSWORD";

    QString pwd = QDir().absolutePath();
    QString clustertestdir = QDir::fromNativeSeparators(pwd + "/fake-ssh-home/");

    QString testdir = QDir::toNativeSeparators(clustertestdir +"/TestDir/");
    QString testfile = QDir::toNativeSeparators(clustertestdir + "/testfile.txt");

    QFETCH(QString, Command);
    QFETCH(bool, TestDir_exists);
    QFETCH(bool, TestFile_exists);

    qInfo() << "Starting SSH session...";
    ssh_session session = createSSHSession(Address,Username,Password);
    if (session == NULL)
    {
        qInfo() << "Unable to create SSH session.";
        QFAIL("SSH connection failure - ensure sshd is running.");
    }
    else
    {
    qInfo() << "Attempting to execute specified command...";
    sshExecute(session, Command.toStdString());

    qInfo() << "Short sleep to allow command to execute...";
    sleep(5);

    bool dir_exists = QDir().exists(testdir);
    bool file_exists = QFile().exists(testfile);

    QCOMPARE(dir_exists,TestDir_exists);
    QCOMPARE(file_exists,TestFile_exists);

    ssh_disconnect(session);
    ssh_free(session);
    }
}

void clusterTester::test_cluster_write_data()
{
    QTest::addColumn<QString>("Command");
    QTest::addColumn<bool>("TestDir_exists");
    QTest::addColumn<bool>("TestFile_exists");

    QString pwd = QDir().absolutePath();

    QString prelim_command = "cd '" + QDir::toNativeSeparators(pwd + "/fake-ssh-home")+"'; ";

    QTest::addRow("Nothing useful") << prelim_command+"echo 'Zura ja nai, Katsura da!'" << false << false;
    QTest::addRow("Touch new file") << prelim_command+"touch testfile.txt; echo Test 2 >> testfile.txt" << false << true;
    QTest::addRow("Make new directory") << prelim_command+"mkdir TestDir; touch TestDir/dirfile.txt; echo Test 3 >> testfile.txt" << true << true;
    QTest::addRow("Remove directory") << prelim_command+"rm -r TestDir; echo Test 4 >> testfile.txt" << false << true;
    QTest::addRow("Remove file") << prelim_command+"rm testfile.txt" << false << false;
}

void clusterTester::test_cluster_download()
{
    std::string Address = "localhost";
    std::string Username = "USERNAME";
    std::string Password = "PASSWORD";

    QFETCH(QString, Folder);
    QFETCH(bool, File1_exists);
    QFETCH(bool, File2_exists);
    QFETCH(bool, File3_exists);

    QString pwd = QDir().absolutePath();
    QString localtestdir = QDir::toNativeSeparators(pwd+"/DownloadTesting/"+Folder);
    QString clustertestdir = QDir::fromNativeSeparators(pwd+"/fake-ssh-home");

    QString File1Path = QDir::toNativeSeparators(localtestdir+"/File1.txt");
    QString File2Path = QDir::toNativeSeparators(localtestdir+"/File2.txt");
    QString File3Path = QDir::toNativeSeparators(localtestdir+"/File3.txt");

    if (!QDir().exists(localtestdir))
    {
        qInfo() << "Creating destination directory: " << localtestdir;
        QDir().mkdir(localtestdir);
    }
    if (!QDir().exists(clustertestdir))
    {
        qInfo() << "Creating mock SSH home directory: " << clustertestdir;
        QDir().mkdir(clustertestdir);
    }
    if (!QDir().exists(clustertestdir+"/FolderA/."))
    {
        qInfo() << "Populating mock SSH directory with files and folders...";
        QString gen_test_files_script = QDir::toNativeSeparators(pwd+"/gen_test_files.sh");
        system(gen_test_files_script.toStdString().c_str());
    }

    qInfo() << "Starting SSH session...";
    ssh_session session = createSSHSession(Address,Username,Password);
    if (session == NULL)
    {
        qInfo() << "Unable to create SSH session.";
        QFAIL("SSH connection failure - ensure sshd is running.");
    }
    else
    {
        sftp_session sftp = createSFTPSession(session);
        if (sftp==NULL)
        {
            qInfo() << "Unable to create SFTP session.";
            QFAIL("SFTP session failure.");
        }
        else
        {
            qInfo() << "Attempting to download target folder with SFTP...";
            getClusterFolder((clustertestdir+"/"+Folder).toStdString(),localtestdir.toStdString(),session,sftp);
        }
        sftp_free(sftp);

    qInfo() << "Short sleep to allow files to transfer...";
    sleep(10);

    ssh_disconnect(session);
    ssh_free(session);
    }

    bool File1Download = QDir().exists(File1Path);
    bool File2Download = QDir().exists(File2Path);
    bool File3Download = QDir().exists(File3Path);

    qInfo() << "Short sleep before directory cleanup...";
    sleep(5);
    if (File1Download)
    {
        qInfo() << "Deleting File1.txt...";
        QFile().remove(File1Path);
    }
    if (File2Download)
    {
        qInfo() << "Deleting File2.txt...";
        QFile().remove(File2Path);
    }
    if (File3Download)
    {
        qInfo() << "Deleting File3.txt...";
        QFile().remove(File3Path);
    }

    QCOMPARE(File1Download,File1_exists);
    QCOMPARE(File2Download,File2_exists);
    QCOMPARE(File3Download,File3_exists);
}

void clusterTester::test_cluster_download_data()
{
    QTest::addColumn<QString>("Folder");
    QTest::addColumn<bool>("File1_exists");
    QTest::addColumn<bool>("File2_exists");
    QTest::addColumn<bool>("File3_exists");

    QTest::addRow("Only 1") << "FolderA" << true << false << false;
    QTest::addRow("Only 2") << "FolderB" << false << true << false;
    QTest::addRow("Both 1 and 2") << "FolderAB" << true << true << false;
    QTest::addRow("All files") << "FolderABC" << true << true << true;
}
