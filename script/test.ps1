$pipe = New-Object System.IO.Pipes.NamedPipeClientStream(
    ".", 
    "ExtendEval",
    [System.IO.Pipes.PipeDirection]::InOut
)

$pipe.Connect(5000)
Write-Host "Connected to ExtendEval"

$message = "alert('123');123;"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($message)
$pipe.Write($bytes, 0, $bytes.Length)

$buffer = New-Object byte[] 4096
$bytesRead = $pipe.Read($buffer, 0, $buffer.Length)
$response = [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytesRead)
Write-Host "Response: $response"

$pipe.Close()
