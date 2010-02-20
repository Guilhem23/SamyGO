/*
   Elf file and process memory patcher
   Copyright (C) 2010 Tomas Vanek

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   SamyGo Home Page: http://samygo.sourceforge.net
   */

#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <libelf.h>

char *const*parg;
int pid;
int verbosity;
int compare;
int nocheck;
int check_failed;

char *d_fn;
int d_fd;
Elf *d_elf;
Elf_Scn *d_scn;
Elf32_Ehdr *d_ehdr;
Elf32_Shdr *d_shdr;
size_t d_ndx;
Elf_Data *d_data;

int s_fd;
Elf *s_elf;
Elf_Scn *s_scn;
Elf32_Ehdr *s_ehdr;
Elf32_Shdr *s_shdr;
size_t s_ndx;
Elf_Data *s_data;
char *s_sectname;

void usage(void)
{
	printf(
			"\n"
			"elfpatcher 1.1\n"
			"Takes one or more elf32 files and writes the content of segments named .patch*\n"
			"Patching a binary file is potentially dangerous, use solely at your own risk\n"
			"\n"
			"Usage:\n"
			"	elfpatcher [-v] [-c|-n]-p pid elf-patch-file...\n"
			"	elfpatcher [-v] [-c|-n] -e elf-file-to be patched elf-patch-file...\n"
			"\n"
			"-p pid.... stops running process using PTRACE, patches text or data\n"
			"           in process memory and continues the process\n"
			"-e file... writes patches to appropriate segments of elf32 file\n"
			"-c........ do not write, just check if patches are applied\n"
			"-n........ do not check if segments other than .patch* compare to orig data\n"
			"-v........ verbose: shows hex representation of patch\n"
			"-v -v..... double verbose: shows original data before applying patches too\n"
			"\n"
	      );
}

void PrintHex(unsigned char *bf, int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf(" %02x", *bf++);
}

void ElfErr(const char *fn_name)
{
	fprintf(stderr, "%s: %s\n", fn_name, elf_errmsg(-1));
}

int GetNextPatchSection()
{
	while (1) {
		if (!s_elf) {
			if (!*parg || !**parg) {
				return 1;
			}
			fprintf(stderr, "Reading patches from file %s\n", *parg);
			s_fd = open(*parg++, O_RDONLY);
			if (s_fd == -1) {
				perror("src open");
				continue;
			}

			s_elf = elf_begin(s_fd, ELF_C_READ, NULL);
			if (!s_elf) {
				ElfErr("src elf_begin");
				continue;
			}
			s_ehdr = elf32_getehdr(s_elf);
			if (!s_ehdr) {
				ElfErr("src elf_getehdr");
				elf_end(s_elf);
				s_elf = NULL;
				continue;
			}
			s_ndx = s_ehdr->e_shstrndx;
		}

		s_scn = elf_nextscn(s_elf, s_scn);
		if (!s_scn) {
			if (elf_errno())
				ElfErr("src elf_getscn");
			elf_end(s_elf);
			s_elf = NULL;
			continue;
		}

		s_shdr = elf32_getshdr(s_scn);
		if (!s_shdr) {
			ElfErr("src elf_getshdr");
			continue;
		}

		s_sectname = elf_strptr(s_elf, s_ndx, s_shdr->sh_name);
		if (!s_sectname) {
			fprintf(stderr, "src noname section?!\n");
			continue;
		}
		//      fprintf(stderr, "src section %s\n", s_sectname);
		if (s_shdr->sh_type != SHT_PROGBITS || ~s_shdr->sh_flags & SHF_ALLOC) {
			//	fprintf(stderr, "src non PROGBITS or non alloc flag\n");
			continue;
		}

		s_data = elf_getdata(s_scn, NULL);
		if (!s_data) {
			ElfErr("src elf_getdata");
			continue;
		}
		if (!s_data->d_buf && s_data->d_size < s_shdr->sh_size) {
			fprintf(stderr, "src elf_getdata none or small buf\n");
			continue;
		}
		break;
	}
	return 0;
}

