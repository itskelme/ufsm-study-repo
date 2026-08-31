import os, pty, time, select, fcntl, termios, struct
pid, fd = pty.fork()
if pid == 0:
    os.execv("./l1-t3", ["./l1-t3", "notas.txt"]); os._exit(1)
fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", 24, 80, 0, 0))
def drain(t=0.3):
    end=time.time()+t
    while time.time()<end:
        r,_,_=select.select([fd],[],[],0.05)
        if r:
            try: os.read(fd,4096)
            except OSError: return
def send(s,t=0.25): os.write(fd,s.encode()); drain(t)
drain(0.6); send("g"); send("n"); send("g"); send("q"); drain(0.4)
try: os.waitpid(pid,0)
except OSError: pass
