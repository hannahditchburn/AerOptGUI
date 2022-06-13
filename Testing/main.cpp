#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include "clusterManager.h"
#include "clusterTester.h"
#include "FileManipulation.h"

void* start_ssh_thread(void* arg)
{
    pthread_detach(pthread_self());

    std::string sshpath = "C:/Users/m.y.pei/Documents/GitHub/AerOptGUI/Testing/sshd_bash.sh";
    system(sshpath.c_str());

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t ptid;
    // Creating a new thread for SSHD
    qInfo() << "Creating separate thread to run bash script...";
    pthread_create(&ptid, NULL, &start_ssh_thread, NULL);

    QString pwd = QDir().absolutePath();
    QString ssh_home = QDir::toNativeSeparators(pwd+"/fake-ssh-home");
    if (!QDir().exists(ssh_home))
    {
        QDir().mkdir(ssh_home);
    }

    clusterTester TestObj;    

    return QTest::qExec(&TestObj, argc, argv);
}

// QTEST_APPLESS_MAIN(clusterTester)