int FindDestSection(int addr)
{
	char *sectname;

	if (d_shdr) {
		if (addr < d_shdr->sh_addr) {
			d_scn = NULL;
			d_shdr = NULL;
			d_data = NULL;
		}
	}

	while (1) {
		if (d_shdr && d_data) {
			if (addr < d_shdr->sh_addr) {
				fprintf(stderr, "Cannot find section by address\n");
				return 1;
			}
			if (addr < d_shdr->sh_addr + d_shdr->sh_size) {
				if (addr >= d_shdr->sh_addr + d_data->d_size) {
					fprintf(stderr, "dest elf_getdata small buf\n");
					return 1;
				}
				return 0;
			}
		}

		d_scn = elf_nextscn(d_elf, d_scn);
		if (!d_scn) {
			d_shdr = NULL;
			d_data = NULL;
			if (elf_errno()) {
				ElfErr("dest elf_getscn");
				continue;
			}
			return 2; // end of file
		}

		d_shdr = elf32_getshdr(d_scn);
		if (!d_shdr) {
			ElfErr("dest elf_getshdr");
			continue;
		}

		sectname = elf_strptr(d_elf, d_ndx, d_shdr->sh_name);
		if (!sectname) {
			fprintf(stderr, "dest noname section?!\n");
			continue;
		}
		//      fprintf(stderr, "dest section %s\n", sectname);
		if (d_shdr->sh_type != SHT_PROGBITS || ~d_shdr->sh_flags & SHF_ALLOC) {
			//      fprintf(stderr, "dest non PROGBITS or non alloc flag\n");
			continue;
		}

		if (addr >= d_shdr->sh_addr + d_shdr->sh_size) {
			continue;
		}

		d_data = elf_getdata(d_scn, NULL);
		if (!d_data) {
			ElfErr("dest elf_getdata");
		}
		fprintf(stderr, "destination section %s at 0x%07x\n", sectname, d_shdr->sh_addr);
	}
}

int PtracePeek(unsigned char *bf, int addr, int size)
{
	int i;
	union {
		long l;
		unsigned char b[2];
	} d;

	for (i = 0; i < s_shdr->sh_size;) {
		d.l = ptrace(PTRACE_PEEKTEXT, pid, (void *)(addr + i), NULL);
		if (errno) {
			return errno;
		}
		bf[i++] = d.b[0];
		bf[i++] = d.b[1];
	}
	return 0;
}

int PtracePoke(unsigned const char *bf, int addr, int size)
{
	int i;
	long pr;
	union {
		unsigned short w;
		unsigned char b[2];
	} d;

	for (i = 0; i < s_shdr->sh_size; i+= 2) {
		d.b[0] = bf[i];
		d.b[1] = bf[i+1];
		pr = ptrace(PTRACE_POKETEXT, pid, addr + i, d.w);
		if (pr == -1) {
			perror("ptrace poke");
			return errno;
		}
	}
	return 0;
}

