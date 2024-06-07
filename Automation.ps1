
$random = Get-Random -Minimum 150 -Maximum 1000
[System.Console]::Beep($random, 200)
Start-Sleep -Milliseconds 300
exit 0 # success