#include <fuselagefs/fuselagefs.hh>
using namespace Fuselage;
using namespace Fuselage::Helpers;

#include <aio.h>
#include <errno.h>

#include <stdlib.h>

#include <strings.h>
#include <iostream>
#include <fcntl.h>
using namespace std;





const string PROGRAM_NAME = "mount.readahead";
// thise is same as BASEDIR in fuselage. But here it's required
const char*  CMDLINE_OPTION_BasePath_CSTR = 0;

class CustomFilesystem
:
	public Delegatefs
{
	typedef Delegatefs _Base;
	off_t m_oldOffset;
	off_t m_startNextAIOOffset;
	enum
	{
		aio_buffer_sz = 32 * 1024 * 1024,

		aio_consume_window = aio_buffer_sz * 1,
		debug_readahread_aio = false,
	};
	// char aio_buffer[ aio_buffer_sz ];
	bool forceNewReadAHead;

	char* aio_buffer;
	struct aiocb arg; 

	void schedule_readahread_aio( int fd, 
			off_t offset)
	{
		if (aio_error(&arg) != EINPROGRESS)
		{
			if (aio_return( &arg )!=0) {
				cerr << "aio_return():" 
					<< aio_return( &arg ) 
					   << endl;
				bzero( &arg, sizeof (struct aiocb)); 
			}

			if(m_startNextAIOOffset - aio_consume_window <= offset || forceNewReadAHead)
			{


				ssize_t retval; ssize_t nbytes; 
				bzero( &arg, sizeof (struct aiocb)); 
				arg.aio_fildes = fd;
				arg.aio_offset = m_startNextAIOOffset; 
				if (forceNewReadAHead) arg.aio_offset = offset; 
				arg.aio_buf = (void *) aio_buffer; 
				arg.aio_nbytes = aio_buffer_sz; 
				arg.aio_sigevent.sigev_notify = SIGEV_NONE; 
				// 	retval = posix_fadvise(fd, offset, aio_buffer_sz, POSIX_FADV_SEQUENTIAL);	
				cerr << "Starting an async read request"
					<< " at offset:" << arg.aio_offset << ", current offset" << offset <<endl;
				retval = aio_read( &arg );
				if( retval < 0 )
					cerr << "error starting aio request!" 
						<< endl;

				m_startNextAIOOffset = m_startNextAIOOffset + aio_buffer_sz;
				if (forceNewReadAHead) m_startNextAIOOffset = offset + aio_buffer_sz;

				if( debug_readahread_aio )
				{
					while ( (retval = aio_error( &arg ) ) 
							== EINPROGRESS )
					{sleep(1);}
					cerr << "aio_return():" 
						<< aio_return( &arg ) 
						   << endl;
				}
			}
			forceNewReadAHead = false;
		}
	}

	public:

	CustomFilesystem()
		:
			_Base(),
			m_startNextAIOOffset( 0 ),
			m_oldOffset( -1 )
	{
		aio_buffer= new char[aio_buffer_sz];
		bzero( &arg, sizeof (struct aiocb)); 
		forceNewReadAHead = true;
	}

	virtual int fs_read( const char *path, 
			char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
	{
		//   cerr << "fs_read() offset:" << offset
		//        << " sz:" << size << endl;
		int fd = fi->fh;

		//   bool forceNewReadAHead = false;
		if( (offset - size) != m_oldOffset )
		{
			cerr << "possible seek() between read()s!" 
				<< endl;
			forceNewReadAHead = true;
			aio_cancel( fd, 0 );
		}
		schedule_readahread_aio( fd, offset);
		m_oldOffset = offset;
		return _Base::fs_read( path, buf, 
				size, offset, fi );
	}
	ustomFilesystem()
	{
		delete[] aio_buffer;
	}

};





int main(int argc, const char* argv[])
{
	//    unsigned long ShowHelp          = 0;

	CustomFilesystem myfuse;

	struct poptOption* optionsTable = myfuse.getPopTable();

	poptContext optCon = poptGetContext(PROGRAM_NAME.c_str(), argc, (const char**)argv, optionsTable, 0);
	poptSetOtherOptionHelp(optCon, "[OPTIONS]* mountpoint");

	signed char c=-1;
	while ((c = poptGetNextOpt(optCon)) >= 0)
	{
	}

	if (c < -1) {
		// an error occurred during option processing
		fprintf(stderr, "%s: %s\n", 
				poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
				poptStrerror(c));
		return 1;
	}

	string mountPoint = "";
	while( const char* tCSTR = poptGetArg(optCon) )
	{
		string t = tCSTR;
		cerr << "t:" << t << endl;
		if( mountPoint.empty() )
			mountPoint = t;
	}

	if( mountPoint.empty() )
	{
		cerr << "no mountpoint given" << endl;

		poptPrintHelp(optCon, stderr, 0);
		exit(1);
	}

	//****************************************
	//*    //****************************************

	list<string> fuseArgs;
	fuseArgs.push_back( "readaheadfs" );

	myfuse.AugmentFUSEArgs( fuseArgs );

	// disable multi-threaded operation
	fuseArgs.push_back( "-s" );

	// allow mounts over non-empty file/dir
	//    fuseArgs.push_back( "-o" );
	//    fuseArgs.push_back( "nonempty" );

	// set filesystem name
	fuseArgs.push_back( "-o" );
	fuseArgs.push_back( "fsname=readaheadfs" );


	//     fuseArgs.push_back( "-o" );
	//     fuseArgs.push_back( "large_read" );

	fuseArgs.push_back( mountPoint );

	poptFreeContext(optCon);    
	return myfuse.main( fuseArgs );
	//return 0;

}


