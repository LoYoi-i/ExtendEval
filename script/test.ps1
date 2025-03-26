# 创建管道客户端
$pipe = New-Object System.IO.Pipes.NamedPipeClientStream(
    ".",                     # 服务器名称（本地用"."）
    "my_rust_pipe",          # 管道名（不含 \\.\pipe\ 前缀）
    [System.IO.Pipes.PipeDirection]::InOut
)

# 连接管道（超时5秒）
$pipe.Connect(5000)
Write-Host "已连接到管道"

# 发送消息（UTF-8编码）
$message = "alert('456')"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($message)
$pipe.Write($bytes, 0, $bytes.Length)

# 接收响应
$buffer = New-Object byte[] 4096
$bytesRead = $pipe.Read($buffer, 0, $buffer.Length)
$response = [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytesRead)
Write-Host "收到响应: $response"

# 关闭管道
$pipe.Close()