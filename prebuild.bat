git rev-list HEAD --count > rev.txt

for /f %%i in ('git rev-parse HEAD') do set rev=%%i
for /f %%i in ('git rev-parse --abbrev-ref HEAD') do set branch=%%i

echo #define GIT_HASH L"%rev%" > hash.txt
echo #define GIT_BRANCH L"%branch%" >> hash.txt