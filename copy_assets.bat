cd build/src
mkdir data
cd ../../data
tar -xf models.rar 
tar -xf haarcascades.rar
tar -xf lbpcascades.rar

xcopy "models" "../build/src/data/models" /h /i /c /k /e /r /y
xcopy "haarcascades" "../build/src/data/haarcascades" /h /i /c /k /e /r /y
xcopy "lbpcascades" "../build/src/data/lbpcascades" /h /i /c /k /e /r /y
rmdir models /s/q
rmdir haarcascades /s/q
rmdir lbpcascades /s/q

cd ../
xcopy "assets" "build/src/assets" /h /i /c /k /e /r /y
y
y
y