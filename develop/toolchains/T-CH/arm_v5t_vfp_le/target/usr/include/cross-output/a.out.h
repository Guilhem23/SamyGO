

#ifndef A_OUT_H
#define A_OUT_H 1







struct exec
{
  unsigned long a_info;
  unsigned int a_text;
  unsigned int a_data;
  unsigned int a_bss;
  unsigned int a_syms;
  unsigned int a_entry;
  unsigned int a_trsize;
  unsigned int a_drsize;
};

enum machine_type
{
  M_OLDSUN2 = 0,
  M_68010 = 1,
  M_68020 = 2,
  M_SPARC = 3,
  M_386 = 100,
  M_MIPS1 = 151,
  M_MIPS2 = 152
};
struct nlist
{
  union
    {
      char *n_name;
      struct nlist *n_next;
      long n_strx;
    } n_un;
  unsigned char n_type;
  char n_other;
  short n_desc;
  unsigned long n_value;
};
struct relocation_info
{
  int r_address;
  unsigned int r_symbolnum:24;
  unsigned int r_pcrel:1;
  unsigned int r_length:2;
  unsigned int r_extern:1;
  unsigned int r_pad:4;
};


#define N_MAGIC(exec) ((exec).a_info & 0xffff)


#define QMAGIC 0314


#define ZMAGIC 0413


#endif
