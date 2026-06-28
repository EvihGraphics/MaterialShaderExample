param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$EvidenceRoot = "$PWD\LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-H-1-Real-Headless-GPU",
    [switch]$Restore
)

if (!(Test-Path $EvidenceRoot)) {
    New-Item -ItemType Directory -Force -Path $EvidenceRoot | Out-Null
}

$Descriptors = @(
    "$UERoot\Engine\Plugins\Experimental\PlainPropsEngine\PlainPropsEngine.uplugin",
    "$UERoot\Engine\Plugins\Experimental\PlainPropsUObject\PlainPropsUObject.uplugin"
)

$StateFile = "$EvidenceRoot\02_PluginDescriptorCheck\InvalidPluginDescriptors.json"

if ($Restore) {
    if (Test-Path $StateFile) {
        $State = Get-Content $StateFile | ConvertFrom-Json
        foreach ($Item in $State) {
            $OriginalPath = $Item.OriginalPath
            $BackupPath = $Item.BackupPath
            $OriginalHash = $Item.OriginalHash

            if (Test-Path $BackupPath) {
                Move-Item -Force $BackupPath $OriginalPath
                
                $NewHash = (Get-FileHash $OriginalPath -Algorithm SHA256).Hash
                if ($NewHash -ne $OriginalHash) {
                    Write-Host "Hash mismatch after restore for $OriginalPath"
                    exit 8
                }
                
                if (Test-Path $BackupPath) {
                    Write-Host "Backup file still exists after restore: $BackupPath"
                    exit 8
                }
            } else {
                Write-Host "Restore failed: Backup file not found $BackupPath"
                exit 8
            }
        }
    }
    exit 0
}

$InvalidList = @()

foreach ($Desc in $Descriptors) {
    if (Test-Path $Desc) {
        try {
            $Json = Get-Content $Desc -Raw | ConvertFrom-Json -ErrorAction Stop
        } catch {
            $Hash = (Get-FileHash $Desc -Algorithm SHA256).Hash
            $Backup = "$Desc.bak"
            Move-Item -Force $Desc $Backup
            Write-Host "Disabled damaged plugin descriptor: $Desc"
            $InvalidList += @{
                OriginalPath = $Desc
                BackupPath = $Backup
                OriginalHash = $Hash
            }
        }
    }
}

if ($InvalidList.Count -gt 0) {
    if (!(Test-Path "$EvidenceRoot\02_PluginDescriptorCheck")) {
        New-Item -ItemType Directory -Force -Path "$EvidenceRoot\02_PluginDescriptorCheck" | Out-Null
    }
    $InvalidList | ConvertTo-Json | Out-File $StateFile -Force
}

exit 0
