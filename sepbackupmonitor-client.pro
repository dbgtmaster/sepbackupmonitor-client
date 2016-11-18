PRODUCTVERSION = 1.1-0

QT += core gui network webkit webkitwidgets

TARGET = sepbackupmonitor-client
TEMPLATE = app

RC_FILE = sepbackupmonitor-client.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

include(sepbackupmonitor-client.pri)

SOURCES += main.cpp \
    LoginWindow/LoginWindow.cpp \
    TcpMainThread.cpp \
    TcpAbstractCommand.cpp \
    MainWindow/MainWindow.cpp \
    MainWindow/VersionWindow.cpp \
    TcpCommandFactories.cpp \
    TcpAbstractCommandFactory.cpp \
    TcpCommandsHelper.cpp \
    LoginWindow/LoginManageServersWindow.cpp \
    LoginWindow/LoginConnectionsManager.cpp \
    MainWindow/TcpConnectionErrorDialog.cpp \
    System/TcpCommandsPermissions/TcpCommandsPermissions.cpp \
    System/TcpCommandsPermissions/TcpCommandsItemDelegate.cpp \
    System/TcpCommandsPermissions/TcpCommandsItemModel.cpp \
    System/TcpCommandsPermissions/TcpCommandsItem.cpp \
    Administraton/UserGroupsWindow.cpp \
    Administraton/UserGroupEditor.cpp \
    Administraton/UserGroupCreatorWindow.cpp \
    EventFilters/LabelToCheckboxEventFilter.cpp \
    MainWindow/MainMenu.cpp \
    customerAdministration/CustomerAdministration.cpp \
    customerAdministration/CustomerViewWindow.cpp \
    customerAdministration/CustomerViewModule.cpp \
    customerAdministration/CustomerViewModuleMain.cpp \
    customerAdministration/CustomerViewModuleBackupMonitor.cpp \
    BackupMonitor/BackupMonitorItemModel.cpp \
    BackupMonitor/BackupMonitorViewJob.cpp \
    MainWindow/MdiAbstractWindow.cpp \
    MainWindow/MdiAbstractWindowFactory.cpp \
    customerAdministration/CustomerAdministrationFactory.cpp \
    BackupMonitor/BackupMonitorFactory.cpp \
    BackupMonitor/BackupMonitorDelegate.cpp \
    BackupMonitor/BackupMonitorItem.cpp \
    BackupMonitor/BackupMonitorClientStatus.cpp \
    BackupMonitor/BackupMonitorListJobs.cpp \
    BackupMonitor/BackupMonitor.cpp \
    BackupMonitor/BackupMonitorClientStatusItemModel.cpp \
    BackupMonitor/BackupMonitorClientStatusItem.cpp \
    BackupMonitor/BackupMonitorClientStatusDelegate.cpp \
    System/ServerPerformanceStatistics.cpp \
    MainWindow/TcpPermissionErrorWindow.cpp \
    Utils/UtilDateTime.cpp \
    Notifications/NotificationsWindow.cpp \
    BackupMonitor/BackupMonitorClientStatusSortFilterProxy.cpp \
    BackupMonitor/BackupMonitorClientStatusMousePopup.cpp \
    BackupMonitor/BackupMonitorManageGroups.cpp \
    BackupMonitor/BackupMonitorExport.cpp \
    BackupMonitor/BackupMonitorManageGroupsEditor.cpp \
    BackupMonitor/BackupMonitorManageGroupsNew.cpp \
    BackupMonitor/BackupMonitorExportThread.cpp \
    BackupMonitor/BackupMonitorExportStatusDialog.cpp \
    Utils/html.cpp \
    BackupMonitor/BackupMonitorClientStatusPleaseWaitWidget.cpp \
    BackupMonitor/BackupMonitorDashBoard.cpp \
    BackupMonitor/BackupMonitorDashBoardNumericItem.cpp \
    BackupMonitor/BackupMonitorLogWidget.cpp

