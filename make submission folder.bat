@echo off
md ..\zip_this_and_turn_in
xcopy *.h ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.cpp ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.hpp ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.c ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.sln ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.rc ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.ico ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.hlsl ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.hlsli ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.cso ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.vcproj ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.vcxproj ..\zip_this_and_turn_in\ /E /C /Y
xcopy *.xml ..\zip_this_and_turn_in\ /E /C /Y