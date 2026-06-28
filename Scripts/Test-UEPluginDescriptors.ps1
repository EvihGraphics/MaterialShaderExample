param (
    [string]$UERoot = "D:\UE\UnrealEngine_Animation_Tech",
    [string]$EvidenceRoot = "LocalVisualResults\UE57\HIVE_4090x2\UE4-2A-1-H-Headless-Automated-Acceptance",
    [switch]$Restore
)

if (!(Test-Path $EvidenceRoot)) {
    New-Item -ItemType Directory -Force -Path $EvidenceRoot | Out-Null
}

$Descriptors = @(
    "$UERoot\Engine\Plugins\Experimental\PlainPropsEngine\PlainPropsEngine.uplugin",
    "$UERoot\Engine\Plugins\Experimental\PlainPropsUObject\PlainPropsUObject.uplugin"
)

if ($Restore) {
    foreach ($Desc in $Descriptors) {
        $Backup = "$Desc.bak"
        if (Test-Path $Backup) {
            Move-Item -Force $Backup $Desc
            Write-Host "Restored $Desc"
        }
    }
    exit 0
}

$InvalidList = @()

foreach ($Desc in $Descriptors) {
    if (Test-Path $Desc) {
        $Backup = "$Desc.bak"
        Move-Item -Force $Desc $Backup
        Write-Host "Disabled damaged plugin descriptor: $Desc"
        $InvalidList += $Desc
    }
}

$InvalidList | ConvertTo-Json | Out-File "$EvidenceRoot\02_PluginDescriptorCheck\InvalidPluginDescriptors.json" -Force

exit 0
