/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/


//!    File   : ImageVis3D.h
//!    Author : Jens Krueger
//!             SCI Institute
//!             University of Utah
//!    Date   : September 2008
//
//!    Copyright (C) 2008 SCI Institute

#pragma once

#ifndef IMAGEVIS3D_H
#define IMAGEVIS3D_H

#include <StdDefines.h>
class MasterController;
class TextfileOut;

#include "AutoGen/ui_ImageVis3D.h"
#include "RenderWindowGL.h"
#if defined(_WIN32) && defined(USE_DIRECTX)
#include "RenderWindowDX.h"
#endif

#include "Q1DTransferFunction.h"
#include "Q2DTransferFunction.h"
#include "DebugOut/QTOut.h"
#include <UI/SettingsDlg.h>
#include <UI/Welcome.h>

#include <QtCore/QTimer>

#include <string>
#include <vector>

class QHttp;
class QHttpResponseHeader;
class QAuthenticator;
class QFile;
class QSslError;
class FTPDialog;

class MainWindow : public QMainWindow, protected Ui_MainWindow, public Scriptable
{
  Q_OBJECT

  public:
    MainWindow(MasterController& masterController,
               bool bScriptMode = false,   // suppress dialog boxes
               QWidget* parent = 0,
               Qt::WindowFlags flags = 0);

    virtual ~MainWindow();
    
    QTOut* GetDebugOut() {return m_pDebugOut;}

    // Scriptable implementation
    virtual void RegisterCalls(Scripting* pScriptEngine);
    virtual bool Execute(const std::string& strCommand, const std::vector< std::string >& strParams, std::string& strMessage);

    bool RunScript(const std::string& strFilename);
    bool StayOpen() const {return m_bStayOpenAfterScriptEnd;}

  public slots:
    void SetRenderProgress(unsigned int iLODCount, unsigned int iCurrentCount, unsigned int iBrickCount, unsigned int iWorkingBrick);

  protected slots:
    void FtpFail();
    void FtpSuccess();

    void SetCaptureFilename();
    void CaptureFrame();
    void CaptureSequence();
    void CaptureRotation();
    void LoadDataset();
    void LoadDataset(std::string strFilename) {LoadDataset(QString(strFilename.c_str()));}
    void LoadDirectory();
    void CloseCurrentView();
    void ResizeCurrentView(int iSizeX, int iSizeY);
    void CloneCurrentView();
    void CheckForUpdates();
    void OnlineHelp();
    void OnlineVideoTut();
    void CloseWelcome();
    void ReportABug();

    void ToggleRenderWindowView2x2();
    void ToggleRenderWindowViewSingle();

    void Use1DTrans();
    void Use2DTrans();
    void UseIso();
    void DisableAllTrans();

    void Transfer1DSetColors();
    void Transfer1DSetGroups();
    void Transfer1DLoad();
    bool Transfer1DLoad(std::string strFilename);
    void Transfer1DSave();
    void Transfer1DCopyTo2DTrans();

    void Transfer2DAddGradient();
    void Transfer2DDeleteGradient();
    void Transfer2DChooseGradientColor();
    void Transfer2DChooseGradientOpacity();
    void Transfer2DToggleGradientType();
    void Transfer2DLoad();
    bool Transfer2DLoad(std::string strFilename);
    void Transfer2DSave();

    void SetUpdateMode();
    void ApplyUpdate();

    void Transfer2DSwatchesChanged();
    void Transfer2DUpdateSwatchButtons();
    void Transfer2DUpdateGradientType();
    void Transfer2DUpdateGradientBox();
    void Transfer2DUpdateGradientButtons();

    void FilterImage();

    bool LoadWorkspace();
    bool SaveWorkspace();
    bool ApplyWorkspace();

    bool LoadGeometry();
    bool SaveGeometry();

    void OpenRecentFile();
    void ClearMRUList();

    void UpdateMenus();
    void ExportDataset();
    void MergeDatasets();
    bool ExportDataset(UINT32 iLODLevel, std::string targetFileName);
    void ExportMesh();
    bool ExportMesh(UINT32 iLODLevel, std::string targetFileName);

    void RenderWindowActive(RenderWindow* sender);
    void RenderWindowClosing(RenderWindow* sender);
    void StereoDisabled();
    void RenderWindowViewChanged(int iMode);