HEADERS += \
    LoginWindow/LoginWindow.h \
    TcpMainThread.h \
    TcpAbstractCommand.h \
    MainWindow/MainWindow.h \
    MainWindow/VersionWindow.h \
    TcpCommandFactories.h \
    TcpAbstractCommandFactory.h \
    TcpCommandsHelper.h \
    LoginWindow/LoginManageServersWindow.h \
    LoginWindow/LoginConnectionsManager.h \
    MainWindow/TcpConnectionErrorDialog.h \
    System/TcpCommandsPermissions/TcpCommandsPermissions.h \
    System/TcpCommandsPermissions/TcpCommandsItemDelegate.h \
    System/TcpCommandsPermissions/TcpCommandsItemModel.h \
    System/TcpCommandsPermissions/TcpCommandsItem.h \
    Administraton/UserGroupsWindow.h \
    Administraton/UserGroupEditor.h \
    Administraton/UserGroupCreatorWindow.h \
    EventFilters/LabelToCheckboxEventFilter.h \
    MainWindow/MainMenu.h \
    customerAdministration/CustomerAdministration.h \
    customerAdministration/CustomerViewWindow.h \
    customerAdministration/CustomerViewModule.h \
    customerAdministration/CustomerViewModuleMain.h \
    customerAdministration/CustomerViewModuleBackupMonitor.h \
    BackupMonitor/BackupMonitorItemModel.h \
    BackupMonitor/BackupMonitorViewJob.h \
    MainWindow/MdiAbstractWindow.h \
    MainWindow/MdiAbstractWindowFactory.h \
    customerAdministration/CustomerAdministrationFactory.h \
    BackupMonitor/BackupMonitorFactory.h \
    BackupMonitor/BackupMonitorDelegate.h \
    BackupMonitor/BackupMonitorItem.h \
    BackupMonitor/BackupMonitorClientStatus.h \
    BackupMonitor/BackupMonitorListJobs.h \
    BackupMonitor/BackupMonitor.h \
    BackupMonitor/BackupMonitorClientStatusItemModel.h \
    BackupMonitor/BackupMonitorClientStatusItem.h \
    BackupMonitor/BackupMonitorClientStatusDelegate.h \
    System/ServerPerformanceStatistics.h \
    MainWindow/TcpPermissionErrorWindow.h \
    globals.h \
    Utils/UtilDateTime.h \
    Notifications/NotificationsWindow.h \
    BackupMonitor/BackupMonitorClientStatusSortFilterProxy.h \
    BackupMonitor/BackupMonitorClientStatusMousePopup.h \
    BackupMonitor/BackupMonitorManageGroups.h \
    BackupMonitor/BackupMonitorExport.h \
    BackupMonitor/BackupMonitorManageGroupsEditor.h \
    BackupMonitor/BackupMonitorManageGroupsNew.h \
    BackupMonitor/BackupMonitorExportThread.h \
    BackupMonitor/BackupMonitorExportStatusDialog.h \
    Utils/html.h \
    BackupMonitor/BackupMonitorClientStatusPleaseWaitWidget.h \
    BackupMonitor/BackupMonitorGlobals.h \
    BackupMonitor/BackupMonitorDashBoard.h \
    BackupMonitor/BackupMonitorDashBoardNumericItem.h \
    BackupMonitor/BackupMonitorLogWidget.h

FORMS += \
    LoginWindow/LoginWindow.ui \
    MainWindow/MainWindow.ui \
    MainWindow/VersionWindow.ui \
    LoginWindow/LoginManageServersWindow.ui \
    MainWindow/TcpConnectionErrorDialog.ui \
    System/TcpCommandsPermissions/TcpCommandsPermissions.ui \
    Administraton/UserGroupsWindow.ui \
    Administraton/UserGroupEditor.ui \
    Administraton/UserGroupCreatorWindow.ui \
    customerAdministration/CustomerAdministration.ui \
    customerAdministration/CustomerViewWindow.ui \
    customerAdministration/CustomerViewModuleMain.ui \
    customerAdministration/CustomerViewModuleBackupMonitor.ui \
    BackupMonitor/BackupMonitorViewJob.ui \
    BackupMonitor/BackupMonitorClientStatus.ui \
    BackupMonitor/BackupMonitorListJobs.ui \
    BackupMonitor/BackupMonitor.ui \
    System/ServerPerformanceStatistics.ui \
    MainWindow/TcpPermissionErrorWindow.ui \
    Notifications/NotificationsWindow.ui \
    BackupMonitor/BackupMonitorCheckBackupDays.ui \
    BackupMonitor/BackupMonitorClientStatusMousePopup.ui \
    BackupMonitor/BackupMonitorManageGroups.ui \
    BackupMonitor/BackupMonitorExport.ui \
    BackupMonitor/BackupMonitorManageGroupsEditor.ui \
    BackupMonitor/BackupMonitorManageGroupsNew.ui \
    BackupMonitor/BackupMonitorExportStatusDialog.ui \
    BackupMonitor/BackupMonitorClientStatusPleaseWaitWidget.ui \
    BackupMonitor/BackupMonitorDashBoard.ui \
    BackupMonitor/BackupMonitorLogWidget.ui

RESOURCES += \
    Application.qrc

OTHER_FILES += \
    sepackupmonitor-client.rc

ICON = "graphics/favicon.ico"






















































