git rev-list HEAD --count > rev.txt

for /f %%i in ('git rev-parse HEAD') do set rev=%%i

echo #define GIT_HASH L"%rev%" > hash.txt