    void ShowVersions();
    void ShowGPUInfo(bool bWithExtensions);
    void ShowSysInfo();
    void ListSupportedImages();
    void ClearDebugWin();
    void ParseAndExecuteDebugCommand();
    void SetDebugViewMask();

    void CheckForRedraw();
    bool ShowSettings() {return ShowSettings(false);}
    bool ShowSettings(bool bInitializeOnly);
    void SetLighting(bool bLighting);

    void Collapse2DWidgets();
    void Expand2DWidgets();
    void SetSampleRate(int iValue);
    void SetIsoValue(float fValue);
    void SetIsoValue(int iValue);
    void SetClearViewIsoValue(float fValue);
    void ToggleGlobalBBox(bool bRenderBBox);
    void ToggleLocalBBox(bool bRenderBBox);
    void ToggleClipPlane(bool);
    void ClipToggleLock(bool);
    void ClipToggleShow(bool);

    void SetRescaleFactors();
    virtual void closeEvent(QCloseEvent *event);

    // update
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);

    void UploadLogToServer();

    void ShowAbout();
    void ChooseIsoColor();

    void ToggleFullscreen();
    void ToggleClearView();
    void SetFocusIsoValue(int iValue);
    void ChooseFocusColor();
    void SetFocusSize(int iValue);
    void SetContextScale(int iValue);
    void SetBorderSize(int iValue);

    void UpdateLockView();
    void LockModalityChange();
    void ChangeLocks();

    void ToggleStereoRendering();
    void SetStereoEyeDistance();
    void SetStereoFocalLength();

    void Show1DTrans();
    void Show2DTrans();
    void ShowIsoEdit();
    void PreserveTransparencyChanged();

    void ShowWelcomeScreen();
    void SaveDefaultWorkspace();
    void SaveDefaultGeometry();

  private :
    QTimer*                                   m_pRedrawTimer;
    MasterController&                         m_MasterController;
    QString                                   m_strCurrentWorkspaceFilename;
    Q1DTransferFunction*                      m_1DTransferFunction;
    Q2DTransferFunction*                      m_2DTransferFunction;
    QGLWidget*                                m_glShareWidget;
    QTOut*                                    m_pDebugOut;
    static const unsigned int                 ms_iMaxRecentFiles = 5;
    QAction*                                  m_recentFileActs[ms_iMaxRecentFiles];
    FLOATVECTOR3                              m_vBackgroundColors[2];
    FLOATVECTOR4                              m_vTextColor;
    bool                                      m_bShowVersionInTitle;
    bool                                      m_bQuickopen;
    unsigned int                              m_iMinFramerate;
    unsigned int                              m_iLODDelay;
    unsigned int                              m_iActiveTS;
    unsigned int                              m_iInactiveTS;
    bool                                      m_bWriteLogFile;
    QString                                   m_strLogFileName;
    unsigned int                              m_iLogLevel;

    WelcomeDialog*                            m_pWelcomeDialog;

    unsigned int                              m_iBlendPrecisionMode;
    bool                                      m_bPowerOfTwo;
    bool                                      m_bDownSampleTo8Bits;
    bool                                      m_bDisableBorder;
    bool                                      m_bAvoidCompositing;
    bool                                      m_bAutoSaveGEO;
    bool                                      m_bAutoSaveWSP;
    MasterController::EVolumeRendererType     m_eVolumeRendererType;
    bool                                      m_bUpdatingLockView;
    QString                                   m_strLogoFilename;
    int                                       m_iLogoPos;
    bool                                      m_bAutoLockClonedWindow;
    bool                                      m_bAbsoluteViewLocks;
    bool                                      m_bCheckForUpdatesOnStartUp;
    bool                                      m_bCheckForDevBuilds;
    bool                                      m_bShowWelcomeScreen;

    bool                                      m_bStayOpenAfterScriptEnd;

    RenderWindow* WidgetToRenderWin(QWidget* w);
    RenderWindow* CreateNewRenderWindow(QString dataset);
    bool CheckRenderwindowFitness(RenderWindow *renderWin, bool bIfNotOkShowMessageAndCloseWindow=true);

    void SetupWorkspaceMenu();
    bool LoadWorkspace(QString strFilename,
           bool bSilentFail = false,
           bool bRetryResource = true);

    bool SaveWorkspace(QString strFilename);

    bool LoadGeometry(QString strFilename,
          bool bSilentFail = false,
          bool bRetryResource = true);

    bool SaveGeometry(QString strFilename);

    bool LoadDefaultWorkspace();
    bool LoadDefaultGeometry();

    void SetTitle();
    void setupUi(QMainWindow *MainWindow);

    void UpdateMRUActions();
    void AddFileToMRUList(const QString &fileName);

    void GetDebugViewMask();

    bool LoadDataset(const std::vector< std::string >& strParams);
    bool LoadDataset(QString fileName, QString targetFileName="", bool bNoUserInteraction=false);

    QString GetConvFilename();

    void InitAllWorkspaces();

    void CheckSettings();
    void ApplySettings();
    void ApplySettings(RenderWindow* renderWin);
    void SetSampleRateSlider(int iValue);
    void UpdateSampleRateLabel(int iValue);
    void SetIsoValueSlider(int iValue, int iMaxValue);
    void UpdateIsoValLabel(int iValue, int iMaxValue);
    void SetToggleGlobalBBoxLabel(bool bRenderBBox);
    void SetToggleLocalBBoxLabel(bool bRenderBBox);
    void SetToggleClipEnabledLabel(bool);
    void SetToggleClipShownLabel(bool);
    void SetToggleClipLockedLabel(bool);
    void ClearProgressView();

    void ToggleClearViewControls(int iRange);
    void SetFocusIsoValueSlider(int iValue, int iMaxValue);
    void UpdateFocusIsoValLabel(int iValue, int iMaxValue);
    void SetFocusSizeValueSlider(int iValue);
    void SetContextScaleValueSlider(int iValue);
    void SetBorderSizeValueSlider(int iValue);

    void CompareFiles(const std::string& strFile1, const std::string& strFile2) const;

    void RemoveAllLocks(RenderWindow* sender);
    void RemoveAllLocks(RenderWindow* sender, size_t iLockType);
    bool SetLock(size_t iLockType, RenderWindow* winA, RenderWindow* winB);
    bool IsLockedWith(size_t iLockType, RenderWindow* winA, RenderWindow* winB);

    bool CaptureFrame(const std::string& strTargetName);
    bool CaptureSequence(const std::string& strTargetName, std::string* strRealFilename=NULL);

    void RotateCurrentViewX(double angle);
    void RotateCurrentViewY(double angle);
    void RotateCurrentViewZ(double angle);
    void TranslateCurrentView(double x, double y, double z);

    void ShowInformationDialog(QString strTitle, QString strMessage);
    void ShowWarningDialog(QString strTitle, QString strMessage);
    void ShowCriticalDialog(QString strTitle, QString strMessage);

    void SetAndCheckRunningFlag();
    void RemoveRunningFlag();
    void ReportABug(const std::string& strFile);
    void ToggleLogFile();

    TextfileOut* m_pTextout;

    RenderWindow* ActiveRenderWin();
    QMdiSubWindow* ActiveSubWindow();
    RenderWindow* m_pActiveRenderWin;

    // update
    QHttp* m_pHttp;
    QFile* m_pUpdateFile;
    int    m_iHttpGetId;
    bool   m_bStartupCheck;
    bool   m_bScriptMode;
    void CheckForUpdatesInternal();
    void QuietCheckForUpdates();
    bool GetVersionsFromUpdateFile(const std::string& strFilename, float& fIV3DVersion, int& iIV3DSVNVersion, float& fTuvokVersion, int& iTuvokSVNVersion);
    void DeleteUpdateFile();

    // ftp
    FTPDialog*   m_pFTPDialog;
    std::string  m_strFTPTempFile;
    bool         m_bFTPDeleteSource;
    bool         m_bFTPFinished;
    bool         FtpTransfer(std::string strSource, std::string strDest, bool bDeleteSource = true);
    std::string  GenUniqueName(const std::string& strPrefix, const std::string& strExt="txt");
    bool         Pack(const std::vector< std::string >& strParams);

    bool m_bClipDisplay;
    bool m_bClipLocked;

    friend class MergeDlg;
};

#endif // IMAGEVIS3D_H
