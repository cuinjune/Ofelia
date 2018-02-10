set ofPath=%cd%\..\..\..\..
del /s /f /q %ofPath%\addons\ofxPd\libs\libpd\
rmdir /s /q %ofPath%\addons\ofxPd\libs\libpd\
xcopy /s %cd%\updateOF\libpd %ofPath%\addons\ofxPd\libs\libpd\
xcopy /y %cd%\updateOF\ofTrueTypeFont.cpp %ofPath%\libs\openFrameworks\graphics\
pause