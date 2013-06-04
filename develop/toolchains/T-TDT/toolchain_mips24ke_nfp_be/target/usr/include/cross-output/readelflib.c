

int process_elf32_file (const char *file_name, const char *lib, int *flag,
   unsigned int *osversion, char **soname,
   void *file_contents, size_t file_length);
int process_elf64_file (const char *file_name, const char *lib, int *flag,
   unsigned int *osversion, char **soname,
   void *file_contents, size_t file_length);


int
process_elf_file (const char *file_name, const char *lib, int *flag,
    unsigned int *osversion, char **soname, void *file_contents,
    size_t file_length)
{
  ElfW(Ehdr) *elf_header = (ElfW(Ehdr) *) file_contents;
  int ret;

  if (elf_header->e_ident [EI_CLASS] == ELFCLASS32)
    {
      ret = process_elf32_file (file_name, lib, flag, osversion, soname,
    file_contents, file_length);


      if (ret && (READ_FIELD (elf_header->e_flags) & EF_MIPS_ABI2) != 0)
 *flag = FLAG_MIPS64_LIBN32|FLAG_ELF_LIBC6;
    }
  else
    {
      ret = process_elf64_file (file_name, lib, flag, osversion, soname,
    file_contents, file_length);

      if (!ret)
 *flag = FLAG_MIPS64_LIBN64|FLAG_ELF_LIBC6;
    }

  return ret;
}





