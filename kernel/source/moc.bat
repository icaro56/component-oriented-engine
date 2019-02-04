@echo .

..\..\DEPENDENCES\qt\bin\moc ..\include\MainWindow.h -o .\moc_MainWindow.cpp
..\..\DEPENDENCES\qt\bin\moc ..\include\Core.h -o .\moc_Core.cpp
..\..\DEPENDENCES\qt\bin\moc ..\include\components\physics\Collider.h -o .\components\physics\moc_Collider.cpp
..\..\DEPENDENCES\qt\bin\moc ..\include\components\physics\CharacterController.h -o .\components\physics\moc_CharacterController.cpp
..\..\DEPENDENCES\qt\bin\moc ..\include\components\script\Script.h -o .\components\script\moc_Script.cpp
pause
