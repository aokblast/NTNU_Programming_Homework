# Operating System HW2

## 1.

a. In Linux:

```bash
less <fileName>
```

b. We need system call open to open the file and read to read the file what we opened.

​	e.g. In x86_64_Linux, we have syscall 2(open) and 0(read).

## 2.

Pros: We don't have to use different function to handle different types of the target we want. (e.g. Reading signal from device or reading from file will both call read() but with different file descriptor in Linux ) 

Cons: Every task must be completed by using general purpose function(e.g. If we want to control a light of some device in Linux, we have to write the file descriptor of the device by write()  instead of just turning it up by using a more intuitive method like turnOn())

## 3.

ab. In x86_64_Linux, we can use many system call to get program's information

ptrace(syscall 101): get memory or register information of given program

dup2(syscall 33): Redirect the I/O to where you want    

rt_sigaction(syscall 13): Change action of specific signal
