#!/usr/bin/awk -f
{
  if ($3=="") next;
  if ($3 ~ /^[.]/) next;
  adr=strtonum($1);
  name=$3;
  if (pthread) {
    if ((name ~ /^pthread_/) || (name ~ /^__pthread_/) || (name ~ /^__new_sem/) || (name ~ /^raise/) || (adr >= 0x780a80 && adr < 0x787e48))
    {
      if (pthread==2) next;
    } else {
      if (pthread==1) next;
    }
  }
  if (libc) {
    if (adr >= 0x137cfe0)
    {
      if (libc==2) next;
    } else {
      if (libc==1) next;
    }
  }
  
  if (name ~ /^_x_/) next;
  sub(/::/,"__",name);
  sub(/\r$/,"",name);
  printf("  %s = 0x%06x;\n", name, adr);
  weak=name;

  if (name ~ /^__libc_(accept|calloc|close|connect|creat|errno|fcntl|free|fsync|h_errno|lseek(64)?|mallinfo|malloc|mallopt|memalign|msgrcv|msgsnd|msync|nanosleep|open(64)?|pause|pipe|poll|pread(64)?|pselect|pvalloc|pwrite(64)?|readv?|realloc|recv|recvfrom|recvmsg|select|send|sendmsg|sendto|siglongjmp|sigpause|sigsuspend|sigwait|sigwaitinfo|tcdrain|valloc|wait|waitid|waitpid|writev?|__xpg_sigpause)$/) {
    sub(/^__libc_/,"",weak);
  }

  if (name ~ /^_IO_(fclose|fdopen|feof|ferror|fflush|fgetpos|fgetpos64|fgets|flockfile|fopen|fopen64|fopencookie|fprintf|fputs|fread|fsetpos|fsetpos64|ftell|ftrylockfile|funlockfile|fwrite|getc|getdelim|gets|getwc|obstack_printf|obstack_vprintf|popen|printf|putc|puts|setbuffer|setvbuf|sprintf|sscanf|stderr|stdin|stdout|ungetc|vasprintf|vdprintf|vfprintf|vfwprintf|vscanf|vsnprintf|vsprintf|vsscanf|vswprintf)$/) {
    sub(/^_IO_/,"",weak);
  }
 
  if (name ~ /^__new_(getaliasbyname_r|getaliasent_r|getgrent_r|getgrgid_r|getgrnam_r|gethostbyaddr_r|gethostbyname2_r|gethostbyname_r|gethostent_r|getnetbyaddr_r|getnetbyname_r|getnetent_r|getprotobyname_r|getprotobynumber_r|getprotoent_r|getpwent_r|getpwnam_r|getpwuid_r|getrlimit|getrlimit64|getrpcbyname_r|getrpcbynumber_r|getrpcent_r|getservbyname_r|getservbyport_r|getservent_r|getspent_r|getspnam_r|msgctl|nftw|nftw64|pclose|semctl|sem_destroy|sem_getvalue|sem_init|sem_post|sem_trywait|sem_wait|setrlimit|shmctl|sys_sigabbrev|sys_siglist|tmpfile)$/) {
    sub(/^__new_/,"",weak);
  }

  if (name !~ /^__(libc|new)_/ && name != "__longjmp") {
    sub(/^__/,"",weak);
  }

  sub(/_2_1$/,"",weak);
  if(weak!=name) {
    printf("  %s = 0x%06x;\n", weak, adr);
  }
#  if(name=="__libc_siglongjmp") printf("  %s = 0x%06x;\n", "longjmp", adr);
}
