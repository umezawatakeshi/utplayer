; ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe
; $Id$

[Setup]
AppName=Ut Alpha Video Player
AppId=utplayer-x86
AppVersion=1.0.0
AppCopyright=Copyright (c) 2012 UMEZAWA Takeshi
AppPublisher=UMEZAWA Takeshi
AppPublisherURL=http://umezawa.dyndns.info/
UninstallDisplayName=Ut Alpha Video Player
DefaultDirName={pf}\utplayer
DefaultGroupName=Ut Alpha Video Player
SolidCompression=yes
OutputBaseFilename=utplayer-setup
LicenseFile=..\gplv2.rtf
InfoAfterFile=info.en.rtf

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl";
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"; InfoAfterFile: "info.ja.rtf"

[Files]
Source: "..\Release\utplayer.exe";     DestDir: "{app}"; Flags: ignoreversion;
Source: "..\gplv2.rtf";                DestDir: "{app}"; Flags: ignoreversion;
Source: "..\gplv2.txt";                DestDir: "{app}"; Flags: ignoreversion;
Source: "..\gplv2.ja.sjis.txt";        DestDir: "{app}"; Flags: ignoreversion;
Source: "..\readme.en.html";           DestDir: "{app}"; Flags: ignoreversion;
Source: "..\readme.ja.html";           DestDir: "{app}"; Flags: ignoreversion;

[Icons]
Name: "{group}\Ut Alpha Video Player";            Filename: "{app}\utplayer.exe"
Name: "{group}\License Agreement (English, RTF)"; Filename: "{app}\gplv2.rtf"
Name: "{group}\License Agreement (English)";      Filename: "{app}\gplv2.txt"
Name: "{group}\License Agreement (Japanese)";     Filename: "{app}\gplv2.ja.sjis.txt"
Name: "{group}\Readme (English)";                 Filename: "{app}\readme.en.html"
Name: "{group}\Readme (Japanese)";                Filename: "{app}\readme.ja.html"

[Run]
Filename: "{app}\readme.en.html"; Description: "View readme";   Flags: postinstall shellexec skipifsilent; Languages: en
Filename: "{app}\readme.ja.html"; Description: "readme ÇÃï\é¶"; Flags: postinstall shellexec skipifsilent; Languages: ja
