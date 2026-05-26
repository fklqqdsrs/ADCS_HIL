param (
    [string]$com_port,
    [int]$com_baud,
    [string]$project_dir,
    [string]$report_path,
    [string]$log_path
)

$ErrorActionPreference = 'Stop'

Write-Host "--- Smoke Test Runner ---"
Write-Host "Port: $com_port, Baud: $com_baud"

$manifests = Get-ChildItem -Path $project_dir -Filter "smoke_manifest.json" -Recurse
Write-Host "Found $($manifests.Count) manifests."

$all_results = New-Object System.Collections.Generic.List[PSObject]
$sp = New-Object System.IO.Ports.SerialPort $com_port, $com_baud, "None", 8, "One"
$sp.ReadTimeout = 2000
$sp.WriteTimeout = 1000
$sp.NewLine = "`n"

$log_file = New-Item -ItemType File -Path $log_path -Force
function Log-Output($msg) {
    $ts = Get-Date -Format "HH:mm:ss"
    $line = "[$ts] $msg"
    Write-Host $line
    Add-Content -Path $log_path -Value $line
}

function Wait-For-Prompt($timeout_s = 2) {
    $deadline = (Get-Date).AddSeconds($timeout_s)
    while ((Get-Date) -lt $deadline) {
        try {
            $line = $sp.ReadLine().Trim()
            if ($line -like "*TSC-1 >*") { return $true }
        } catch { }
    }
    return $false
}

function Flush-Buffer {
    Start-Sleep -Milliseconds 300
    while ($sp.BytesToRead -gt 0) {
        try { $null = $sp.ReadExisting() } catch { break }
        Start-Sleep -Milliseconds 100
    }
}

try {
    $sp.Open()
    Log-Output "Serial port $com_port opened."
    
    # Initial Reset
    Flush-Buffer
    $sp.WriteLine("")
    Start-Sleep -Milliseconds 1000
    Flush-Buffer

    foreach ($mFile in $manifests) {
        $manifest = Get-Content $mFile.FullName | ConvertFrom-Json
        Log-Output ">> Application: $($manifest.application)"
        
        # Gap between applications
        Start-Sleep -Seconds 2
        $sp.WriteLine("")
        Flush-Buffer

        foreach ($scenario in $manifest.scenarios) {
            Log-Output "  Test: $($scenario.name) (Command: '$($scenario.command)')"
            
            # Ensure prompt is ready
            Flush-Buffer
            $sp.WriteLine($scenario.command)
            Start-Sleep -Milliseconds 200
            
            $found = $false
            $timeout = if ($scenario.timeout_s) { $scenario.timeout_s } else { 5 }
            $deadline = (Get-Date).AddSeconds($timeout)
            $captured_lines = New-Object System.Collections.Generic.List[string]
            
            while ((Get-Date) -lt $deadline) {
                try {
                    $line = $sp.ReadLine().Trim()
                    if ($line) {
                        # Always log UART output for debugging
                        Log-Output "    UART: $line"
                        $captured_lines.Add($line) | Out-Null
                        if ($line -match [regex]::Escape($scenario.expect)) {
                            $found = $true
                            # For list commands, keep reading until prompt
                            if ($scenario.name -notmatch "list|stat_all") { break }
                        }
                    }
                } catch {
                    if ($found) { break }
                }
            }
            
            # Wait for command to finish and prompt to return
            Wait-For-Prompt -timeout_s 2 | Out-Null
            
            $all_results.Add([PSCustomObject]@{
                app = $manifest.application
                name = $scenario.name
                success = $found
                expect = $scenario.expect
                output = $captured_lines -join "`n"
            })
            
            if ($found) {
                Log-Output "  SUCCESS"
            } else {
                Log-Output "  FAILED (Pattern '$($scenario.expect)' not found)"
            }
            
            # Small delay between commands
            Start-Sleep -Milliseconds 500
        }
    }
} catch {
    Log-Output "FATAL ERROR: $($_.Exception.Message)"
    throw $_
} finally {
    if ($sp.IsOpen) { $sp.Close() }
    Log-Output "Serial port closed."
}

# --- Generate JUnit XML ---
Log-Output "Generating JUnit report at $report_path"
$total = $all_results.Count
$failures = ($all_results | Where-Object { -not $_.success }).Count

$xml = [xml]"<testsuite name='SmokeTests' tests='$total' failures='$failures'></testsuite>"
foreach ($res in $all_results) {
    $node = $xml.CreateElement("testcase")
    $node.SetAttribute("classname", "smoke.$($res.app)")
    $node.SetAttribute("name", $res.name)
    if (-not $res.success) {
        $failNode = $xml.CreateElement("failure")
        $failNode.SetAttribute("message", "Expected pattern not found: $($res.expect)")
        $failNode.InnerText = "Full output captured:`n$($res.output)"
        $node.AppendChild($failNode) | Out-Null
    }
    $xml.DocumentElement.AppendChild($node) | Out-Null
}
$xml.Save($report_path)

if ($failures -gt 0) {
    Write-Host "Smoke tests failed with $failures failures." -ForegroundColor Red
    exit 1
} else {
    Write-Host "All smoke tests passed!" -ForegroundColor Green
    exit 0
}
