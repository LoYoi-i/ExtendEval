use std::ffi::{CStr, CString, c_char, c_int};
use std::io::{Read, Write};
use std::os::windows::io::FromRawHandle;
use std::ptr;
use winapi::shared::winerror::ERROR_PIPE_CONNECTED;
use winapi::um::winbase::{CreateNamedPipeA, PIPE_ACCESS_DUPLEX};

const PIPE_NAME: &str = r"\\.\pipe\my_rust_pipe";
const BUFFER_SIZE: usize = 4096;

// Define the callback function type that returns a string (null-terminated C string)
type CallbackFn = extern "C" fn(data: *const u8, len: c_int) -> *const c_char;

#[unsafe(no_mangle)]
pub extern "C" fn start_pipe_server(callback: CallbackFn) -> c_int {
    pipe_server(callback);
    0
}

pub fn pipe_server(callback: CallbackFn) {
    let pipe_name = CString::new(PIPE_NAME).unwrap();

    unsafe {
        loop {
            let pipe_handle = CreateNamedPipeA(
                pipe_name.as_ptr(),
                PIPE_ACCESS_DUPLEX,
                0x00000002 | 0x00000004 | 0x00000000, // PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
                1,
                BUFFER_SIZE as u32,
                BUFFER_SIZE as u32,
                0,
                ptr::null_mut(),
            );

            if pipe_handle == winapi::um::handleapi::INVALID_HANDLE_VALUE {
                println!("Failed to create named pipe");
                continue;
            }

            println!("Waiting for client connection...");
            let connected =
                winapi::um::namedpipeapi::ConnectNamedPipe(pipe_handle, ptr::null_mut());

            if connected == 0 {
                let err = winapi::um::errhandlingapi::GetLastError();
                if err != ERROR_PIPE_CONNECTED as u32 {
                    println!("Connection failed: {}", err);
                    winapi::um::handleapi::CloseHandle(pipe_handle);
                    continue;
                }
            }

            println!("Client connected");
            let mut pipe = std::fs::File::from_raw_handle(pipe_handle as *mut _);
            let mut buffer = [0u8; BUFFER_SIZE];

            loop {
                match pipe.read(&mut buffer) {
                    Ok(size) if size > 0 => {
                        // Call the C callback with received data
                        let c_str_ptr = callback(buffer.as_ptr(), size as c_int);
                        println!("c_str_ptr: {:?}", c_str_ptr);

                        // Convert the returned C string to a Rust string
                        let response = if !c_str_ptr.is_null() {
                            CStr::from_ptr(c_str_ptr).to_string_lossy().into_owned()
                        } else {
                            "No response from callback".to_string()
                        };

                        println!("Callback returned: {}", response);

                        if let Err(e) = pipe.write_all(response.as_bytes()) {
                            println!("Failed to send response: {}", e);
                            break;
                        }

                        if let Err(e) = pipe.flush() {
                            println!("Failed to flush: {}", e);
                            break;
                        }
                    }
                    Ok(_) => break,
                    Err(e) => {
                        println!("Read error: {}", e);
                        break;
                    }
                }
            }
        }
    }
}
