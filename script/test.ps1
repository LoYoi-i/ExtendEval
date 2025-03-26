# �����ܵ��ͻ���
$pipe = New-Object System.IO.Pipes.NamedPipeClientStream(
    ".",                     # ���������ƣ�������"."��
    "my_rust_pipe",          # �ܵ��������� \\.\pipe\ ǰ׺��
    [System.IO.Pipes.PipeDirection]::InOut
)

# ���ӹܵ�����ʱ5�룩
$pipe.Connect(5000)
Write-Host "�����ӵ��ܵ�"

# ������Ϣ��UTF-8���룩
$message = "alert('456')"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($message)
$pipe.Write($bytes, 0, $bytes.Length)

# ������Ӧ
$buffer = New-Object byte[] 4096
$bytesRead = $pipe.Read($buffer, 0, $buffer.Length)
$response = [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytesRead)
Write-Host "�յ���Ӧ: $response"

# �رչܵ�
$pipe.Close()