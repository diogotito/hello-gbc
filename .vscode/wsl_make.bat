@set /A "JOBS=%NUMBER_OF_PROCESSORS% / 2 + 2"
@wsl make -j%JOBS% %*