#include "mainwindow.h"
#include <QApplication>

#ifndef GEN_PDB_FILE
#define GEN_PDB_FILE

#include <windows.h>
#include <dbghelp.h>
#include <QDir>
#include <QDateTime>
#include <QDebug>

//异常捕获函数
long ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    QString currPath = QDir::currentPath()+"/dmp/";
    {
        // 在程序exe的上级目录中创建dmp文件夹
        QDir *dmp = new QDir;
        bool exist = dmp->exists(currPath);
        if(exist == false)
            dmp->mkdir(currPath);
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    QString time =  current_date + ".dmp";
    EXCEPTION_RECORD *record = pException->ExceptionRecord;

    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));

    qDebug()<<"errCode: "<<errCode;
    qDebug()<<"errAddr: "<<errAddr;
    qDebug()<<"errFlag: "<<errFlag;
    qDebug()<<"errPara: "<<errPara;


    HANDLE hDumpFile = CreateFile((LPCWSTR)QString(currPath + time).utf16(),
                                  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


    if(hDumpFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
        CloseHandle(hDumpFile);
    }
    else{
        qDebug()<<"hDumpFile == null";
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

#endif // GEN_PDB_FILE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    MainWindow w;
    w.show();

    return a.exec();
}
