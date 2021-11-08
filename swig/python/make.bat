del Postbinary.py
del _Postbinary.pyd
del Postbinary_wrap.cxx
del Postbinary_wrap.o
del Pb64_32p.py
del Pb64_32p_wrap.cxx
del Pb64_32p_wrap.o

del "C:\cv_p37\Postbinary.py"
del "C:\cv_p37\_Postbinary.pyd"

mingw32-make
xcopy /Y Postbinary.py C:\cv_p37
xcopy /Y _Postbinary.pyd C:\cv_p37
pause