int
process_elf32_file (const char *file_name, const char *lib, int *flag,
    unsigned int *osversion, char **soname, void *file_contents,
    size_t file_length)
{
  int i;
  unsigned int j;
  unsigned int phnum;
  HostElfW(Addr) loadaddr;
  unsigned int dynamic_addr;
  size_t dynamic_size;
  char *program_interpreter;

  ElfW(Ehdr) *elf_header;
  ElfW(Phdr) *elf_pheader, *segment;
  ElfW(Dyn) *dynamic_segment, *dyn_entry;
  char *dynamic_strings;

  HostElfW(Sword) d_tag;

  elf_header = (ElfW(Ehdr) *) file_contents;
  *osversion = 0;

  if (elf_header->e_ident [EI_CLASS] != ElfW (CLASS))
    {
      if (opt_verbose)
 {
   if (elf_header->e_ident [EI_CLASS] == ELFCLASS32)
     error (0, 0, _("%s is a 32 bit ELF file.\n"), file_name);
   else if (elf_header->e_ident [EI_CLASS] == ELFCLASS64)
     error (0, 0, _("%s is a 64 bit ELF file.\n"), file_name);
   else
     error (0, 0, _("Unknown ELFCLASS in file %s.\n"), file_name);
 }
      return 1;
    }

  if (READ_FIELD (elf_header->e_type) != ET_DYN)
    {
      error (0, 0, _("%s is not a shared object file (Type: %d).\n"), file_name,
      READ_FIELD (elf_header->e_type));
      return 1;
    }


  elf_pheader = (ElfW(Phdr) *) (READ_FIELD (elf_header->e_phoff)
    + file_contents);
  do { if ((void *)(elf_pheader) < file_contents || (void *)(elf_pheader) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;



  *flag = FLAG_ELF;

  loadaddr = -1;
  dynamic_addr = 0;
  dynamic_size = 0;
  program_interpreter = NULL;
  phnum = READ_FIELD (elf_header->e_phnum);
  for (i = 0, segment = elf_pheader; i < phnum; i++, segment++)
    {
      do { if ((void *)(segment) < file_contents || (void *)(segment) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;

      switch (READ_FIELD (segment->p_type))
 {
 case PT_LOAD:
   if (loadaddr == (HostElfW(Addr)) -1)
     loadaddr = (READ_FIELD (segment->p_vaddr)
   - READ_FIELD (segment->p_offset));
   break;

 case PT_DYNAMIC:
   if (dynamic_addr)
     error (0, 0, _("more than one dynamic segment\n"));

   dynamic_addr = READ_FIELD (segment->p_offset);
   dynamic_size = READ_FIELD (segment->p_filesz);
   break;

 case PT_INTERP:
   program_interpreter = (char *) (file_contents
       + READ_FIELD (segment->p_offset));
   do { if ((void *)(program_interpreter) < file_contents || (void *)(program_interpreter) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;


   for (j = 0; j < sizeof (interpreters) / sizeof (interpreters [0]);
        ++j)
     if (strcmp (program_interpreter, interpreters[j].soname) == 0)
       {
  *flag = interpreters[j].flag;
  break;
       }
   break;

 case PT_NOTE:
   if (!*osversion
       && READ_FIELD (segment->p_filesz) == 32
       && READ_FIELD (segment->p_align) >= 4)
     {
       HostElfW(Off) p_offset = READ_FIELD (segment->p_offset);
       ElfW(Word) *abi_note = (ElfW(Word) *) (file_contents + p_offset);
       if (READ_FIELD (abi_note [0]) == 4
    && READ_FIELD (abi_note [1]) == 16
    && READ_FIELD (abi_note [2]) == 1
    && memcmp (abi_note + 3, "GNU", 4) == 0)
  *osversion = (READ_FIELD (abi_note [4]) << 24) |
        ((READ_FIELD (abi_note [5]) & 0xff) << 16) |
        ((READ_FIELD (abi_note [6]) & 0xff) << 8) |
        (READ_FIELD (abi_note [7]) & 0xff);
     }
   break;

 default:
   break;
 }

    }
  if (loadaddr == (HostElfW(Addr)) -1)
    {

      loadaddr = 0;
    }


  if (dynamic_size == 0)
    return 1;

  dynamic_segment = (ElfW(Dyn) *) (file_contents + dynamic_addr);
  do { if ((void *)(dynamic_segment) < file_contents || (void *)(dynamic_segment) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;


  dynamic_strings = NULL;
  for (dyn_entry = dynamic_segment;
       (d_tag = READ_FIELD (dyn_entry->d_tag)) != DT_NULL;
       ++dyn_entry)
    {
      do { if ((void *)(dyn_entry) < file_contents || (void *)(dyn_entry) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;
      if (d_tag == DT_STRTAB)
 {
   HostElfW(Word) d_val = READ_FIELD (dyn_entry->d_un.d_val);
   dynamic_strings = (char *) (file_contents + d_val - loadaddr);
   do { if ((void *)(dynamic_strings) < file_contents || (void *)(dynamic_strings) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;
   break;
 }
    }

  if (dynamic_strings == NULL)
    return 1;


  for (dyn_entry = dynamic_segment;
       (d_tag = READ_FIELD (dyn_entry->d_tag)) != DT_NULL;
       ++dyn_entry)
    {
      if (d_tag == DT_NEEDED || d_tag == DT_SONAME)
 {
   char *name = dynamic_strings + READ_FIELD (dyn_entry->d_un.d_val);
   do { if ((void *)(name) < file_contents || (void *)(name) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;

   if (d_tag == DT_NEEDED)
     {

       if (*flag == FLAG_ELF)
  {

    for (j = 0;
         j < sizeof (known_libs) / sizeof (known_libs [0]);
         ++j)
      if (strcmp (name, known_libs [j].soname) == 0)
        {
   *flag = known_libs [j].flag;
   break;
        }
  }
     }

   else if (d_tag == DT_SONAME)
     *soname = xstrdup (name);


   if (*soname && *flag != FLAG_ELF)
     return 0;
 }
    }




  if (*soname == NULL)
    *soname = xstrdup (lib);

  return 0;
}





int
process_elf64_file (const char *file_name, const char *lib, int *flag,
    unsigned int *osversion, char **soname, void *file_contents,
    size_t file_length)
{
  int i;
  unsigned int j;
  unsigned int phnum;
  HostElfW(Addr) loadaddr;
  unsigned int dynamic_addr;
  size_t dynamic_size;
  char *program_interpreter;

  ElfW(Ehdr) *elf_header;
  ElfW(Phdr) *elf_pheader, *segment;
  ElfW(Dyn) *dynamic_segment, *dyn_entry;
  char *dynamic_strings;

  HostElfW(Sword) d_tag;

  elf_header = (ElfW(Ehdr) *) file_contents;
  *osversion = 0;

  if (elf_header->e_ident [EI_CLASS] != ElfW (CLASS))
    {
      if (opt_verbose)
 {
   if (elf_header->e_ident [EI_CLASS] == ELFCLASS32)
     error (0, 0, _("%s is a 32 bit ELF file.\n"), file_name);
   else if (elf_header->e_ident [EI_CLASS] == ELFCLASS64)
     error (0, 0, _("%s is a 64 bit ELF file.\n"), file_name);
   else
     error (0, 0, _("Unknown ELFCLASS in file %s.\n"), file_name);
 }
      return 1;
    }

  if (READ_FIELD (elf_header->e_type) != ET_DYN)
    {
      error (0, 0, _("%s is not a shared object file (Type: %d).\n"), file_name,
      READ_FIELD (elf_header->e_type));
      return 1;
    }


  elf_pheader = (ElfW(Phdr) *) (READ_FIELD (elf_header->e_phoff)
    + file_contents);
  do { if ((void *)(elf_pheader) < file_contents || (void *)(elf_pheader) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;



  *flag = FLAG_ELF;

  loadaddr = -1;
  dynamic_addr = 0;
  dynamic_size = 0;
  program_interpreter = NULL;
  phnum = READ_FIELD (elf_header->e_phnum);
  for (i = 0, segment = elf_pheader; i < phnum; i++, segment++)
    {
      do { if ((void *)(segment) < file_contents || (void *)(segment) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;

      switch (READ_FIELD (segment->p_type))
 {
 case PT_LOAD:
   if (loadaddr == (HostElfW(Addr)) -1)
     loadaddr = (READ_FIELD (segment->p_vaddr)
   - READ_FIELD (segment->p_offset));
   break;

 case PT_DYNAMIC:
   if (dynamic_addr)
     error (0, 0, _("more than one dynamic segment\n"));

   dynamic_addr = READ_FIELD (segment->p_offset);
   dynamic_size = READ_FIELD (segment->p_filesz);
   break;

 case PT_INTERP:
   program_interpreter = (char *) (file_contents
       + READ_FIELD (segment->p_offset));
   do { if ((void *)(program_interpreter) < file_contents || (void *)(program_interpreter) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;


   for (j = 0; j < sizeof (interpreters) / sizeof (interpreters [0]);
        ++j)
     if (strcmp (program_interpreter, interpreters[j].soname) == 0)
       {
  *flag = interpreters[j].flag;
  break;
       }
   break;

 case PT_NOTE:
   if (!*osversion
       && READ_FIELD (segment->p_filesz) == 32
       && READ_FIELD (segment->p_align) >= 4)
     {
       HostElfW(Off) p_offset = READ_FIELD (segment->p_offset);
       ElfW(Word) *abi_note = (ElfW(Word) *) (file_contents + p_offset);
       if (READ_FIELD (abi_note [0]) == 4
    && READ_FIELD (abi_note [1]) == 16
    && READ_FIELD (abi_note [2]) == 1
    && memcmp (abi_note + 3, "GNU", 4) == 0)
  *osversion = (READ_FIELD (abi_note [4]) << 24) |
        ((READ_FIELD (abi_note [5]) & 0xff) << 16) |
        ((READ_FIELD (abi_note [6]) & 0xff) << 8) |
        (READ_FIELD (abi_note [7]) & 0xff);
     }
   break;

 default:
   break;
 }

    }
  if (loadaddr == (HostElfW(Addr)) -1)
    {

      loadaddr = 0;
    }


  if (dynamic_size == 0)
    return 1;

  dynamic_segment = (ElfW(Dyn) *) (file_contents + dynamic_addr);
  do { if ((void *)(dynamic_segment) < file_contents || (void *)(dynamic_segment) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;


  dynamic_strings = NULL;
  for (dyn_entry = dynamic_segment;
       (d_tag = READ_FIELD (dyn_entry->d_tag)) != DT_NULL;
       ++dyn_entry)
    {
      do { if ((void *)(dyn_entry) < file_contents || (void *)(dyn_entry) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;
      if (d_tag == DT_STRTAB)
 {
   HostElfW(Word) d_val = READ_FIELD (dyn_entry->d_un.d_val);
   dynamic_strings = (char *) (file_contents + d_val - loadaddr);
   do { if ((void *)(dynamic_strings) < file_contents || (void *)(dynamic_strings) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;
   break;
 }
    }

  if (dynamic_strings == NULL)
    return 1;


  for (dyn_entry = dynamic_segment;
       (d_tag = READ_FIELD (dyn_entry->d_tag)) != DT_NULL;
       ++dyn_entry)
    {
      if (d_tag == DT_NEEDED || d_tag == DT_SONAME)
 {
   char *name = dynamic_strings + READ_FIELD (dyn_entry->d_un.d_val);
   do { if ((void *)(name) < file_contents || (void *)(name) > (file_contents+file_length)) { error (0, 0, _("file %s is truncated\n"), file_name); return 1; } } while (0);;

   if (d_tag == DT_NEEDED)
     {

       if (*flag == FLAG_ELF)
  {

    for (j = 0;
         j < sizeof (known_libs) / sizeof (known_libs [0]);
         ++j)
      if (strcmp (name, known_libs [j].soname) == 0)
        {
   *flag = known_libs [j].flag;
   break;
        }
  }
     }

   else if (d_tag == DT_SONAME)
     *soname = xstrdup (name);


   if (*soname && *flag != FLAG_ELF)
     return 0;
 }
    }




  if (*soname == NULL)
    *soname = xstrdup (lib);

  return 0;
}
