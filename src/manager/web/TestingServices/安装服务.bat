%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\installutil.exe "%~dp0bin\Debug\TestingServicesFire.exe"
Net Start ServiceTest
sc config ServiceTest start= auto