int main(int argc, char *const argv[])
{
	pid_t pid2;
	long pr;
	int status;
	int r, opt;
	int result = 0;
	int w_fd= -1;

	while ((opt = getopt(argc, argv, "vcinp:e:")) != -1) {
		switch (opt) {
			case 'v':
				verbosity++;
				break;
			case 'c':
				compare = 1;
				break;
			case 'n':
				nocheck = 1;
				break;
			case 'p':
				pid = atoi(optarg);
				break;
			case 'e':
				d_fn = optarg;
				break;
			default:
				usage();
				exit(2);
		}
	}

	if (optind >= argc || (pid && d_fn)) {
		usage();
		exit(2);
	}
	parg = argv + optind;

	if (elf_version(EV_CURRENT) == EV_NONE) {
		fprintf(stderr, "elf_version error\n");
	}

	r = GetNextPatchSection();

	if (r) {
		exit(r);
	}

	if (d_fn) {
		fprintf(stderr, "Patching file %s\n", d_fn);
		d_fd = open(d_fn, O_RDWR);
		if (d_fd == -1) {
			perror("dest open");
			result= 2;
			goto err_exit1;
		}
		if (! compare) {
			w_fd= dup(d_fd);
		}

		d_elf = elf_begin(d_fd, ELF_C_READ, NULL);
		if (!d_elf) {
			ElfErr("dest elf_begin");
			result= 3;
			goto err_exit1;
		}
		d_ehdr = elf32_getehdr(d_elf);
		if (!d_ehdr) {
			ElfErr("dest elf_getehdr");
			result= 3;
			goto err_exit2;
		}
		d_ndx = d_ehdr->e_shstrndx;
	}

	if (pid) {
		pr = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
		if (pr == -1) {
			perror("ptrace attach");
			elf_end(s_elf);
			exit(10);
		}

		pid2 = waitpid(pid, &status, 0);
		if (pid2 == -1) {
			perror("wait");
			sleep(1);
		}
		if (pid != pid2) {
			fprintf(stderr, "pid mismatch\n");
		}
	}

	do {
		enum { OP_SKIP, OP_CHECK, OP_COMPARE, OP_WRITE } op;
		const char* op_name[]= {
			"Skiping",
			"Checking",
			"Comparing",
			"Writing"
		};
		Elf32_Addr addr = s_shdr->sh_addr;
		Elf32_Off off = s_shdr->sh_addr;
		unsigned char *pbf = NULL, *d_orig = NULL;

		if (check_failed) {
			op= OP_COMPARE;
		} else if (strncmp(".patch", s_sectname, 6)) {
			op = nocheck ? OP_SKIP :
				compare ? OP_COMPARE : OP_CHECK;
		} else {
			op = compare ? OP_COMPARE : OP_WRITE;
		}

		if (d_elf) {
			if (FindDestSection(addr)) {
				result = 3;
				op = OP_SKIP;
			} else if (d_shdr) {
				off += d_shdr->sh_offset - d_shdr->sh_addr;
				// convert to destination file offset

				addr -= d_shdr->sh_addr;
				// convert to destination section offset
			}
		}

		fprintf(stderr, "%s %s %d bytes at 0x%07x\n",
			op_name[op], s_sectname, s_shdr->sh_size, s_shdr->sh_addr);

		if (verbosity) {
			printf("0x%07x:", off);
		}

		if (op == OP_COMPARE || op == OP_CHECK || verbosity >= 2) {
			if (pid) {
				pbf = (unsigned char *)malloc(s_shdr->sh_size);
				if (pbf) {
					if (!PtracePeek(pbf, addr, s_shdr->sh_size)) {
						d_orig = pbf;
					}
				}
			} else if (d_data) {
				d_orig = d_data->d_buf + addr;
			}
		}

		if (verbosity >= 2 && d_orig) {
			PrintHex(d_orig, s_shdr->sh_size);
			printf(" :");
		}

		if (op == OP_CHECK) {
			if (! d_orig) {
				check_failed = 5;
				fprintf(stderr, "Cannot read data to check segment - write aborted\n");
			} else if (memcmp(d_orig, s_data->d_buf, s_shdr->sh_size)) {
				check_failed = 4;
				fprintf(stderr, "Segment differs - write aborted\n");
			}
		}

		if (op == OP_COMPARE && d_orig) {
			if (memcmp(d_orig, s_data->d_buf, s_shdr->sh_size)) {
				result = 1;
				fprintf(stderr, "Segment differs\n");
			} else {
				fprintf(stderr, "Segment data equal\n");
			}
		}

		if (op == OP_WRITE) {
			if (pid) {
				PtracePoke(s_data->d_buf, addr, s_shdr->sh_size);
			} else if (d_data) {
				r = lseek(w_fd, off, SEEK_SET);
				if (r < 0) {
					perror("lseek");
				} else {
					r = write(w_fd, s_data->d_buf, s_shdr->sh_size);
					if (r < 0) {
						perror("write");
					}
					if (r != s_shdr->sh_size) {
						fprintf(stderr, "write returned %d of %d\n", r, s_shdr->sh_size);
					}
				}
			}
		}

		if (verbosity) {
			PrintHex(s_data->d_buf, s_shdr->sh_size);
			printf("\n\n");
		}

		if (pbf) {
			free(pbf);
		}

		r = GetNextPatchSection();

	} while (r == 0);

	if (pid) {
		pr = ptrace(PTRACE_DETACH, pid, NULL, NULL);
		if (pr == -1) {
			perror("ptrace detach");
			result = 11;
		}
	}

err_exit2:
	if (w_fd >= 0) {
		close(w_fd);
	}
	elf_end(d_elf);
err_exit1:
	elf_end(s_elf);
	if (check_failed) return check_failed;
	return result;
}

