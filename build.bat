mkdir build
cd build
cmake ../

cd src
mkdir default_detectors
cd ../../data/default_detectors
tar -xf models.rar 
tar -xf haarcascades.rar
tar -xf lbpcascades.rar

xcopy "models" "../../build/src/default_detectors/models" /h /i /c /k /e /r /y
xcopy "haarcascades" "../../build/src/default_detectors/haarcascades" /h /i /c /k /e /r /y
xcopy "lbpcascades" "../../build/src/default_detectors/lbpcascades" /h /i /c /k /e /r /y

xcopy "../detector_paths" "../../build/src/detector_paths" /h /i /c /k /e /r /y
rmdir models /s/q
rmdir haarcascades /s/q
rmdir lbpcascades /s/q

pause
