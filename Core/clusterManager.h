#ifndef clusterManager_H
#define clusterManager_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <QSettings>
#include <QThread>
#include <QTimer>

#define SSH_NO_CPP_EXCEPTIONS
#define LIBSSH_STATIC 1
#include <libssh/libsshpp.hpp>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>

class clusterManager : public QThread
{
    Q_OBJECT
public:
    clusterManager();
    ~clusterManager();

    void setWorkingDirectory(QString workDir);
    void setClusterAddress(QString address);
    void setUsername(QString username);
    void setPassword(QString password);
    // Included to allow AerOptGUI to correctly pull data from cluster
    void setRunDateTime();

signals:
    void directoryChanged(const QString&);
    void stdOut(const QString);
    void stdErr(const QString);

private:

    std::string mWorkingDirectory = "";
    std::string mAddress = "";
    std::string mUsername = "";
    std::string mPassword = "";

    void run();
    void folderCheckLoop();
    int submitToCluster();

    int folderFromCluster(std::string source, std::string destination);
    int fileFromCluster(std::string source, std::string destination);

};


class clusterUpdate : public QThread
{
    Q_OBJECT
public:
    clusterUpdate();
    ~clusterUpdate();
    void setWorkingDirectory(std::string workDir);
    void setClusterAddress(std::string address);
    void setUsername(std::string username);
    void setPassword(std::string password);
signals:
    void updatePerformed(const QString);
private:
    std::string mWorkingDirectory = "";
    std::string mAddress = "";
    std::string mUsername = "";
    std::string mPassword = "";

    void run();
    int updateStatus();
};

ssh_session createSSHSession( std::string address, std::string username, std::string password );
ssh_channel createSSHChannel(ssh_session session);
sftp_session createSFTPSession(ssh_session session);

int sshVerifyPassword( QString address, QString username, QString password );

void sshExecute(ssh_session session, std::string command);

int fileToCluster(std::string source, std::string destination, ssh_session session);
int getClusterFile(std::string source, std::string destination, ssh_session session, sftp_session sftp);
int getClusterFolder(std::string source, std::string destination, ssh_session session, sftp_session sftp);
int downloadAndVerifyClusterFile(std::string source, std::string destination, int size, ssh_session session, sftp_session sftp);

#endif // clusterManager_H
