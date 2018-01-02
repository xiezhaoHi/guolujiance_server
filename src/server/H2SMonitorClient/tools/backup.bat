@ECHO OFF
@REM 
@REM Author: tangqiao
@REM Date:   2015-08-07
@REM Purpose:backup pdb and exe
@REM 
@REM Usage: backup_pdb.bat <OutDir> <TargetName> 
@REM 
@REM 



IF "%2"=="" ( GOTO SHOW_USAGE )
IF "%1"=="" ( GOTO SHOW_USAGE )


@SET OUTDIR=%1
@SET TARGETNAME=%2
@SET BACKUPDIR=backup
@SET BUILDDATE=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%
@SET BUILDTIME=%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%


@REM ���������ļ���
IF NOT EXIST %OUTDIR%\%BACKUPDIR% (MKDIR %OUTDIR%\%BACKUPDIR%)

@REM �������������ļ���
IF NOT EXIST %OUTDIR%\%BACKUPDIR%\%BUILDDATE% (MKDIR %OUTDIR%\%BACKUPDIR%\%BUILDDATE%)

@REM ��������ʱ���ļ���
IF NOT EXIST %OUTDIR%\%BACKUPDIR%\%BUILDDATE%\%BUILDTIME% (MKDIR %OUTDIR%\%BACKUPDIR%\%BUILDDATE%\%BUILDTIME%)

@REM ����pdb&exe
@ECHO ���� %TARGETNAME%.exe
COPY %OUTDIR%%TARGETNAME%.exe %OUTDIR%\%BACKUPDIR%\%BUILDDATE%\%BUILDTIME%\
@ECHO ���� %TARGETNAME%.pdb
COPY %OUTDIR%%TARGETNAME%.pdb %OUTDIR%\%BACKUPDIR%\%BUILDDATE%\%BUILDTIME%\


@REM TIMEOUT /T 1
@EXIT


:SHOW_USAGE

@ECHO Usage: backup.bat OutDir TargetName

@PAUSE
@EXIT