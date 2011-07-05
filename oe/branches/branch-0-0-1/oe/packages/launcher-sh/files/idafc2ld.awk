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
  
  sub(/::/,"__",name);
  sub(/\r$/,"",name);
  printf("  %s = 0x%06x;\n", name, adr);
  weak=name;
  sub(/^_IO_/,"",weak);
  sub(/^__libc_/,"",weak);
  sub(/^__new_/,"",weak);
  sub(/^__/,"",weak);
  sub(/_2_1$/,"",weak);
  if(weak!=name && name!="__longjmp") printf("  %s = 0x%06x;\n", weak, adr);
  if(name=="__libc_siglongjmp") printf("  %s = 0x%06x;\n", "longjmp", adr);
}
